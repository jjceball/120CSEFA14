#include "buffpipe.h"

BuffPipe::BuffPipe() {
    queue = new List;
    semaphore = new Semaphore("buffer semaphore", 0);
    lock = new Lock("buffer lock");
}

BuffPipe::~BuffPipe() {
    delete queue;
    delete semaphore;
    delete lock;
}

char BuffPipe::pop() {
    char ch;
    semaphore->P();
    lock->Acquire();
    ch = (char)(int)queue->Remove();
    lock->Release();
    return ch;
}

int BuffPipe::push(char ch) {
    lock->Acquire(); 
    queue->Append((void*)(int)ch);
    lock->Release();
    semaphore->V();
    return 1;
}

int test_buffer_pipe() {
    char c[3] = {0};
    buffpipe = new BuffPipe();

    buffpipe->push('1');
    buffpipe->push('2');
    c[0] = buffpipe->pop();
    buffpipe->push('3');
    c[1] = buffpipe->pop();
    c[2] = buffpipe->pop();

    ASSERT(c[0] = '1');
    ASSERT(c[1] = '2');
    ASSERT(c[2] = '3');
    printf("buffer_test\n");

    delete buffpipe;
    return 1;
}
