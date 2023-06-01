/**
 * W zadaniu mamy pięć wątków: producenta i konsumenta.
 * Każdy producent może wyprodukować zadaną z góry liczbę wartości liczby typu int, tj. 4
 * Każdy wątek producenta produkuje wartości i przechowuje je we współdzielonym buforze o długości 3, 
 * podczas gdy wątek konsumenta pobiera wartości z bufora.
 * Pierwszy zapis do bufora  należy zrealizować dla indeksu 2.
 * Alokacja pamięci bufora powinna być zrealizowana w oparciu o calloc/malloc.
 * 
 * Używamy muteksu i dwóch semaforów (full, empty) do synchronizacji dostępu do buforów. 
 * Muteks służy do zarządzania dostępem do sekcji krytycznej. 
 * Semafory umożliwiają wątkom sygnalizować sobie nawzajem, czy można wykonać operacje zapisu/odczytu.
 * 
 * Każdy wątek producenta generuje liczbą losowa, a następnie wykonuje operację na semaforze.  
 * Jeśli zapis jest możliwy to zajmuje muteks, wstawia wcześniej wygenerowaną liczbę do bufora o indeksie (na pozycji) in
 * wyświetla komunikat na ekranie 
 * (który producent wykonał operację na buforze, zawartość bufora o indeksie (na pozycji) in, wartość indeksu in)
 * inkrementuje wartość indeksu in (operacja modulo), zwalnia mutex
 * a następnie wykonuje operacje na semaforze.
 * 
 * Każdy wątek konsumenta wykonuje operacje na semaforze.
 * Jeśli odczyt jest możliwy to zajmuje muteks
 * wczytuje zawartość bufora o indeksie (z pozycji) out
 * wyświetla komunikat na ekranie
 * (który spośród trzech konsumentów wykonał operację na buforze, pobrana zawartość bufora, wartość indeksu out)
 * modyfikuje wartość indeksu out
 * Zwalnia muteks, a następnie wykonuje operacje na semaforze.
 * 
 * Funkcja main tworzy wątki producenta i konsumenta
*/


#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>



#define MaxItems 4
#define BufferSize 3

sem_t empty;						// sem.
sem_t full;							// sem.

pthread_mutex_t mutex;              // mutex uzupelnic

int in = 2;                         // pierwszy zapis na 2 miejscu
int out = 2;
int *buffer;


void *producer(void *arg)
{   
    int item;
    for(int i = 0; i < MaxItems; i++) {
        item = rand();
        sem_wait(&empty);	// oczekiwanie na wolne miejsce w buforze
        pthread_mutex_lock(&mutex);	// blokowanie mutexu

        buffer[in] = item;
        printf("Producer %d: Insert Item %d at %d\n", *((int*)arg), buffer[in], in);
        in = (in + 1) % BufferSize;	// inkrementacja indeksu zapisu

        pthread_mutex_unlock(&mutex);	// odblokowanie mutexu
        sem_post(&full);	        // zwiększenie liczby zajętych miejsc w buforze
    }
    return NULL;
}
void *consumer(void *arg)
{   
    for(int i = 0; i < MaxItems; i++) {
        sem_wait(&full);	        // oczekiwanie na zajęte miejsce w buforze
        pthread_mutex_lock(&mutex);	    // blokowanie mutexu

        int item = buffer[out];
        printf("Consumer %d: Remove Item %d from %d\n", *((int*)arg), item, out);
        out = (out + 1) % BufferSize;	// inkrementacja indeksu odczytu

        pthread_mutex_unlock(&mutex);	// odblokowanie mutexu
        sem_post(&empty);	        // zwiększenie liczby wolnych miejsc w buforze
    }
    return NULL;
}

int main()
{
    pthread_t pro[5],con[5];
    int producers[5] = {1, 2, 3, 4};	// etykiety (numery) producentów i konsumentów
    buffer = (int*)calloc(BufferSize, sizeof(int));

    sem_init(&empty, 0, BufferSize);	// inicjalizacja empty
    sem_init(&full, 0, 0);	// inicjalizacja full
    pthread_mutex_init(&mutex, NULL);	// inicjalizacja mutexu


    for(int i = 0; i < 5; i++) {
        pthread_create(&pro[i], NULL, producer, &producers[i]);	// utworzenie wątków producenta
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&con[i], NULL, consumer, &producers[i]);	// utworzenie wątków konsumenta
    }

    for(int i = 0; i < 5; i++) {
        pthread_join(pro[i], NULL);	// oczekiwanie na zakończenie wątków producenta
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(con[i], NULL);	// oczekiwanie na zakończenie wątków konsumenta
    }

    sem_destroy(&empty);	    // zwolnienie semafora
    sem_destroy(&full);		// zwolnienie semafora
    pthread_mutex_destroy(&mutex);	// zwolnienie mutexu

    free(buffer);               // zwolnienie pamieci

    return 0;
}