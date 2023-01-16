#include "../include/include.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int argc_cmd = 0;
char argv_cmd[BUFSIZ][BUFSIZ];

struct ftp_client
{
    int socket, socket_data;
    struct sockaddr_in servaddr, dataddr;
    int port_data;
    int open, ip;
} ftp_client;

char *prompt_shell(void)
{
    static char buf[512];

    snprintf(buf, sizeof(buf), "%s@ftp > ", getenv("USER"));
    fflush(stdout);

    return buf;
}

int takeInput(char *str)
{
    char *buf;
    HIST_ENTRY *last;
    buf = readline(prompt_shell());
    last = previous_history();
    if (strlen(buf) != '\0' && (!last || strcmp(last->line, buf)))
    {
        add_history(buf);
        strcpy(str, buf);
        return 0;
    }
    else
    {
        strcpy(str, buf);
        return 1;
    }
}

void parse_command(char command[BUFSIZ])
{
    if (!strlen(command))
    {
        return;
    }

    int idx = 0;
    for (int i = 0; i < (int)strlen(command); i++)
    {
        if (command[i] == ' ')
        {
            argc_cmd += 1;
            idx = 0;
        }
        else
        {
            argv_cmd[argc_cmd][idx] = command[i];
            idx += 1;
        }
    }
    argc_cmd++;
}

void print_args()
{
    printf("Here are the arguments:\n");
    for (int i = 0; i < argc_cmd; i++)
    {
        printf("%s\n", argv_cmd[i]);
    }
}

void reset_args()
{
    memset(argv_cmd, 0, BUFSIZ * BUFSIZ);
    argc_cmd = 0;
}

int code_return(char *buf, int size)
{
    CHK(recv(ftp_client.socket, buf, size, 0));
    char store = buf[3];
    buf[3] = '\0';
    int ret = atoi(buf);
    buf[3] = store;
    return ret;
}

int send_data_port(void)
{

    ftp_client.dataddr.sin_family = AF_INET;
    ftp_client.dataddr.sin_port = htons(DATA_PORT);
    ICHK(inet_aton("192.168.15.15", &ftp_client.dataddr.sin_addr));

    CHK((ftp_client.socket_data = socket(AF_INET, SOCK_STREAM, 0)));
    CHK(bind(ftp_client.socket_data, (struct sockaddr *)&ftp_client.dataddr, sizeof(ftp_client.dataddr)));
    CHK(listen(ftp_client.socket_data, 2));

    char buf[BUFSIZ];
    snprintf(buf, BUFSIZ, "PORT %d,%d,%d,%d,%d,%d\r\n", 192, 168, 15, 15, 214, 216);
    CHK(send(ftp_client.socket, buf, strlen(buf), 0));

    return ftp_client.socket_data;
}

void ftp_open(void)
{
    if (ftp_client.open)
        return;
    if (ftp_client.ip == 0)
        ICHK(inet_aton(argv_cmd[1], &ftp_client.servaddr.sin_addr));
    ftp_client.servaddr.sin_family = AF_INET;
    ftp_client.servaddr.sin_port = htons(SERV_PORT);
    CHK((ftp_client.socket = socket(AF_INET, SOCK_STREAM, 0)));

    char buf_recv[BUFSIZ];

    CHK(connect(ftp_client.socket, (struct sockaddr *)&ftp_client.servaddr, sizeof(ftp_client.servaddr)));
    if (code_return(buf_recv, BUFSIZ) != CONN_SUCCESS)
    {
        printf("Error connection\n");
        return;
    }

    char buf[BUFSIZ], buf_user[64];

    int exit = 1;
    while (exit)
    {

        printf("User name : ");
        scanf("%s", buf_user);
        snprintf(buf, BUFSIZ, "USER %s\r\n", buf_user);
        send(ftp_client.socket, buf, strlen(buf), 0);

        if (code_return(buf_recv, BUFSIZ) != USER_SUCCESS)
        {
            printf(RED "Try again !" NC "\n");
            exit = 1;
            continue;
        }

        printf("Password : ");
        scanf("%s", buf_user);

        snprintf(buf, BUFSIZ, "PASS %s\r\n", buf_user);
        send(ftp_client.socket, buf, strlen(buf), 0);
        if (code_return(buf_recv, BUFSIZ) != PASS_SUCCESS)
        {
            printf(RED "Try again !" NC "\n");
            exit = 1;
            continue;
        }

        exit = 0;
    }

    printf(GRN "%d Login successful" NC "\n", 230);
    ftp_client.open = 1;
}

