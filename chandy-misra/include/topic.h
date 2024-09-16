#ifndef PHILOSOPHERS_DINNING_TOPIC_H
#define PHILOSOPHERS_DINNING_TOPIC_H

#include <pthread.h>

namespace pd {
    class Topic {
    public:
        pthread_cond_t cv;
        pthread_mutex_t mutex;

        Topic(pthread_cond_t cv, pthread_mutex_t mutex);
        void wait(void);
        void broadcast(void);
    };
}
#endif //PHILOSOPHERS_DINNING_TOPIC_H
