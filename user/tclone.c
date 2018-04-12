#include "types.h"
#include "stat.h"
#include "user.h"

int temp;
lock_t *lock;

void f(void* a, void* b) {
    printf(1, "Child: Inside function f...\n");
    lock_acquire(lock);
    temp++;
    lock_release(lock);
    printf(1, "Child: Done...c:%d\n", temp);
    exit();
}


int
main(int argc, char *argv[])
{
    //char* a  = "a";
    char* b  = "b";
    char* c  = "c";

    void (*func)(void*, void*) = &f; 
    lock = malloc(sizeof(lock_t));

    // Intializing the lock
    printf(1, "Parent: Initializing the lock\n");
    lock_init(lock); 
    printf(1, "Parent: ticket: %d, turn: %d\n", lock->ticket, lock->turn);

    //printf(1, "calling thread_create from tclone function\n");
    //printf(1, "function is pointing to: %p\n", func);

    temp = 0;
    printf(1, "Parent: c is %d\n", temp);

    printf(1, "Parent: count before creating threads: %d\n", temp);
    printf(1, "Parent: Creating threads...\n");
    int pid;
    //for (int i = 0; i < 5; i++) {
        pid = thread_create((void*)func, (void*)b, (void*)c);
	printf(1, "Parent: pid of child process: %d\n", pid);
    //}

    printf(1, "Parent: Joining threads...\n");
    thread_join();
    
    printf(1, "Parent: Done joining the threads...\n");
    printf(1, "Parent: c after joining threads: %d\n", temp);

    exit();
}
