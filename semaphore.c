/***************************
 * Lab 6 - Semaphores
 * CIS 452-10
 * 10/11/18
 *
 * Authors:
 * Ali Scheffler
 * Anthony Blanton
 * ************************/
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

#define SIZE 16

int main (int arc, char *argv[]) 
{ 
   int status; 
   long int i, loop, temp, *shmPtr; 
   int shmId; 
   pid_t pid;

      // get value of loop variable (from command-line argument)
      loop = atoi(argv[1]);      

   if ((shmId = shmget (IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
      perror ("i can't get no..\n"); 
      exit (1); 
   } 
   if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) { 
      perror ("can't attach\n"); 
      exit (1); 
   }

   //Create a new semaphore and initialize semaphore
   int semId = semget(IPC_PRIVATE, 1, 00600);
   semctl(semId, 0, SETVAL, 1);

   //Create structure
   struct sembuf sbuf;

   int result;
   shmPtr[0] = 0; 
   shmPtr[1] = 1;

   //Setup 
   sbuf.sem_num = 0;
   sbuf.sem_flg = 0;

   if (!(pid = fork())) { 
      for (i=0; i<loop; i++) {
	        //lock
	 	sbuf.sem_op = -1;   
                result += semop(semId, &sbuf, 1); 
	        
		// swap the contents of shmPtr[0] and shmPtr[1]
		temp = shmPtr[0];
		shmPtr[0] = shmPtr[1];
		shmPtr[1] = temp;

		//Unlock
		sbuf.sem_op = 1;
		result += semop(semId, &sbuf, 1);
      } 
      if (shmdt (shmPtr) < 0) { 
         perror ("just can't let go\n"); 
         exit (1); 
      } 
      exit(0); 
   } 
   else 
      for (i=0; i<loop; i++) { 
	        //lock
	      	sbuf.sem_op = -1;
		result += semop(semId, &sbuf, 1);
                
		// swap the contents of shmPtr[1] and shmPtr[0] 
      		temp = shmPtr[0];
		shmPtr[0] = shmPtr[1];
		shmPtr[1] = temp;

		//unlock
		sbuf.sem_op = 1;
		result += semop(semId, &sbuf, 1);
      }

   if(result < 0)
   {
	   printf("Unsuccesful\n");
   }

   wait (&status); 
   printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

   //Remove the semaphore
   semctl(semId, 0, IPC_RMID);

   if (shmdt (shmPtr) < 0) { 
      perror ("just can't let go\n"); 
      exit (1); 
   } 
   if (shmctl (shmId, IPC_RMID, 0) < 0) { 
      perror ("can't deallocate\n"); 
      exit(1); 
   }

   return 0; 
} 
