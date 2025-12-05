#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <omp.h>

int main() {
    int meat = 5;
    int flights = 0;
    int need_food = 0;
    int first_chick = -1;
    int eagle_in_flight = 0;    
    int done = 0;
#pragma omp parallel num_threads(4) shared(meat, flights, need_food, first_chick, done, eagle_in_flight)
    {
        int tid = omp_get_thread_num();
        if (tid == 0) {
            printf("The eagle: there are already 5 pieces of meat in the nest\n");
            while (1) {
                int must_fly = 0;
#pragma omp critical
                {
                    if (flights == 3 && meat == 0 && !eagle_in_flight) {done = 1;}
                    else if (need_food && !eagle_in_flight && flights < 3) {
                        flights++;
                        must_fly = 1;
                        need_food = 0;
                        eagle_in_flight = 1;
                        printf("The eagle: flying for prey (fly № %d)\n", flights);
                    }
                }
                if (done) {
                    printf("The eagle: All the food is eaten, and I end job\n");
                    break;
                }
                if (!must_fly) {
                    usleep(100000);
                    continue;
                }
                sleep(3);
#pragma omp critical
                {
                    meat += 5; eagle_in_flight = 0;
                    printf("The Eagle says: Eaglets, eat!\n");
                }
            }
        } else {
            int id = tid;
            while (1) {
                int ate = 0;
#pragma omp critical
                {
                    if (flights == 3 && meat == 0 && !eagle_in_flight) {
                        done = 1;
                    }
                    if (!done) {
                        if (meat > 0 &&
                            (first_chick == -1 || first_chick == id || meat < 5)) {
                            meat--;
                            if (first_chick == id && meat == 4)
                                first_chick = -1;
                            printf("The %d eaglet take piece of meat, remain %d\n",
                                   id, meat);
                            ate = 1;
                        }
                        else if (meat == 0 && flights < 3 && !need_food && !eagle_in_flight) {
                            need_food = 1;
                            first_chick = id;
                            printf("The %d eaglet says: No meat! I'm crying!\n", id);
                        }
                    }
                }
                if (done) {
                    printf("The %d eaglet says: there is no food anymore, I end job\n", id);
                    break;
                }
                if (ate) {
                    printf("The %d eaglet is eating\n", id);
                    sleep(1);
                } else {
                    usleep(100000);
                }
            }
        }
    }
    return 0;
}