/*
 * create many threads, and make them all zombies
 * Authors:
 * - Varun Naik, Spring 2018
 */
#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 0x1000
#define NUM_THREADS 1
#define check(exp, msg) if(exp) {} else {\
  printf(1, "%s:%d check (" #exp ") failed: %s\n", __FILE__, __LINE__, msg);\
  printf(1, "TEST FAILED\n");\
  kill(ppid);\
  exit();}

int ppid = 0;
volatile int global = 0;
volatile int lastpid = 0;

void
func(void *arg1, void *arg2)
{
  int pid;
  pid = getpid();
  printf(1,"pid of child thread in func: %d\n", pid);

  // Sleep, so that (most of) the child thread runs after the main thread exits
  sleep(1000);

  // Make sure the scheduler is sane
  check(global == 1, "global is incorrect");


  check(ppid < pid && pid <= lastpid, "getpid() returned the wrong pid");

  if (pid == lastpid) {
    sleep(100);
    printf(1, "PASSED TEST!\n");
  }

    printf(1, "Calling exit from thread\n");
    exit();

  check(0, "Continued after exit");
}

int
main(int argc, char *argv[])
{
  int pid, i;
  char *addr;

  ppid = getpid();
  check(ppid > 2, "getpid() failed");

  // Expand address space for stacks
  addr = sbrk(NUM_THREADS*PGSIZE);
  check((uint)&argc < 3*PGSIZE, "Stack of main thread is not in first three pages");
  check(addr != (char *)-1, "sbrk() failed");

  pid = fork();
  check(pid >= 0, "fork() failed");

  if (pid > 0) {
    // Parent process: do not return to command line until all child threads
    // have joined

    check(pid > ppid, "fork() failed");

    // Wait for main thread of child process
    pid = wait();
    check(pid > ppid, "wait() failed");
    sleep(300);

    exit();
    check(0, "Continued after exit");
  } else {
    // Child process


    pid = getpid();

    printf(1, "pid of the child proc about to exit: %d\n", pid);
    
    check(pid > ppid, "fork() failed");
    ppid = pid;
    lastpid = ppid;

    for (i = 0; i < NUM_THREADS; ++i) {
      pid = clone(&func, NULL, NULL, (void *)(addr + i*PGSIZE));
      check(pid != -1, "Not enough threads created");
      check(pid > lastpid, "clone() returned the wrong pid");
      lastpid = pid;
    }

    printf(1, "Created %d child threads, creator thread exiting..., global: %d\n", NUM_THREADS, global);
    global = 1;
    
    printf(1, "Parent calling exit\n");
    
    exit();

    check(0, "Continued after exit");
  }
}
