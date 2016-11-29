#include <ints.h>
#include <threads.h>
#include <buddy.h>
#include <assert.h>
#include <lock.h>

#define CREATE 0
#define RUN 1
#define FINISH 2

#define THREAD_COUNT 100


static volatile int cur_thread = 1;
extern void *start_thread;
volatile struct thread threads[THREAD_COUNT];
uint8_t size = 2;

int get_cur_thread() {
    return cur_thread;
}

void init_threads() {
    threads[1].state = RUN;
    threads[0].state = CREATE;
}

int create_thread(void (*fun)(void*), void *arg) {
    lock();

    volatile struct thread *new_thread = &threads[size];
    size++;
    new_thread->size = 4;
    new_thread->stack_start = (void*) buddy_alloc(new_thread->size, 0);
    new_thread->sp = (uint8_t*) new_thread->stack_start + 4096 * (1 << (new_thread->size)) - sizeof(struct thread_data);

    struct thread_data *init_thread_data = new_thread->sp;
    init_thread_data->r12 = 0;
    init_thread_data->r13 = 0;
    init_thread_data->r14 = 0;
    init_thread_data->r15 = 0;
    init_thread_data->rbx = 0;
    init_thread_data->rbp = 0;
    init_thread_data->start_addr = &start_thread;
    init_thread_data->fun_ptr = fun;
    init_thread_data->arg = arg;
    new_thread->state = RUN;

    unlock();
    return (int)(new_thread - threads);
}

void run(int id) {
    if (cur_thread == id) {
        return;
    }

    struct thread *thread = (struct thread *) threads + id;
    int prev = cur_thread;
    cur_thread = id;
    struct thread *pthread = (struct thread *) threads + prev;
    int pr = switch_threads(&pthread->sp, thread->sp);
    free_thread(pr);
}

void yield() {
    for (int i = (cur_thread + 1) % size; ; i = (i+1) % size) {
        if (i != 0 && threads[i].state == RUN) {
          run(i);
          break;
        }
    }
}

void finish_thread() {
    volatile struct thread *cur = (struct thread *) threads + cur_thread;
    cur->state = FINISH;
    yield();
}

void free_thread(int prev_thread) {
    volatile struct thread *thread = (struct thread *) threads + prev_thread;
    if (thread->state == FINISH && prev_thread != cur_thread) {
        buddy_free(thread->stack_start);
        thread->state = CREATE;
    }
}