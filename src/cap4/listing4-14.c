#include <pthread.h>
#include <stdio.h>
#include <unistd.h> // para sleep

// Variables globales
int thread_flag;
pthread_cond_t thread_flag_cv;
pthread_mutex_t thread_flag_mutex;

// Función que hace trabajo
void do_work() {
    printf("Haciendo trabajo...\n");
    sleep(1); // simula trabajo
}

// Inicializa mutex, condition variable y bandera
void initialize_flag() {
    pthread_mutex_init(&thread_flag_mutex, NULL);
    pthread_cond_init(&thread_flag_cv, NULL);
    thread_flag = 0;
}

// Función del hilo
void* thread_function(void* thread_arg) {
    while (1) {
        pthread_mutex_lock(&thread_flag_mutex);

        while (!thread_flag)
            pthread_cond_wait(&thread_flag_cv, &thread_flag_mutex);

        pthread_mutex_unlock(&thread_flag_mutex);

        do_work();
    }
    return NULL;
}

// Cambia la bandera y despierta al hilo si está esperando
void set_thread_flag(int flag_value) {
    pthread_mutex_lock(&thread_flag_mutex);
    thread_flag = flag_value;
    pthread_cond_signal(&thread_flag_cv);
    pthread_mutex_unlock(&thread_flag_mutex);
}

// Main
int main() {
    pthread_t worker;

    // Inicializar mutex, condition variable y bandera
    initialize_flag();

    // Crear el hilo
    pthread_create(&worker, NULL, thread_function, NULL);

    // Activar la bandera para que el hilo haga trabajo
    printf("Activando trabajo del hilo...\n");
    set_thread_flag(1);
    sleep(3); // deja que el hilo haga trabajo unos segundos

    // Desactivar la bandera
    printf("Desactivando trabajo del hilo...\n");
    set_thread_flag(0);
    sleep(2); // espera para ver que el hilo no hace trabajo

    // Reactivar la bandera
    printf("Reactivando trabajo del hilo...\n");
    set_thread_flag(1);
    sleep(2);

    // Cancelar el hilo (porque está en bucle infinito)
    pthread_cancel(worker);
    pthread_join(worker, NULL);

    printf("Programa terminado.\n");
    return 0;
}
