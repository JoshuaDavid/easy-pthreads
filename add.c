#include "easy_pthreads.c"

int add(int a, int b) { 
    sleep(1);
    return a + b; 
}

void call_add(pj *job) {
    void **args = job->args;
    void *ret = job->ret;
    int* retVal = calloc(1, sizeof(int));
    *retVal = add(*((int *)args[0]), *((int *)args[1]));
    job->ret = retVal;
}

int main(void) {
    int i = 0;
    void *rets[10];
    pj *jobs[10];
    // Start the threads
    for(i = 0; i < 10; i++) {
        int   *ab   = calloc(2, sizeof(int));
        void **args = calloc(2, sizeof(void *));
        rets[i]     = calloc(1, sizeof(int));
        ab [0] = i * 2 + 5, ab  [1] = i * i;
        args[0] = &(ab[0]), args[1] = &(ab[1]);
        jobs[i] = pj_create(args, rets[i], call_add);
    }
    // Get results from all threads.
    for(i = 0; i < 10; i++) {
        pj_join(jobs[i]);
        int a = *((int *)jobs[i]->args[0]);
        int b = *((int *)jobs[i]->args[1]);
        int ret = *((int *)jobs[i]->ret);
        printf("add(%i, %i) == %i\n", a, b, ret);
    }
}
