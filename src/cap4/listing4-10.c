#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Definición de un trabajo */
struct job {
    struct job* next;
    const char* text;
};

/* Cola global de trabajos */
struct job* job_queue = NULL;

/* Procesa un trabajo: cuenta caracteres del texto */
void process_job(struct job* j) {
    int count = strlen(j->text);
    printf("Texto: \"%s\" tiene %d caracteres\n", j->text, count);
}

/* Función del hilo que consume la cola */
void* thread_function(void* arg) {
    while (job_queue != NULL) {
        /* Tomar el siguiente trabajo */
        struct job* next_job = job_queue;
        job_queue = job_queue->next;

        /* Procesar y limpiar */
        process_job(next_job);
        free(next_job);
    }
    return NULL;
}

int main() {
    pthread_t worker;

    /* Crear algunos trabajos manualmente */
    const char* texts[] = {
        "Hola mundo",
        "Aguante LP3",
        "Buenas profesor Lima",
        "Aguante olimpia"
    };
    int n = sizeof(texts) / sizeof(texts[0]);

    for (int i = 0; i < n; i++) {
        struct job* new_job = malloc(sizeof(struct job));
        new_job->text = texts[i];
        new_job->next = job_queue;
        job_queue = new_job;
    }

    /* Lanzar un hilo que consuma la cola */
    pthread_create(&worker, NULL, thread_function, NULL);

    /* Esperar a que termine */
    pthread_join(worker, NULL);

    return 0;
}


