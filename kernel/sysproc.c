#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"

int
sys_clone(void)
{
    void* func;
    void* arg1;
    void* arg2;
    void* stack;
    
    //cprintf("Inside sys_clone in kernel/sysproc.c\n");
    
    // Getting the first parameter
    if (argptr(0, (void*)&func, sizeof(func)) < 0) {
	cprintf("Unable to get the pointer\n");
	return -1;
    }
    //cprintf("value of func: %s\n", (char*)func);
    
    // Getting the second parameter
    if (argptr(1, (void*)&arg1, sizeof(arg1)) < 0) {
	cprintf("Unable to get the pointer\n");
	return -1;
    }
    cprintf("value of arg1: %d\n", *(int*)arg1);
    
    // Getting the third parameter
    if (argptr(2, (void*)&arg2, sizeof(arg2)) < 0) {
	cprintf("Unable to get the pointer\n");
	return -1;
    }
    cprintf("value of arg2: %d\n", *(int*)arg2);
    
    // Getting the fourth parameter
    if (argptr(3, (void*)&stack, sizeof(stack)) < 0) {
	cprintf("Unable to get the pointer\n");
	return -1;
    }

    //cprintf("value of stack: %s\n", stack);
    //cprintf("Calling clone from sys_clone\n");
    
    int pid = clone(func, arg1, arg2, stack);
    return pid;
}

int
sys_join(void) {
    //cprintf("Inside sys_join in kernel/sysproc.c\n");
    
    void **stack;
    
    // Getting the first parameter
    if (argptr(0, (void*)&stack, sizeof(stack)) < 0) {
        cprintf("Unable to get the pointer\n");
        return -1;
    }

    //cprintf("stack is pointing to %p\n", stack);
    //cprintf("dereferenced stack %p\n", *(int*)stack);
    
    return join(stack);
}



int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
