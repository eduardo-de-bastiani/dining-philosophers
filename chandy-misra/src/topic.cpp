#include <pthread.h>
#include "topic.h"

namespace pd {
    Topic::Topic(pthread_cond_t cv, pthread_mutex_t mutex) : cv(cv), mutex(mutex){}

    void Topic::wait() {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cv, &mutex);
        pthread_mutex_unlock(&mutex);
    }

    void Topic::broadcast() {
        pthread_mutex_lock(&mutex);
        pthread_cond_broadcast(&cv);
        pthread_mutex_unlock(&mutex);
    }
}