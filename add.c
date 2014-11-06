#include "easy_pthreads.c"

int add(int a, int b) { 
    sleep(1);
    return a + b; 
}

pj *pj_create_add(int a, int b);
void pj_exec_add(pj *job);
int pj_join_add(pj *job);
void pj_cleanup_add(pj* job);

pj *pj_create_add(int a, int b) {
    // Allocate memory for args
    int   *pa   = calloc(1, sizeof(int));
    int   *pb   = calloc(1, sizeof(int));
    // and arg pointers
    void **args = calloc(2, sizeof(void *));
    // and return value
    int   *ret  = calloc(1, sizeof(int));

    // Set the argument values for the job equal to the ones passed here.
    // If these are strings, arrays, or structs, they will need to be copied.
    *pa = a, *pb = b;
    // and make the argument pointers point to the right place.
    args[0] = pa, args[1] = pb;

    // Branch off into a new thread to execute...
    pj *job = pj_create(args, ret, pj_exec_add);
    return job;
}

void pj_exec_add(pj *job) {
    // Recover the arguments.
    int a = *((int *)job->args[0]);
    int b = *((int *)job->args[1]);
    // Execute the function.
    int ret = add(a, b);
    // Put the return value where we can get at it.
    *((int *)job->ret) = ret;
}

int pj_join_add(pj *job) {
    pj_join(job);
    int ret = *((int *)job->ret);
    return ret;
}

int main(void) {
    int i = 0;
    pj *jobs[10] = { NULL };

    // Start the threads
    for(i = 0; i < 10; i++) {
        jobs[i] = pj_create_add(i * i, 2 * i - 1);
    }

    // Get results from all threads.
    for(i = 0; i < 10; i++) {
        int result = pj_join_add(jobs[i]);
        printf("Job %i: result == %i\n", i, result);
    }

    return 0;
}
