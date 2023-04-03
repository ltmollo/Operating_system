#include <stdio.h>
#include <ftw.h>
#include <libgen.h>

static long long total_size = 0;

int display_file_info(const char *fpath, const struct stat *sb, int typeflag) {
    if (!S_ISDIR(sb->st_mode)) {
        printf("%lld %s\n", (long long)sb->st_size, basename((char*)fpath));
        total_size += sb->st_size;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        return 1;
    }

    if (ftw(argv[1], display_file_info, 20) == -1) {
        fprintf(stderr, "ERROR going through directories");
        return 1;
    }

    printf("Sumaryczny rozmiar plikow: %lld\n", total_size);
    return 0;
}