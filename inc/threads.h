#ifndef __THREADS_H__
#define __THREADS_H__

struct thread {
    void *sp;
    void *stack_start;
    int size;
    int state;
};

struct thread_data  {
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rbx;
    uint64_t rbp;
    void *start_addr;
    void (*fun_ptr)(void *);
    void *arg;
};

int get_cur_thread();

void init_threads();

int create_thread(void (*fun)(void *), void *arg);

void run(int id);

void yield();

void finish_thread();

void free_thread(int prev_thread);

int switch_threads(void ** old_sp, void * new_sp);

#endif