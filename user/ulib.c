#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"

static inline int fetch_and_add(int *var, int val) {
    //printf(1, "Inside fetch_and_add\n");
    //printf(1, "var=%d\n", *var);    
    //printf(1, "val=%d\n", val);    
    
    __asm__ volatile
    ("lock; xaddl %0, %1"
	: "+r" (val),  "+m" (*var) // input + output
	: // No input
	: "memory"
    );
    return val;
}

void lock_init(lock_t *lock) {
    //printf(1, "Inside lock_init\n");
    lock->ticket = 0;
    lock->turn = 0;
}

void lock_acquire(lock_t *lock) {
    //printf(1, "Inside lock_acquire: turn: %d\n", lock->turn);
    //printf(1, "value of lock->ticker before fetch: %d\n", lock->ticket);
    int myTurn = fetch_and_add(&lock->ticket, 1);  
    //printf(1, "value of lock->ticker after fetch: %d\n", lock->ticket);
    while (lock->turn != myTurn) {
	// spin
    }
}

void lock_release(lock_t *lock) {
    //printf(1, "Inside lock_release: turn: %d\n", lock->turn);
    lock->turn = lock->turn + 1;
}

int thread_create(void* start_routine, void* arg1, void* arg2) {
    printf(1, "Inside thread_create function\n");
  
    // Allocate memory for the stack
    int PGSIZE = 4096;
   
    void* ptrToFree = malloc(PGSIZE * 2);
    void* stack;
    
    // Page aligning the stack
    if((int)ptrToFree % PGSIZE == 0) {
	stack = ptrToFree;
    } else {
	int offset = PGSIZE - ((int)ptrToFree % PGSIZE);
	stack = ptrToFree + offset;
    }
    
    //structPtrs[count].ptrToFree = ptrToFree;
    //structPtrs[count].stack = stack;
    //count++;
    
    //printf(1, "ptrToFree: %p\n", ptrToFree);
    //printf(1, "stack: %p\n", stack);
    
    //printf(1, "ptrToFree in struct: %p\n", structPtrs[count - 1].ptrToFree);
    //printf(1, "stack in struct: %p\n", structPtrs[count - 1].stack);

    //printf(1, "valid reference %s\n", (char*)(stack + PGSIZE));

    //printf(1, "calling clone from thread_create\n");
    int pid = clone(start_routine, arg1, arg2, stack);

    // return fom thread_create
    return pid;
}

int thread_join() {
    //printf(1, "Inside thread_join function\n");
   
    void *stack;

    // Free the memory for the stack
    //for (int i = 0; i < count; i++) {
	//printf(1, "ptrToFree in struct: %p\n", structPtrs[i].ptrToFree);
	//printf(1, "stack in struct: %p\n", structPtrs[i].stack);
	//stack = &(structPtrs[i].stack);
	//printf(1, "void **stack %p\n", stack);
	//printf(1, "void *stack %p\n", *(int*)stack);
	int ret = join(&stack);
	//printf(1, "Done with join\n");
	//free(structPtrs[i].ptrToFree);
    //}
    //printf(1, "child: thread join done\n");
    return ret;
}

char*
strcpy(char *s, char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
stat(char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, void *vsrc, int n)
{
  char *dst, *src;
  
  dst = vdst;
  src = vsrc;
  while(n-- > 0)
    *dst++ = *src++;
  return vdst;
}
