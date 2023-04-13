#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc == 2) {
        if (strcmp(argv[1], "nadawca") == 0) {
            FILE* mail = popen("mail -f /var/mail/$USER | sort -k2", "w");
            pclose(mail);
        } else if (strcmp(argv[1], "data") == 0) {
            FILE* mail = popen("mail -f /var/mail/$USER | sort -k4 -k5 -k6", "w");
            pclose(mail);
        } else {
            printf("Invalid arg\n");
            return 1;
        }
    } else if (argc == 4) {
        char* cmd = malloc(strlen("echo '") + strlen(argv[3]) + strlen("' | mail -s '") + strlen(argv[2]) + strlen("' ") + strlen(argv[1]) + 1);
        sprintf(cmd, "echo '%s' | mail -s '%s' %s", argv[3], argv[2], argv[1]);
        FILE* mail = popen(cmd, "w");
        pclose(mail);
        free(cmd);
    } else {
        printf("Wrong nb of args\n");
        return 2;
    }

    return 0;
}