void ftp_dir(void)
{

    char buf_recv[BUFSIZ];
    send_data_port();
    int code = code_return(buf_recv, BUFSIZ);
    if (code == PORT_SUCCESS)
    {

        char *list = "LIST\r\n";
        send(ftp_client.socket, list, strlen(list), 0);

        char buf[BUFSIZ];
        CHK(recv(ftp_client.socket, buf, BUFSIZ, 0));

        int socket_temp;
        CHK(socket_temp = accept(ftp_client.socket_data, NULL,
                                 0));
        int size_rcv = 1;
        while (size_rcv > 0)
        {

            memset(buf, '\0', BUFSIZ);
            CHK((size_rcv = recv(socket_temp, buf, BUFSIZ, 0)));
            printf("%s", buf);
        }

        CHK(close(socket_temp));
        CHK(close(ftp_client.socket_data));
    }
    else
    {
        CHK(close(ftp_client.socket_data));
    }
}

void ftp_get(void)
{
    int fd;
    char file[BUFSIZ];
    snprintf(file, BUFSIZ, "assets/%s", argv_cmd[1]);
    CHK((fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0666)));

    char buf_recv[BUFSIZ];
    send_data_port();
    int code = code_return(buf_recv, BUFSIZ);
    if (code == PORT_SUCCESS)
    {

        char dir[BUFSIZ];
        snprintf(dir, BUFSIZ, "RETR %s\r\n", argv_cmd[1]);
        send(ftp_client.socket, dir, strlen(dir), 0);

        char buf[BUFSIZ];
        CHK(recv(ftp_client.socket, buf, BUFSIZ, 0));

        int socket_temp;
        CHK(socket_temp = accept(ftp_client.socket_data, NULL,
                                 0));

        int size_rcv = 1;
        while (size_rcv > 0)
        {

            memset(buf, '\0', BUFSIZ);
            CHK((size_rcv = recv(socket_temp, buf, BUFSIZ, 0)));
            write(fd, buf, size_rcv);
        }

        CHK(close(socket_temp));
        CHK(close(ftp_client.socket_data));
    }
    else
    {
        CHK(close(ftp_client.socket_data));
    }

    CHK(close(fd));
}

void ftp_ren(void){
    
        char buf[BUFSIZ];

        char origin[BUFSIZ];
        snprintf(origin, BUFSIZ, "RNFR %s\r\n", argv_cmd[1]);
        send(ftp_client.socket, origin, strlen(origin), 0);

        CHK(recv(ftp_client.socket, buf, BUFSIZ, 0));

        char new[BUFSIZ];
        snprintf(new, BUFSIZ, "RNTO %s\r\n", argv_cmd[2]);
        send(ftp_client.socket, new, strlen(new), 0);

        CHK(recv(ftp_client.socket, buf, BUFSIZ, 0));
}

void process_input(void)
{
    if (strcmp(argv_cmd[0], "open") == 0)
        ftp_open();

    if (strcmp(argv_cmd[0], "dir") == 0)
    {
        if (ftp_client.open == 0)
        {
            printf(RED "Not connected to the server" NC "\n");
            return;
        }
        ftp_dir();
    }

    else if (strcmp(argv_cmd[0], "get") == 0)
    {
        if (ftp_client.open == 0)
        {
            printf(RED "Not connected to the server" NC "\n");
            return;
        }
        ftp_get();
    }

     else if (strcmp(argv_cmd[0], "ren") == 0)
    {
        if (ftp_client.open == 0)
        {
            printf(RED "Not connected to the server" NC "\n");
            return;
        }
        ftp_ren();
    }
}

int main(int argc, char **argv)
{
    char command[BUFSIZ];
    int cnt = 1;

    if (argc == 2) {
        ICHK(inet_aton(argv[1], &ftp_client.servaddr.sin_addr));
        ftp_client.ip = 1;
    } else 
        ftp_client.ip = 0;

    while (cnt)
    {
        reset_args();
        memset(command, 0, BUFSIZ);

        takeInput(command);

        if (strcmp("quit", command) == 0)
        {
            cnt = 0;
            continue;
        }
        parse_command(command);
        process_input();
    }

    (void)argc;
    (void)argv;

    return 0;
}