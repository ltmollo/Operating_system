#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

int main() {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    long long total_size = 0;

    dir = opendir(".");         // open current catalog
    if (dir == NULL) {
        fprintf(stderr, "Can't open a catalog\n");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {        //browse catalog

        if (entry->d_name[0] == '.' || stat(entry->d_name, &file_stat) == -1) { // don't want hidden files
            continue;
        }

        if (!S_ISDIR(file_stat.st_mode)) {      // if not catalog
            printf("%lld %s\n", (long long)file_stat.st_size, entry->d_name);
            total_size += file_stat.st_size;
        }
    }

    closedir(dir);

    printf("Sumaryczny rozmiar plikow: %lld\n", total_size);

    return 0;
}
