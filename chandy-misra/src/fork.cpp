#include <pthread.h>

#include "fork.h"

namespace pd {
    Fork::Fork(
            int id,
            int owner_id,
            pthread_mutex_t mutex,
            Topic &topic) : id(id), owner_id(owner_id), isDirty(true), mutex(mutex), topic(topic) {}

    void Fork::request(int const proposer){
        while(owner_id != proposer) {
            if(isDirty) {
                pthread_mutex_lock(&mutex);
                isDirty = false;
                owner_id = proposer;
                pthread_mutex_unlock(&mutex);
            } else {
                topic.wait();
            }
        }
    }

    void Fork::clean() {
        isDirty = true;
        topic.broadcast();
    }

}