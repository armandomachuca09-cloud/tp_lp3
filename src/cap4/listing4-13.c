#include <pthread.h>
#include <stdio.h>

int thread_flag;
pthread_mutex_t thread_flag_mutex;
void do_work(){
  printf("Haciendo Trabajo");
}
void initialize_flag ()
{
  pthread_mutex_init (&thread_flag_mutex, NULL);
  thread_flag = 0;
}
/* Calls do_work repeatedly while the thread flag is set; otherwise
   spins. */
void* thread_function (void* thread_arg)
{
  while (1) {
    int flag_is_set;
    /* Protect the flag with a mutex lock. */
    pthread_mutex_lock (&thread_flag_mutex);
    flag_is_set = thread_flag;
    pthread_mutex_unlock (&thread_flag_mutex);
    if (flag_is_set)
      do_work ();
    /* Else don’t do anything. Just loop again. */
  }
  return NULL;
}
/* Sets the value of the thread flag to FLAG_VALUE. */
void set_thread_flag (int flag_value)
{
  /* Protect the flag with a mutex lock. */
  pthread_mutex_lock (&thread_flag_mutex);
  thread_flag = flag_value;
  pthread_mutex_unlock (&thread_flag_mutex);
}
int main() {
    pthread_t worker;

    // Inicializar la bandera
    initialize_flag();

    // Crear el hilo
    pthread_create(&worker, NULL, thread_function, NULL);

    // Activar la bandera para que el hilo haga trabajo
    printf("Activando trabajo del hilo...\n");
    set_thread_flag(1);
 

    // Desactivar la bandera
    printf("Desactivando trabajo del hilo...\n");
    set_thread_flag(0);
  

    // Terminar el hilo (en este ejemplo, el hilo está en bucle infinito,
    // así que se cancelará)
    pthread_cancel(worker);
    pthread_join(worker, NULL);

    printf("Programa terminado.\n");
    return 0;
}
