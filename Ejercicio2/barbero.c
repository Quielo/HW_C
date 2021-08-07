#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

// numero maximo de clientes
#define MAX_CUSTOMERS 25

// prototipos
void *customer(void *num);
void *barber(void *);

void randwait(int secs);

// entran uno a la vez a las sillas
sem_t waitingRoom;

// limita el acceso a uno
sem_t barberChair;

// la silla donde duerme
sem_t barberPillow;

// para que el cliente espere hasta que el barbero este libre
sem_t seatBelt;

// todo listo
int allDone = 0;

int main(int argc, char *argv[])
{
    pthread_t btid;
    pthread_t tid[MAX_CUSTOMERS];
    long RandSeed;
    int i, numCustomers, numChairs;
    int Number[MAX_CUSTOMERS];

    printf("Ingresar numero de clientes: ");
    scanf("%d", &numCustomers);
    printf("Ingresar numero de sillas: ");
    scanf("%d", &numChairs);

    // numero de hilos tiene que ser menor a los clientes soportados
    if (numCustomers > MAX_CUSTOMERS)
    {
        printf("El numero maximo de clientes es % d.\n", MAX_CUSTOMERS);
        exit(-1);
    }

    // inicia el array
    for (i = 0; i < MAX_CUSTOMERS; i++)
    {
        Number[i] = i;
    }

    // inicia los semaforos
    sem_init(&waitingRoom, 0, numChairs);
    sem_init(&barberChair, 0, 1);
    sem_init(&barberPillow, 0, 0);
    sem_init(&seatBelt, 0, 0);

    // crea el barbero
    pthread_create(&btid, NULL, barber, NULL);

    // Create the customers.
    for (i = 0; i < numCustomers; i++)
    {
        pthread_create(&tid[i], NULL, customer, (void *)&Number[i]);
        sleep(1);
    }

    // hacer que los hilos esperen
    for (i = 0; i < numCustomers; i++)
    {
        pthread_join(tid[i], NULL);
        sleep(1);
    }

    // cuando todos los clientes estan listos, terminar al barbero
    allDone = 1;
    sem_post(&barberPillow);
    pthread_join(btid, NULL);
}

void *customer(void *number)
{
    int num = *(int *)number;

    // ir a la barberia en random
printf("Cliente %d saliendo a la barberia.\n", num);
randwait(2);
printf("Cliente %d llegando a la barberia.\n", num);

// esperar por espacio en la sala de espera
sem_wait(&waitingRoom);
printf("Cliente %d entrando a la sala de espera.\n", num);

// esperar para que el barbero quede libre
sem_wait(&barberChair);

// la silla esta libre
sem_post(&waitingRoom);

// despertar al barbero
printf("Cliente %d despierta al barbero.\n", num);
sem_post(&barberPillow);

// esperar por el barbero
sem_wait(&seatBelt);

// dar la silla
sem_post(&barberChair);
printf("Cliente %d se va de la tienda.\n", num);
}

void *barber(void *junk)
{
    // saber cuantos quedan esperando
    while (!allDone)
    {

        // Sdormir hasta que lo despierten
        printf("El barbero esta durmiendo\n");
        sem_wait(&barberPillow);

        // todo listo
        if (!allDone)
        {

            // tiempo random para corte
            printf("El barbero esta cortando el pelo.\n");
            randwait(2);
            printf("El barbero termino de cortar el cabello.\n");

            // liberar cliente
            sem_post(&seatBelt);
        }
        else
        {
printf("El barbero se va a casa por el dia de hoy.\n");
        }
    }
}

void randwait(int secs)
{
    int len;

    // Generar numero random
    len = (int)((1 * secs) + 1);
    sleep(len);
}