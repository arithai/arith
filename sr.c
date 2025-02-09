#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
void *alloc_mmap(const char *filename, size_t length)
{
    int fd;
    printf("%s(%d) filename=%s\n",__FILE__,__LINE__,filename);
    fd = open(filename, (O_RDWR | O_CREAT), S_IWUSR);
    printf("%s(%d) filename=%s\n",__FILE__,__LINE__,filename);
    if (fd < 0) {
        perror("Couldn't create output file\n");
        return NULL;
    }
    if (ftruncate(fd, length)) {
        perror("Couldn't grow output file\n");
        close(fd);
        return NULL;
    }
    void *p = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, fd, 0);
    printf("%s(%d) filename=%s\n",__FILE__,__LINE__,filename);
    if (p == -1) {
        perror("mmap failed");
        close(fd);
        return NULL;
    }
    close(fd);
    printf("%s(%d) filename=%s\n",__FILE__,__LINE__,filename);
    return p;
}

void* create_shared_memory(size_t size) {
  // Our memory buffer will be readable and writable:
  int protection = PROT_READ | PROT_WRITE;

  // The buffer will be shared (meaning other processes can access it), but
  // anonymous (meaning third-party processes cannot obtain an address for it),
  // so only this process and its children will be able to use it:
  int visibility = MAP_SHARED | MAP_ANONYMOUS;

  // The remaining parameters to `mmap()` are not important for this use case,
  // but the manpage for `mmap` explains their purpose.
  return mmap(NULL, size, protection, visibility, 0xff3, 0);
}
int main(int argc, char **argv)
{
  char parent_message[] = "hello";  // parent process will write this message
  char child_message[] = "goodbye2"; // child process will then write this one

//void* shmem = create_shared_memory(128);
  void* shmem = alloc_mmap("/media/sf_u14/github/arith/sh.txt", 128);

    printf("%s(%d)\n",__FILE__,__LINE__);
  memcpy(shmem, parent_message, sizeof(parent_message));
    printf("%s(%d)\n",__FILE__,__LINE__);

  int pid = fork();

  if (pid == 0) {
    printf("Child read: %s\n", shmem);
    memcpy(shmem, child_message, sizeof(child_message));
    printf("Child wrote: %s\n", shmem);

  } else {
    printf("Parent read: %s\n", shmem);
    sleep(10);
    printf("After 1s, parent read: %s\n", shmem);
  }
}
