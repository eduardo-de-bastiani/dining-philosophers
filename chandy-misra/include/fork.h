#ifndef PHILOSOPHERS_DINNING_FORK_H
#define PHILOSOPHERS_DINNING_FORK_H

#include <pthread.h>
#include "topic.h"

namespace pd {
    class Fork {
    public:
        int id;
        int owner_id;
        bool isDirty;
        pthread_mutex_t mutex;
        Topic topic;

        Fork(int id, int owner_id, pthread_mutex_t mutex, Topic& topic);
        void request(int proposer);
        void clean();
    };
}
#endif //PHILOSOPHERS_DINNING_FORK_H
