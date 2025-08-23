#include <pthread.h>
#include <iostream>
#include <chrono>
#include <thread>

/* Clase para simular salida del hilo mediante excepción */
class ThreadExitException
{
public:
    ThreadExitException(void* return_value)
        : thread_return_value_(return_value) {}

    void* DoThreadExit() {
        pthread_exit(thread_return_value_);
    }

private:
    void* thread_return_value_;
};

/* Función simulando un buffer RAII */
class TempBuffer {
public:
    TempBuffer(size_t size) {
        buffer = new char[size];
        std::cout << "Buffer asignado\n";
    }
    ~TempBuffer() {
        delete[] buffer;
        std::cout << "Buffer liberado automáticamente\n";
    }
private:
    char* buffer;
};

/* Función de trabajo del hilo */
bool should_exit_thread_immediately() {
    // Para el ejemplo, salimos tras un solo paso
    static bool first = true;
    if (first) { first = false; return false; }
    return true;
}

void do_some_work() {
    TempBuffer temp(1024); // buffer temporal RAII

    int step = 0;
    while (true) {
        std::cout << "Hilo trabajando, paso " << ++step << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (should_exit_thread_immediately())
            throw ThreadExitException(nullptr); // salir del hilo
    }
}

/* Función principal del hilo */
void* thread_function(void*) {
    try {
        do_some_work();
    }
    catch (ThreadExitException& ex) {
        ex.DoThreadExit();
    }
    return nullptr;
}

int main() {
    pthread_t hilo;

    pthread_create(&hilo, nullptr, thread_function, nullptr);

    pthread_join(hilo, nullptr);

    std::cout << "Hilo terminado correctamente.\n";

    return 0;
}
