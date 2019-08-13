#ifndef THPOOL_H
#define THPOOL_H

typedef struct thpool_t threadpool_t;

threadpool_t *thpool_init(int num_threads);
int           thpool_add_work(threadpool_t *, void (*funcptr)(int, void *), void *args);
void          thpool_wait(threadpool_t *);
void          thpool_pause(threadpool_t *);
void          thpool_resume(threadpool_t *);
void          thpool_destroy(threadpool_t *);
int           thpool_num_threads_working(threadpool_t *);

#endif /* THPOOL_H */