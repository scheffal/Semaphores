/**
 * Lab 6 Extra Credit
 * 
 * Authors:
 * Anthony Blanton
 * Alison Scheffler
 * 
 * To use:
 * $ gcc -c libsem.c
 * $ ar cr libsem.a libsem.o
 * $ gcc semaphore.c -Llib -lsem
 * 
 * Reference:
 * https://www.thegeekstuff.com/2010/08/ar-command-examples/
 */

#include "libsem.h"

/** Create semaphore */
int Create(key_t key) {
    return semget(key, 1, 0600);
}

/** initialize semaphore */
int Init(int semid) {
    return semctl(semid, 0, SETVAL, 1);
}

/** Wait */
void Wait(int semid) {

    // initialize the structure passed to semop that
    // decrements the value of the semaphore, telling others
    // to wait
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = -1;
    sops.sem_flg = 0;

    int val = semctl(semid, 0, GETVAL);
    // wait for the semaphore value to change
    while (val <= 0);
    // increment the value
    semop(semid, &sops, 1);
    // semctl(semid, 0, SETVAL, 0);
}

/** Signal */
void Signal(int semid) {
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = 1;
    sops.sem_flg = 0;

    semop(semid, &sops, 1);
    // semctl(semid, 0, SETVAL, 1);
}

/** Destroy semaphore */
int Destroy(int semid) {
    return semctl(semid, 0, IPC_RMID);
}