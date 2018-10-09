/**
 * Lab 6 Extra Credit
 * 
 * Authors:
 * Anthony Blanton
 * Alison Scheffler
 * 
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int Create(key_t);
int Init(int);
void Wait(int);
void Signal(int);
int Destroy(int);