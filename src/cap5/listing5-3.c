#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

/* We must define union semun ourselves. */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

/* Allocate binary semaphore */
int binary_semaphore_allocation (key_t key, int sem_flags) {
    return semget(key, 1, sem_flags);
}

/* Deallocate binary semaphore */
int binary_semaphore_deallocate (int semid) {
    union semun ignored_argument;
    return semctl(semid, 0, IPC_RMID, ignored_argument);
}

/* Initialize a binary semaphore with a value of 1 */
int binary_semaphore_initialize (int semid) {
    union semun argument;
    unsigned short values[1];
    values[0] = 1;              // inicializar en 1 (semáforo libre)
    argument.array = values;
    return semctl(semid, 0, SETALL, argument);
}

int main() {
    key_t key = 1234;  
    int semid;

    // Crear semáforo
    semid = binary_semaphore_allocation(key, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("Error creando semáforo");
        exit(1);
    }
    printf("Semáforo creado con ID = %d\n", semid);

    // Inicializar semáforo
    if (binary_semaphore_initialize(semid) == -1) {
        perror("Error inicializando semáforo");
        exit(1);
    }
    printf("Semáforo inicializado en 1.\n");

    // Eliminar semáforo
    if (binary_semaphore_deallocate(semid) == -1) {
        perror("Error eliminando semáforo");
        exit(1);
    }
    printf("Semáforo eliminado correctamente.\n");

    return 0;
}
