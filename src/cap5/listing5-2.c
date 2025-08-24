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

/* Obtain a binary semaphore’s ID, allocating if necessary. */
int binary_semaphore_allocation (key_t key, int sem_flags)
{
    return semget (key, 1, sem_flags);
}

/* Deallocate a binary semaphore. All users must have finished their
   use. Returns -1 on failure. */
int binary_semaphore_deallocate (int semid)
{
    union semun ignored_argument;
    return semctl (semid, 0, IPC_RMID, ignored_argument); // ojo: índice 0
}

/* --- MAIN DE PRUEBA --- */
int main() {
    key_t key = 1234;  // clave arbitraria
    int semid;

    // Crear o abrir semáforo
    semid = binary_semaphore_allocation(key, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("Error creando semáforo");
        exit(1);
    }

    printf("Semáforo creado con ID = %d\n", semid);

    // Liberar semáforo
    if (binary_semaphore_deallocate(semid) == -1) {
        perror("Error eliminando semáforo");
        exit(1);
    }

    printf("Semáforo eliminado correctamente.\n");

    return 0;
}
