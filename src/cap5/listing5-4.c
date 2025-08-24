#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
/* Wait on a binary semaphore. Block until the semaphore value is positive, then
   decrement it by 1. */
int binary_semaphore_wait(int semid)
{
  struct sembuf operations[1];
  /* Use the first (and only) semaphore. */
  operations[0].sem_num = 0;
  /* Decrement by 1. */
  operations[0].sem_op = -1;
  /* Permit undo’ing. */
  operations[0].sem_flg = SEM_UNDO;
  return semop (semid, operations, 1);
}
/* Post to a binary semaphore: increment its value by 1.
This returns immediately. */
int binary_semaphore_post(int semid)
{
    struct sembuf operations[1];
    operations[0].sem_num = 0;
    operations[0].sem_op = 1;
    operations[0].sem_flg = SEM_UNDO;

    return semop(semid, operations, 1);
}

int main() {
    key_t key = 1234;
    int semid;
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
    } arg;

    // Crear semáforo (1 semáforo en el set)
    semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("Error creando semáforo");
        exit(1);
    }

    // Inicializar semáforo en 1
    unsigned short values[1] = {1};
    arg.array = values;
    if (semctl(semid, 0, SETALL, arg) == -1) {
        perror("Error inicializando semáforo");
        exit(1);
    }
    printf("Semáforo creado e inicializado.\n");

    // Simular sección crítica
    printf("Esperando semáforo...\n");
    binary_semaphore_wait(semid);
    printf("Entré a la sección crítica.\n");
    sleep(2); // simula trabajo
    printf("Saliendo de la sección crítica.\n");
    binary_semaphore_post(semid);

    // Eliminar semáforo
    if (semctl(semid, 0, IPC_RMID, arg) == -1) {
        perror("Error eliminando semáforo");
        exit(1);
    }
    printf("Semáforo eliminado.\n");

    return 0;
}
