#include <stdio.h>
#include <sys/wait.h>

#define BUF_SIZE 256

/*
 * Funkcja 'get_date' powinna
 * 1) wywołać polecenie 'date'
 * 2) z jego wyjścia standardowego wczytać linijkę do bufora 'buf'
 * 3) poczekać na zakończenie utworzonego procesu i zwrócić jego kod wyjścia
 */
int get_date(char* buf, int size) {
    FILE* fp = popen("date", "r");
    if (fp == NULL) {
        perror("error");
        return -1;
    }
    fgets(buf, size, fp);
    int status = pclose(fp);
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    return -1;
}

int main(void) {
    char buf[BUF_SIZE];
    get_date(buf, BUF_SIZE);

    if (wait(NULL) != -1) {
        printf("Child not reaped\n");
    }
    printf("Date: %s", buf);
    return 0;
}
