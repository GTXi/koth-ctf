#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <mysql/mysql.h>

#define USER    "Faiz"
#define PORT    5005
#define BUFSIZE 1024
#define TAG     "[C-Srv:Faiz]"

static char *env_or(const char *key, char *fallback) {
    char *v = getenv(key);
    return v ? v : fallback;
}

static MYSQL *db_connect(void) {
    MYSQL *c = mysql_init(NULL);
    if (!c) return NULL;
    if (!mysql_real_connect(c,
            env_or("DB_HOST", "koth_db"),
            env_or("DB_USER", "koth_user"),
            env_or("DB_PASS", "koth_pass"),
            env_or("DB_NAME", "koth"),
            3306, NULL, 0)) {
        fprintf(stderr, TAG " DB error: %s\n", mysql_error(c));
        mysql_close(c);
        return NULL;
    }
    return c;
}

static void child_loop(void) {
    printf(TAG " [Child] Score updater started.\n");
    fflush(stdout);
    while (1) {
        MYSQL *c = db_connect();
        if (c) {
            if (mysql_query(c,
                    "UPDATE koth_standings "
                    "SET points = points + 1, datetime_stamp = NOW() "
                    "WHERE user = '" USER "'"))
                fprintf(stderr, TAG " [Child] Query error: %s\n", mysql_error(c));
            else {
                printf(TAG " [Child] Score updated.\n");
                fflush(stdout);
            }
            mysql_close(c);
        }
        sleep(30);
    }
}

static void serve_client(int fd) {
    char msg[BUFSIZE];
    MYSQL *c = db_connect();
    if (!c) {
        snprintf(msg, BUFSIZE, "Error: DB unavailable.");
        send(fd, msg, strlen(msg), 0);
        close(fd);
        return;
    }
    if (mysql_query(c,
            "SELECT user, points, datetime_stamp "
            "FROM koth_standings WHERE user = '" USER "'")) {
        snprintf(msg, BUFSIZE, "Query error: %s", mysql_error(c));
    } else {
        MYSQL_RES *res = mysql_store_result(c);
        MYSQL_ROW  row = mysql_fetch_row(res);
        if (row)
            snprintf(msg, BUFSIZE,
                "User: %s | Points: %s | Last Update: %s",
                row[0], row[1], row[2]);
        else
            snprintf(msg, BUFSIZE, "No data.");
        mysql_free_result(res);
    }
    mysql_close(c);
    send(fd, msg, strlen(msg), 0);
    close(fd);
}

int main(void) {
    signal(SIGCHLD, SIG_IGN);

    pid_t pid = fork();
    if (pid < 0)  { perror(TAG " fork"); exit(1); }
    if (pid == 0) { child_loop(); exit(0); }

    int srv = socket(AF_INET, SOCK_STREAM, 0);
    if (srv < 0) { perror("socket"); exit(1); }

    int opt = 1;
    setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(PORT);

    if (bind(srv, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        { perror("bind"); exit(1); }

    listen(srv, 10);
    printf(TAG " [Parent] Listening on port %d\n", PORT);
    fflush(stdout);

    socklen_t addrlen = sizeof(addr);
    while (1) {
        int cli = accept(srv, (struct sockaddr *)&addr, &addrlen);
        if (cli < 0) continue;

        pid_t cpid = fork();
        if (cpid == 0) {
            close(srv);
            serve_client(cli);
            exit(0);
        }
        close(cli);
    }
    return 0;
}
