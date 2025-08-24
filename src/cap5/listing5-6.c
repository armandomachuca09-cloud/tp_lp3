#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define FILE_LENGTH 0x100

int main (int argc, char* const argv[])
{
    int fd;
    void* file_memory;
    int integer;

    /* Abrir el archivo. */
    fd = open(argv[1], O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    /* Crear el mapeo de memoria. */
    file_memory = mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (file_memory == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return 1;
    }

    close(fd);  // Ya no necesitamos el descriptor

    /* Leer el entero, imprimirlo y duplicarlo. */
    sscanf((char*)file_memory, "%d", &integer);  // CORRECCIÓN: usar sscanf en lugar de scanf
    printf("value: %d\n", integer);
    snprintf((char*)file_memory, FILE_LENGTH, "%d\n", 2 * integer);  // CORRECCIÓN: usar snprintf para evitar overflow

    /* Liberar la memoria (innecesario porque el programa termina). */
    munmap(file_memory, FILE_LENGTH);

    return 0;
}
