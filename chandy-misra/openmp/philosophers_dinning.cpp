/**********************************
 * Dining Philosophers Problem using OpenMP
 *
 * DESCRIPTION: A program to solve Dining Philosophers Problem
 * in OpenMP
 *
 * Author: Kejie Zhang
 * LAST UPDATED: 02/16/2019
 *
 * USEFUL REFERENCE:
 *    -> OpenMP: https://computing.llnl.gov/tutorials/openMP/
 *    -> Dining Philosophers Problem: https://en.wikipedia.org/wiki/Dining_philosophers_problem
**********************************/

#include <iostream>
#include <random>
#include <ctime>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <math.h>
#include <omp.h>

int num, dinner_time;
omp_lock_t *forks;
int *dinned_times;
bool *philosophers;

using namespace std;

/** Generate random number */
double randomNum(double m) {
    /** Generate random num */
    random_device rd;
    default_random_engine e(rd());
    uniform_int_distribution<> range(m / 2.0, m * 2.0);
    return range(e);
}

/**
 * Dine worker function.
 * Create threads and start work.
 */
void dine(int philosopher_id) {
    int left, right;
    left = philosopher_id;
    if (philosopher_id == num - 1) {
        right = 0;
    } else {
        right = left + 1;
    }

    while (dinned_times[philosopher_id] < dinner_time) {
        /** Thinking... */
        cout << "Philosopher " << philosopher_id << " is thinking\n";
        usleep((unsigned int) randomNum(500000));

        /** Requesting resources... */
        omp_set_lock(&forks[left]);
        if (omp_test_lock(&forks[right]) != 0) {
            omp_unset_lock(&forks[left]);
        } else {
            philosophers[philosopher_id] = 1;
            dinned_times[philosopher_id] += 1;

            cout << "Philosopher " << philosopher_id << " starts eating\n";

            /** Eating... */
            usleep((unsigned int) randomNum(500000));

            /** Release the resource*/
            cout << "Philosopher " << philosopher_id << " stops eating\n";
            omp_unset_lock(&forks[left]);
            omp_unset_lock(&forks[right]);
            philosophers[philosopher_id] = 0;
        }
    }
    cout << "Philosopher " << omp_get_thread_num() << " finished dinning..." << endl;
}

/** Main Function */
int main(int argc, char **argv) {
    /** Parse the command line parameters */
    int i;
    num = 5;
    dinner_time = 10;
    if (argc == 2) {
        num = atoi(argv[1]);
    }

    /** Initialization */
    forks = new omp_lock_t[num];
    dinned_times = new int[num];
    philosophers = new bool[num];
    omp_set_num_threads(num);
    for (i = 0; i < num; i++) {
        omp_init_lock(&forks[i]);
        philosophers[i] = 0;
        dinned_times[i] = 0;
    }

    /** Dining...*/
    cout << "Start the Dinner!!" << endl;
#pragma omp parallel for
    for (int k = 0; k < num; k++) {
        cout << "Philosopher " << k << " begins dinning..." << endl;
        dine(k);
    }

    /** End the dining */
    cout << "End the Dinner!!" << endl;

    /** Release resources */
    delete philosophers;
    delete forks;
    delete dinned_times;

    return 0;
}