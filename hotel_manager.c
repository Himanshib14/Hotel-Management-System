#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUF_SIZE 100
#define EARNINGS_FILE "earning.txt"
#define HOTEL_KEY 38748
#define SHARED_MEMORY_SIZE 512
#define MANAGER_KEY 3674

struct shared_data_hotel_manager
{
    int termination_of_table;
};

struct shared_data_hm_waiter
{
    int total_bill_amt;
};

void write_earnings(int table_number, int total_earnings)
{
    FILE *fp = fopen(EARNINGS_FILE, "a");
    if (fp == NULL)
    {
        perror("Error opening earnings file");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "Earning from Table %d: %d INR\n", table_number, total_earnings);
    fclose(fp);
}

void calculate_and_display_totals(int total_earnings)
{
    FILE *fp = fopen(EARNINGS_FILE, "a");
    if (fp == NULL)
    {
        perror("Error opening earnings file");
        exit(EXIT_FAILURE);
    }

    float total_wage = total_earnings * 0.4;
    float total_profit = total_earnings * 0.6;

    printf("Total Earnings of Hotel: %d INR\n", total_earnings);
    fprintf(fp, "Total Earnings of Hotel: %d INR\n", total_earnings);

    printf("Total Wage of Waiters: %.2f INR\n", total_earnings * 0.4);
    fprintf(fp, "Total Wage of Waiters: %.2f INR\n", total_wage);

    printf("Total Profit: %.2f INR\n", total_profit);
    fprintf(fp, "Total Profit: %.2f INR\n", total_earnings * 0.6);

    fclose(fp);
}

int main()
{
    int table_count = 0;
    printf("Enter the Total Number of Tables at the Hotel: ");
    scanf("%d", &table_count);

    printf("hotel manager is reading...\n");
    int n = 10; // n is no. of elements (number of integers)
    int total_earnings = 0;

    int table_count_flag = table_count;
    int earnings[11];

    for (int i = 1; i <= 10; i++)
    {
        earnings[i] = -1;
    }

    while (table_count_flag != 0)
    {
        int waiterID = 0;
        for (int i = 1; i <= 10; i++)
        {

            waiterID = waiterID + 1;

            int shmid = shmget(HOTEL_KEY + waiterID, sizeof(struct shared_data_hotel_manager), 0666);
            //  if (shmid == -1)
            //{
            //    printf("error 1 \n");
            //  continue;
            // }

            int shmid2 = shmget(MANAGER_KEY + waiterID, SHARED_MEMORY_SIZE, 0666);
            if (shmid2 == -1)
            {
                continue;
            }

            // printf("2\n");

            // shared_data_hotel_manager *shared_data_hotel_manager = shmat(shmid, NULL, 0);
            // if (shared_data_hotel_manager == (struct shared_data_hotel_manager *)(-1))
            // {
            //     perror("shmat failed");
            //     exit(EXIT_FAILURE);
            // }

            // printf("3\n");

            struct shared_data_hm_waiter *shared_data_hm_waiter = shmat(shmid2, NULL, 0);
            if (shared_data_hm_waiter == (struct shared_data_hm_waiter *)(-1))
            {
                perror("shmat failed");
                exit(EXIT_FAILURE);
            }

            // printf("4\n");
            if (earnings[i] == -1)
            {
                table_count_flag--;

                // printf("5\n");

                earnings[i] = shared_data_hm_waiter->total_bill_amt;
                total_earnings += earnings[i];
                // write_earnings(i, earnings[i]);
            }
            else
            {
                earnings[i] += shared_data_hm_waiter->total_bill_amt;
            }

            if (shmdt(shared_data_hm_waiter) == -1)
            {
                continue;
            }

            // if (shmdt(shared_data_hotel_manager) == -1)
            // {
            //     continue;
            // }
        }
    }

    for (int i = 1; i <= 10; i++)
    {
        if (earnings[i] != -1)
        {
            write_earnings(i, earnings[i]);
        }
        // Need to delete shared memory
    }

    calculate_and_display_totals(total_earnings);
    key_t key;
    if ((key = ftok("table.c", 'A')) == -1)
    {
        perror("Error in ftok\n");
        return 1;
    }

    int shmid3;   // shared memory segment identifier
    int *shmPtr2; // Pointer to the shared memory segment
    shmid3 = shmget(key, BUF_SIZE, 0644 | IPC_CREAT);
    if (shmid3 == -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    shmPtr2 = shmat(shmid3, NULL, 0);
    printf("Waiting for confirmation from admin\n");
    while (1)
    {
        if (shmPtr2[0] == 1)
        {
            printf("Thank you for visiting the Hotel!\n");

            if (shmdt(shmPtr2) == -1)
            {
                perror("Error in shmdt in detaching the memory segment\n");
                return 1;
            }
            if (shmctl(shmid3, IPC_RMID, 0) == -1) // don't delete before reading is done
            {
                perror("Error in shmctl\n");
                return 1;
            }

            break;
        }
        sleep(1);
    }
    return 0;
}