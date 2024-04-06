#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <sys/types.h>

int is_valid_entry(struct dirent* entry) {
    return (entry != NULL);
}

void mode_to_string(int mode, char str[]) {
    strcpy(str, "----------");
    if (S_ISDIR(mode)) str[0] = 'd'; // directory?
    if (S_ISCHR(mode)) str[0] = 'c'; // terminal?
    if (S_ISBLK(mode)) str[0] = 'b'; // disk?
    if (S_ISLNK(mode)) str[0] = 'l'; // link?

    if (mode & S_IRUSR) str[1] = 'r'; // bits for the user
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';

    if (mode & S_IRGRP) str[4] = 'r'; // bits for the group
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';

    if (mode & S_IROTH) str[7] = 'r'; // bits for the others
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';
}

void print_file_mode(const char* file_name, mode_t mode) {
    char mode_str[11];
    int mode_number = mode & 0777;
    mode_to_string(mode, mode_str);
    printf("   (%04o/%s)\n", mode_number, mode_str);
}

void search_and_print_files(const char* dir_name, const char* search_term, const char* full_path) {
    DIR* dir_ptr = opendir(dir_name);
    struct dirent* dirent_ptr;

    if (dir_ptr == NULL) {
        perror(dir_name);
        return;
    }

    dirent_ptr = readdir(dir_ptr);

    while (is_valid_entry(dirent_ptr)) {
        struct stat file_stat;
        char file_path[PATH_MAX];

        snprintf(file_path, sizeof(file_path), "%s/%s", full_path, dirent_ptr->d_name);

        if (stat(file_path, &file_stat) == 0) {
            if (strstr(dirent_ptr->d_name, search_term)) {
                printf("%s\n", file_path);

                printf("   %s", dirent_ptr->d_name);
                print_file_mode(dirent_ptr->d_name, file_stat.st_mode);
            }

            if (S_ISDIR(file_stat.st_mode) && strcmp(dirent_ptr->d_name, ".") && strcmp(dirent_ptr->d_name, "..")) {
                search_and_print_files(file_path, search_term, file_path);
            }
        }

        dirent_ptr = readdir(dir_ptr);
    }

    closedir(dir_ptr);
}

int main(int argc, char* argv[]) {
    char current_dir[PATH_MAX];
    char* search_term;

    if (argc == 1) {
        printf("Usage: %s [path] [search term]\nCurrent path is default.", argv[0]);
        return 1;
    } else if (argc == 2) {
        strcpy(current_dir, ".");
        search_term = argv[1];

        if (argc <= 2 && getcwd(current_dir, sizeof(current_dir)) == NULL) {
            perror("getcwd() error");
            return 1;
        }
    } else {
        strcpy(current_dir, argv[1]);
        search_term = argv[2];
    }

    search_and_print_files(current_dir, search_term, current_dir);

    return 0;
}