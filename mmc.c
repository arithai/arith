#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
/*
 * pvtmMmapAlloc - creates a memory mapped file area.  
 * The return value is a page-aligned memory value, or NULL if there is a failure.
 * Here's the list of arguments:
 * @mmapFileName - the name of the memory mapped file
 * @size - the size of the memory mapped file (should be a multiple of the system page for best performance)
 * @create - determines whether or not the area should be created.
 */
void* pvtmMmapAlloc (char * mmapFileName, size_t size, char create)  
{      
  void * retv = NULL;                                                                                              
  if (create)                                                                                         
  {                                                                                                   
    mode_t origMask = umask(0);                                                                       
    int mmapFd = open(mmapFileName, O_CREAT|O_RDWR, 00666);                                           
    umask(origMask);                                                                                  
    if (mmapFd < 0)                                                                                   
    {                                                                                                 
      perror("open mmapFd failed");                                                                   
      return NULL;                                                                                    
    }                                                                                                 
    if ((ftruncate(mmapFd, size) == 0))               
    {                                                                                                 
      int result = lseek(mmapFd, size - 1, SEEK_SET);               
      if (result == -1)                                                                               
      {                                                                                               
        perror("lseek mmapFd failed");                                                                
        close(mmapFd);                                                                                
        return NULL;                                                                                  
      }                                                                                               

      /* Something needs to be written at the end of the file to                                      
       * have the file actually have the new size.                                                    
       * Just writing an empty string at the current file position will do.                           
       * Note:                                                                                        
       *  - The current position in the file is at the end of the stretched                           
       *    file due to the call to lseek().  
              *  - The current position in the file is at the end of the stretched                    
       *    file due to the call to lseek().                                                          
       *  - An empty string is actually a single '\0' character, so a zero-byte                       
       *    will be written at the last byte of the file.                                             
       */                                                                                             
      result = write(mmapFd, "", 1);                                                                  
      if (result != 1)                                                                                
      {                                                                                               
        perror("write mmapFd failed");                                                                
        close(mmapFd);                                                                                
        return NULL;                                                                                  
      }                                                                                               
      retv  =  mmap(NULL, size,   
                  PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, mmapFd, 0);                                     

      if (retv == MAP_FAILED || retv == NULL)                                                         
      {                                                                                               
        perror("mmap");                                                                               
        close(mmapFd);                                                                                
        return NULL;                                                                                  
      }                                                                                               
    }                                                                                                 
  }                                                                                                   
  else                                                                                                
  {                                                                                                   
    int mmapFd = open(mmapFileName, O_RDWR, 00666);                                                   
    if (mmapFd < 0)                                                                                   
    {                                                                                                 
      return NULL;                                                                                    
    }                                                                                                 
    int result = lseek(mmapFd, 0, SEEK_END);                                                          
    if (result == -1)                                                                                 
    {                                                                                                 
      perror("lseek mmapFd failed");                  
      close(mmapFd);                                                                                  
      return NULL;                                                                                    
    }                                                                                                 
    if (result == 0)                                                                                  
    {                                                                                                 
      perror("The file has 0 bytes");                           
      close(mmapFd);                                                                                  
      return NULL;                                                                                    
    }                                                                                              
    retv  =  mmap(NULL, size,     
                PROT_READ | PROT_WRITE, MAP_SHARED, mmapFd, 0);                                       

    if (retv == MAP_FAILED || retv == NULL)                                                           
    {                                                                                                 
      perror("mmap");                                                                                 
      close(mmapFd);                                                                                  
      return NULL;                                                                                    
    }                                                                                                 

    close(mmapFd);                                                                                    

  }                                                                                                   
  return retv;                                                                                        
}   
#include <stdbool.h>
int main(int argc, char **argv)
{
  char parent_message[] = "hello";  // parent process will write this message
  char child_message[] = "goodbye2"; // child process will then write this one

  void* shmem = pvtmMmapAlloc("/media/sf_u14/github/arith/sh.txt", 128, false);
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
