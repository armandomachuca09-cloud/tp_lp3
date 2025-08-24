#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/* Write TEXT to the socket given by file descriptor SOCKET_FD. */
void write_text(int socket_fd, const char* text)
{
    /* Write the number of bytes in the string, including NUL-termination. */
    int length = strlen(text) + 1;
    write(socket_fd, &length, sizeof(length));
    /* Write the string. */
    write(socket_fd, text, length);
}

int main(int argc, char* const argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <socket_path> <message>\n", argv[0]);
        return 1;
    }

    const char* const socket_name = argv[1];
    const char* const message = argv[2];
    int socket_fd;
    struct sockaddr_un name;

    /* Create the socket. */
    socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("socket");
        return 1;
    }

    /* Initialize and store the server’s name in the socket address. */
    memset(&name, 0, sizeof(name));
    name.sun_family = AF_LOCAL;
    strncpy(name.sun_path, socket_name, sizeof(name.sun_path) - 1);

    /* Connect the socket. */
    if (connect(socket_fd, (struct sockaddr*)&name, SUN_LEN(&name)) < 0) {
        perror("connect");
        close(socket_fd);
        return 1;
    }

    /* Write the text on the command line to the socket. */
    write_text(socket_fd, message);

    close(socket_fd);
    return 0;
}
