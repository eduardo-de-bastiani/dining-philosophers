#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <zconf.h>
#include <random>
#include <ctime>
#include <unistd.h>
#include "philosopher.h"

namespace pd {

    Philosopher::Philosopher(
            int id,
            Fork& left_fork,
            Fork& right_fork,
            pthread_mutex_t& global_lock_log
            ) : id(id), left_fork(left_fork), right_fork(right_fork), global_lock_log(global_lock_log)
            {
            }

    void Philosopher::think() {
        log(" is thinking ");
        usleep((unsigned int)randomNum(1000000));
    }

    void Philosopher::eat() {
        left_fork.request(id);
        right_fork.request(id);

        log(" starts eating.");
        usleep((unsigned int)randomNum(1000000));
        log(" ends eating.");

        left_fork.clean();
        right_fork.clean();
    }

    void Philosopher::log(std::string const &message) {
        pthread_mutex_lock(&global_lock_log);
        std::cout
            << std::left
            << "Philosopher " << id << message
            << std::endl;
        pthread_mutex_unlock(&global_lock_log);
    }

    /** Generate random number */
    double Philosopher::randomNum(double m)
    {
        //Generate random num
        std::random_device rd;
        std::default_random_engine e(rd());
        std::uniform_int_distribution<> range(m / 2.0, m * 2.0);
        return range(e);
    }
}
