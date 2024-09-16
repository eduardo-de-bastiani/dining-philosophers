#ifndef PHILOSOPHERS_DINNING_PHILOSOPHER_H
#define PHILOSOPHERS_DINNING_PHILOSOPHER_H

#include <pthread.h>
#include <atomic>
#include <string>
#include "fork.h"

namespace pd {
    class Philosopher {
        public:
            int id;
            std::atomic_bool isSit {true};
            Fork& left_fork;
            Fork& right_fork;
            pthread_mutex_t global_lock_log;

            Philosopher(
                    int id,
                    Fork& left_fork,
                    Fork& right_fork,
                    pthread_mutex_t& global_lock_log);
            void think();
            void eat();
            void log(std::string const &message);
            double randomNum(double m);
    };
}
#endif //PHILOSOPHERS_DINNING_PHILOSOPHER_H
