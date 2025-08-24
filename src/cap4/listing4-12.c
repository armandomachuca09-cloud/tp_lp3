#include <malloc.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // para sleep

struct job {
  /* Link field for linked list. */
  struct job* next;
  /* Un dato sencillo: un número. */
  int value;
};

/* A linked list of pending jobs. */
struct job* job_queue;

/* A mutex protecting job_queue. */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

/* A semaphore counting the number of jobs in the queue. */
sem_t job_queue_count;

/* ----------- FUNCIONES DE LA COLA ----------- */

/* Procesa un trabajo (ejemplo sencillo). */
void process_job(struct job* j) {
    printf("Procesando trabajo con valor = %d\n", j->value);
    sleep(1); // simula algo de tiempo de trabajo
}

/* Inicializa la cola de trabajos. */
void initialize_job_queue ()
{
  job_queue = NULL;
  sem_init (&job_queue_count, 0, 0);
}

/* Hilo que procesa trabajos. */
void* thread_function (void* arg)
{
  while (1) {
    struct job* next_job;

    /* Espera a que haya un trabajo disponible. */
    sem_wait (&job_queue_count);

    /* Bloquea la cola para sacar un trabajo. */
    pthread_mutex_lock (&job_queue_mutex);

    next_job = job_queue;
    job_queue = job_queue->next;

    pthread_mutex_unlock (&job_queue_mutex);

    /* Procesa y libera memoria. */
    process_job (next_job);
    free (next_job);
  }
  return NULL;
}

/* Agrega un nuevo trabajo a la cola. */
void enqueue_job (int value)
{
  struct job* new_job;
  new_job = (struct job*) malloc (sizeof (struct job));
  new_job->value = value;

  pthread_mutex_lock (&job_queue_mutex);
  new_job->next = job_queue;
  job_queue = new_job;
  pthread_mutex_unlock (&job_queue_mutex);

  sem_post (&job_queue_count);
}

/* ----------- MAIN DE PRUEBA ----------- */

int main() {
    pthread_t worker;

    /* Inicializar la cola y el semáforo. */
    initialize_job_queue();

    /* Crear un hilo que procese trabajos. */
    pthread_create(&worker, NULL, thread_function, NULL);

    /* Encolar algunos trabajos. */
    for (int i = 1; i <= 5; i++) {
        printf("Encolando trabajo %d\n", i);
        enqueue_job(i);
        sleep(1); // simula llegada gradual de trabajos
    }

    /* Esperar a que el hilo termine (en este ejemplo nunca lo hace). */
    pthread_join(worker, NULL);

    return 0;
}
