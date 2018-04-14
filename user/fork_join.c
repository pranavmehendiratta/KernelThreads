/*
 * after fork(), join() should not succeed
 * Authors:
 * - Varun Naik, Spring 2018
 * - Inspired by a test case from Spring 2016, last modified by Akshay Uttamani
 */
#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 0x1000
#define check(exp, msg) if(exp) {} else {\
  printf(1, "%s:%d check (" #exp ") failed: %s\n", __FILE__, __LINE__, msg);\
  printf(1, "TEST FAILED\n");\
  kill(ppid);\
  exit();}

int ppid = 0;

int
main(int argc, char *argv[])
{
  void *stack;
  int var = 0;
  int pid;

  ppid = getpid();
  
  printf(1, "Parent pid: %d\n", ppid);
  
  check(ppid > 2, "getpid() failed");

  pid = fork();
  
  printf(1, "Forked process pid: %d\n", pid);
  
  check(pid >= 0, "fork() failed");

  if (pid == 0) {
    // Child process
    
    printf(1, "Inside child process\n");
    
    pid = getpid();
    check(pid > ppid, "fork() failed");
    ++var;
    printf(1, "Calling exit from child process\n");
    exit();
    check(0, "Continued after exit");
  }

  printf(1, "Outside child process\n");

  // Parent process
  check(pid > ppid, "fork() failed");

  printf(1, "Calling join\n");
  pid = join(&stack);
  printf(1, "Done Calling join\n");

  check(pid == -1, "join() returned the wrong pid");

  printf(1, "Waiting for child process\n");
  pid = wait();
  printf(1, "Done Waiting for child process\n");
  check(pid > ppid, "wait() failed");
  check(var == 0, "var is incorrect");

  printf(1, "PASSED TEST!\n");
  exit();
}
