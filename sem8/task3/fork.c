/* fork-example-1.c */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

void* create_shared_memory(size_t size) {
  return mmap(NULL,
              size,
              PROT_READ | PROT_WRITE,
              MAP_SHARED | MAP_ANONYMOUS,
              -1, 0);
}


sem_t readySem;
sem_t exitSem;

pthread_t reader, displayer;

int* shmem;

void* do_child_process(void*_) {
  char buf[100];
  int index = 0;
  int num = 0;
  while(1) {
    fgets(buf, 100, stdin);
    if (strcmp(buf, "-\n")==0) {
      sem_post(&exitSem);
      sem_post(&readySem);
      break;
    }
    sscanf(buf, "%d %d", &index, &num);
    if (index >= 0 && index < 10) {
      shmem[index] = num;
      sem_post(&readySem);
    }
  }
  return NULL;
}

void* do_parent_process(void*_) {
  while(1) {
    sem_wait(&readySem);

    if (sem_trywait(&exitSem)==0) break;

    for (int i = 0; i < 10; i++) {
      printf("%d ", shmem[i]);
    }
    printf("\n");
  }
  return NULL;
}

int main() {
  sem_init(&readySem, 0, 0);
  sem_init(&exitSem, 0, 0);

  shmem = create_shared_memory(10 * sizeof(int));

  for (int i = 0; i < 10; i++) {
    shmem[i] = i+1;
  }

  printf("Shared memory at: %p\n" , shmem);

  pthread_create(&displayer, NULL, do_parent_process, NULL);
  pthread_create(&reader, NULL, do_child_process, NULL);

  pthread_exit(NULL);
  // int pid = fork();

  // if (pid == 0) {
  //   do_child_process(shmem);
  // } else {
  //   printf("Child's pid is: %d\n", pid);
  //   do_parent_process(shmem);
  // }
}