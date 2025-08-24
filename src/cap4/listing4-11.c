#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Definición del trabajo */
struct job {
    struct job* next;
    const char* text;
};

/* Cola global de trabajos */
struct job* job_queue = NULL;

/* Mutex para proteger la cola */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Procesar un trabajo: contar caracteres */
void process_job(struct job* j) {
    int count = strlen(j->text);
    printf("Hilo %lu procesó: \"%s\" (%d caracteres)\n",
           pthread_self(), j->text, count);
}

/* Función del hilo trabajador */
void* thread_function(void* arg) {
    while (1) {
        struct job* next_job;

        /* Bloquear el acceso a la cola */
        pthread_mutex_lock(&job_queue_mutex);

        if (job_queue == NULL)
            next_job = NULL;
        else {
            /* Tomar el siguiente trabajo */
            next_job = job_queue;
            job_queue = job_queue->next;
        }

        /* Desbloquear la cola */
        pthread_mutex_unlock(&job_queue_mutex);

        if (next_job == NULL)
            break;

        /* Procesar el trabajo */
        process_job(next_job);

        /* Liberar memoria */
        free(next_job);
    }
    return NULL;
}

int main() {
    pthread_t threads[3];  // 3 trabajadores
    const char* texts[] = {
        "buen diaaaa",
        "profesoooor",
        "como esta?",
        "espero quee",
        "bieeeen"
    };
    int n = sizeof(texts) / sizeof(texts[0]);

    /* Crear los trabajos y agregarlos a la cola */
    for (int i = 0; i < n; i++) {
        struct job* new_job = malloc(sizeof(struct job));
        new_job->text = texts[i];

        pthread_mutex_lock(&job_queue_mutex);
        new_job->next = job_queue;
        job_queue = new_job;
        pthread_mutex_unlock(&job_queue_mutex);
    }

    /* Lanzar los hilos */
    for (int i = 0; i < 3; i++)
        pthread_create(&threads[i], NULL, thread_function, NULL);

    /* Esperar a que terminen */
    for (int i = 0; i < 3; i++)
        pthread_join(threads[i], NULL);

    return 0;
}
