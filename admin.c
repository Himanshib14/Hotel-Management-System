#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>

#define BUFFER_SIZE 100
#define MIN_WAITER_ID 1
#define MAX_WAITER_ID 10
#define MENU_SIZE 10
#define MAX_ORDER_ITEMS 5
#define SHARED_MEMORY_SIZE 512
#define MAX_CUST 5
#define MANAGER_KEY 4321
#define SHM_key 1000

int main()
{

    key_t key;
    if ((key = ftok("table.c", 'A')) == -1)
    {
        perror("Error in ftok\n");
        return 1;
    }
    int shmid;   // shared memory segment identifier
    int *shmPtr; // Pointer to the shared memory segment
    shmid = shmget(key, BUFFER_SIZE, 0644 | IPC_CREAT);

    if (shmid == -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    shmPtr = shmat(shmid, NULL, 0);
    int close_hotel = 0;
    char closing_signal;

    /*
    struct close *close = (struct close *)shmat(shmid, NULL, 0);

    if ((intptr_t)close == -1)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    close->close_hotel = 0;
    char closing_signal;
    */

    while (1)
    {
        /* code */
        printf("Do you want to close the hotel? Enter Y for Yes and N for No.");
        scanf(" %c", &closing_signal);

        if (closing_signal != 'N' && closing_signal != 'Y')
        {
            printf("Invalid input. Please enter Y or N.\n");
        }
        else if (closing_signal == 'Y')
        {
            // Inform hotel manager to close the hotel
            shmPtr[0] = 1;
            if (shmdt(shmPtr) == -1)
            {
                perror("Error in shmdt in detaching the memory segment\n");
                return 1;
            }
            break;
        }
        else
        {
            continue;
        }

        sleep(10);
    }
}