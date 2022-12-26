/* fork-example-1.c */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

void* create_shared_memory(size_t size) {
  return mmap(NULL,
              size,
              PROT_READ | PROT_WRITE,
              MAP_SHARED | MAP_ANONYMOUS,
              -1, 0);
}


int main() {
  int* shmem = create_shared_memory(10 * sizeof(int));

  for (int i = 0; i < 10; i++) {
    shmem[i] = i+1;
  }

  printf("Shared memory at: %p\n" , shmem);
  int pid = fork();
  if (pid == -1) return;
  
  if (pid == 0) {
    int index = 0;
    int num = 0;
    scanf("%d %d", &index, &num);
    if (index >= 0 && index < 10) {
      shmem[index] = num;
    }
  } else {
    printf("Child's pid is: %d\n", pid);

    wait(NULL);

    for (int i = 0; i < 10; i++) {
      printf("%d ", shmem[i]);
    }
    printf("\n");
  }
}