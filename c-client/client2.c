#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define DEFAULT_HOST "koth_c_srv2"
#define PORT         5005
#define BUFSIZE      1024
#define TAG          "[C-Client→Faiz]"

int main(void) {
    char *host = getenv("SERVER_HOST");
    if (!host) host = DEFAULT_HOST;

    printf(TAG " Starting...\n");
    fflush(stdout);

    while (1) {
        struct addrinfo hints, *res;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family   = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        char port_str[8];
        snprintf(port_str, sizeof(port_str), "%d", PORT);

        if (getaddrinfo(host, port_str, &hints, &res) != 0) {
            fprintf(stderr, TAG " DNS lookup failed for %s\n", host);
            sleep(5);
            continue;
        }

        int sock = socket(res->ai_family, res->ai_socktype, 0);
        if (sock < 0) { freeaddrinfo(res); sleep(5); continue; }

        if (connect(sock, res->ai_addr, res->ai_addrlen) < 0) {
            perror(TAG " connect");
            close(sock);
            freeaddrinfo(res);
            sleep(5);
            continue;
        }
        freeaddrinfo(res);

        char buf[BUFSIZE];
        memset(buf, 0, BUFSIZE);
        int n = recv(sock, buf, BUFSIZE - 1, 0);
        if (n > 0) {
            printf(TAG " %s\n", buf);
            fflush(stdout);
        }
        close(sock);
        sleep(10);
    }
    return 0;
}
