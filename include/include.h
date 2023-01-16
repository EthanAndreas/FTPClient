#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERV_PORT 21
#define DATA_PORT 55000
#define CONN_SUCCESS 220
#define USER_SUCCESS 331
#define PASS_SUCCESS 230
#define PORT_SUCCESS 200

// reset
#define NC "\e[0m"
// text color
#define BLU "\e[30m"
#define RED "\e[31m"
#define GRN "\e[32m"
#define YEL "\e[33m"
#define CYN1 "\e[34m"
#define MAG "\e[35m"
#define CYN2 "\e[36m"
#define WHT "\e[37m"
// background color
#define RED_B "\e[41m"
#define GRN_B "\e[42m"
#define ORA_B "\e[43m"
#define CYN1_B "\e[44m"
#define YEL_B "\e[45m"
#define CYN2_B "\e[46m"
#define WHT_B "\e[47m"

#define CHK(op)            \
    do {                   \
        if ((op) == -1)    \
            alert(1, #op); \
    } while (0)

#define ICHK(op)                     \
    do {                             \
        if ((op) == 0) \
            alert(1, #op);           \
    } while (0)

#define NCHK(op)           \
    do {                   \
        if ((op) == NULL)  \
            alert(1, #op); \
    } while (0)

#define TCHK(op)                \
    do {                        \
        if ((errno = (op)) > 0) \
            alert(1, #op);      \
    } while (0)

#define SCHK(op)                \
    do {                        \
        if ((errno = (op)) < 0) \
            alert(1, #op);      \
    } while (0)

noreturn void alert(int syserr, const char *msg, ...) {
    va_list ap;

    va_start(ap, msg);
    vfprintf(stderr, msg, ap);
    fprintf(stderr, "\n");
    va_end(ap);

    if (syserr == 1)
        perror("");

    exit(EXIT_FAILURE);
}