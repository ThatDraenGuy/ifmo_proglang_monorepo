/* fork-example-1.c */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

struct __attribute__((packed)) pipes {
  int toPipe;
  int fromPipe;
};

enum
{
  MSG_STRING = 1,
  MSG_READY,
  MSG_END
};

enum 
{
  MESSAGE_MAGIC = 0xAFAF, // magic signature value
  MAX_MESSAGE_LEN = 4096  // maximum message length
};

struct __attribute__((packed)) message_header 
{
  uint16_t magic;     // magic signature
  uint16_t type;      // type of the message
  uint16_t data_len;  // length of data
}; 

enum
{
  // maximum data length
  MAX_MESSAGE_DATA_LEN = MAX_MESSAGE_LEN - sizeof(struct message_header)
};

struct __attribute__((packed)) message 
{
  struct message_header header;
  // payload
  uint8_t data[MAX_MESSAGE_DATA_LEN];
};

struct message simple_message(uint16_t type)
{
  return (struct message) {(struct message_header) {MESSAGE_MAGIC, type, 0}, 0};
}

struct message string_message(const char *str)
{
  struct message msg = (struct message) {(struct message_header) {MESSAGE_MAGIC, MSG_STRING, strlen(str) + 1}, 0};
  strcpy(msg.data, str);
  return msg;
}

int send(int fd, const struct message *msg)
{
  /* Check if the input data is not empty */
  if (fd < 0 || msg == NULL)
    return -1;
  
  /* Calculate the message size to send */
  int msg_size = sizeof(struct message_header) + msg->header.data_len;
  
  /* Check if message payload size is valid */
  if (msg->header.data_len > MAX_MESSAGE_DATA_LEN)
    return -1;
  
  /* Write data to the output pipe (we assume it is ready) */
  if (write(fd, msg, msg_size) != msg_size)
    return -2;
  
  return 0;
}

int receive(int fd, struct message *msg)
{
  /* Check if the input data is not empty */
  if (fd < 0 || msg == NULL)
    return -1;

  /* Try to read header */
  int msg_size = read(fd, &msg->header, sizeof(struct message_header));
  if (msg_size == 0)
    return 0;

  /* Check header magic */
  if (msg->header.magic != MESSAGE_MAGIC)
    return -2;

  /* Check if message has payload */
  if (msg->header.data_len > MAX_MESSAGE_DATA_LEN)
    return -2;
  if (msg->header.data_len > 0)
    msg_size += read(fd, &msg->data, msg->header.data_len);

  /* Return number of bytes read */
  return msg_size;

}


void* create_shared_memory(size_t size) {
  return mmap(NULL,
              size,
              PROT_READ | PROT_WRITE,
              MAP_SHARED | MAP_ANONYMOUS,
              -1, 0);
}

struct pipes setup_child_pipes(int pipes[2][2]) {
  // Сохраним нужные дескпиторы конвейеров
    int to_parent_pipe = pipes[1][1];
    int from_parent_pipe = pipes[0][0];

    // И закроем ненужные
    close(pipes[1][0]);
    close(pipes[0][1]);

    return (struct pipes) {.toPipe=to_parent_pipe, .fromPipe=from_parent_pipe};
}
struct pipes setup_parent_pipes(int pipes[2][2]) {
  // Сохраним нужные дескпиторы конвейеров
  int from_child_pipe = pipes[1][0];
  int to_child_pipe = pipes[0][1];

  // И закроем ненужные
  close(pipes[1][1]);
  close(pipes[0][0]);

  return (struct pipes) {.toPipe=to_child_pipe, .fromPipe=from_child_pipe};
}

void do_child_process(int* shmem, struct pipes pipes) {
  char buf[100];
  int index = 0;
  int num = 0;
  while(1) {
    fgets(buf, 100, stdin);
    if (strcmp(buf, "-\n")==0) {
      struct message msg = simple_message(MSG_END);
      send(pipes.toPipe, &msg);
      break;
    }
    sscanf(buf, "%d %d", &index, &num);
    if (index >= 0 && index < 10) {
      shmem[index] = num;
      struct message msg = simple_message(MSG_READY);
      send(pipes.toPipe, &msg);
    }
  }

  // Закроем дескпиторы
  close(pipes.fromPipe);
  close(pipes.toPipe);

}

void do_parent_process(int* shmem, struct pipes pipes) {
  struct message msg = {0};
  while(1) {
    int ret;
    while (ret = receive(pipes.fromPipe, &msg) == 0) {}
    if (ret < 0) return;

    if (msg.header.type==MSG_END) return;
    if (msg.header.type==MSG_READY) {
      for (int i = 0; i < 10; i++) {
        printf("%d ", shmem[i]);
      }
      printf("\n");
    }
  }


}

int main() {
  int* shmem = create_shared_memory(10 * sizeof(int));
  for (int i = 0; i < 10; i++) {
    shmem[i] = i+1;
  }

  printf("Shared memory at: %p\n" , shmem);

  // Создадим два конвейера
  int pipes[2][2];
  pipe(pipes[0]);
  pipe(pipes[1]);
  

  int pid = fork();

  if (pid == 0) {
    do_child_process(shmem, setup_child_pipes(pipes));

  } else {
    printf("Child's pid is: %d\n", pid);

    struct pipes parentPipes = setup_parent_pipes(pipes);
    do_parent_process(shmem, parentPipes);


    // Дождемся завершения ребенка
    waitpid(pid, NULL, 0);

    // Закроем дескпиторы
    close(parentPipes.fromPipe);
    close(parentPipes.toPipe);
  }
}