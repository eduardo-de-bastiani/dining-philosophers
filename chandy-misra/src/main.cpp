/**********************************
 * Dining Philosophers Problem in Chandy/Misra algorithm.
 *
 * DESCRIPTION: A program to solve Dining Philosophers Problem
 * in Chandy/Misra algorithm by using pthreads.
 *
 * Author: Kejie Zhang
 * LAST UPDATED: 02/04/2019
 *
 * USEFUL REFERENCE:
 *    -> Pthreads: https://computing.llnl.gov/tutorials/pthreads/
 *    -> Dining Philosophers Problem: https://en.wikipedia.org/wiki/Dining_philosophers_problem
 **********************************/
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <vector>
#include <zconf.h>
#include <random>

#include "philosopher.h"
#include "fork.h"
#include "topic.h"
#include "cxxopts.hpp"

/** Define global variables */
#define DINING_TIME 10

pthread_mutex_t global_lock_log;

struct thread_data
{
    pd::Philosopher *philosopher;
};

/** Parse the command line parameters */
cxxopts::ParseResult
parse(int argc, char *argv[])
{
    try
    {
        cxxopts::Options options(argv[0], "A CLI to visualize Philosophers Dinning Problem");
        options
            .positional_help("[optional args]")
            .show_positional_help();

        options
            .add_options()("n, num", "number of philosophers", cxxopts::value<int>())("help", "Print help")
#ifdef CXXOPTS_USE_UNICODE
#endif
            ;

        auto result = options.parse(argc, argv);

        if (result.count("num") == 0)
        {
            std::cout << options.help({""}) << std::endl;
            exit(0);
        }

        if (result.count("help"))
        {
            std::cout << options.help({""}) << std::endl;
            exit(0);
        }

        return result;
    }
    catch (const cxxopts::OptionException &e)
    {
        std::cout << "error parsing options: " << e.what() << std::endl;
        exit(1);
    }
}

/** Dine worker function */
void *dine(void *threadarg)
{
    struct thread_data *local_data = (struct thread_data *)threadarg;
    pd::Philosopher *philosopher = local_data->philosopher;
    sleep(1);

    while (philosopher->isSit)
    {
        philosopher->think();
        philosopher->eat();
    }

    pthread_exit(NULL);
}

/** Main function */
int main(int argc, char *argv[])
{
    /*** Parse the command line parameters */

    auto result = parse(argc, argv);
    int num = 5;
    if (result.count("num"))
    {
        num = result["num"].as<int>();
    }
    pthread_mutex_t topic_mutex;
    pthread_cond_t cv;
    pthread_t threads[num];
    pthread_mutex_t mutex_forks[num];

    /** Initialize mutex and conditions variable */
    pthread_cond_init(&cv, NULL);
    pthread_mutex_init(&topic_mutex, NULL);
    pthread_mutex_init(&global_lock_log, NULL);
    for (int i = 0; i < num; i++)
    {
        pthread_mutex_init(&mutex_forks[i], NULL);
    }
    pd::Topic topic{
        cv,
        topic_mutex};

    /** Initialize philosophers and forks */
    std::vector<pd::Philosopher *> philosophers;
    std::vector<pd::Fork *> forks;

    std::cout << "Creating " << num << " philosophers........." << std::endl;
    for (int i = 0; i < num; i++)
    {
        forks.push_back(new pd::Fork(i + 1, i % num + 1, mutex_forks[i], topic));
    }
    for (int i = 0; i < num; i++)
    {
        philosophers.push_back(new pd::Philosopher(i + 1, *forks[i], *forks[(i + 1) % num], global_lock_log));
    }

    const std::chrono::time_point<std::chrono::steady_clock> start =
        std::chrono::steady_clock::now();
    /** Create threads and start work */
    std::cout << "Start the Dinner!!" << std::endl;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    struct thread_data thread_data_array[num];

    // create threads and start work
    for (int tid = 0; tid < num; tid++)
    {
        thread_data_array[tid].philosopher = philosophers[tid];

        int rc = pthread_create(&threads[tid], &attr, dine, (void *)&thread_data_array[tid]);

        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    /** Dining... */
    sleep(DINING_TIME);

    /** End the dining */
    for (int i = 0; i < num; i++)
    {
        philosophers[i]->isSit = false;
    }

    // wait for all threads to complete their work
    for (int tid = 0; tid < num; tid++)
    {
        std::cout << "Philosopher " << tid << " is done dining." << std::endl;
        int rc = pthread_join(threads[tid], NULL);
        if (rc)
        {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }
    const auto end = std::chrono::steady_clock::now();
    std::cout << "All philosophers are done dining." << std::endl;

    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

    /** Release resources */
    for (int i = 0; i < num; i++)
    {
        delete philosophers[i];
        delete forks[i];
    }
    pthread_mutex_destroy(&global_lock_log);
    pthread_mutex_destroy(&topic_mutex);
    pthread_exit(NULL);
}
