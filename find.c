#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void print_entry(char* dir_entry_name) {
    struct stat info;

    if(stat(dir_entry_name, &info) == -1) {
        perror(dir_entry_name);
    } else {
        if(S_ISDIR(info.st_mode)) {
            printf("%s is a directory\n", dir_entry_name);
        }

        printf("Directory entry: %s\n", dir_entry_name);
    }
}

int is_valid_entry(struct dirent *entry) {
    return (entry != NULL);
}

int is_same_name(const char *entry_name, const char *target_name) {
    return strcmp(entry_name, target_name) == 0;
}

int main(int argc, char* argv[]) {
    char* dir_name = ".";
    DIR *dir_ptr;
    struct dirent* dirent_ptr;
    dir_ptr = opendir(dir_name);

    if(dir_ptr == 0) {
        perror(dir_name);
    } else {
        dirent_ptr = readdir(dir_ptr);

        while(is_valid_entry(dirent_ptr)) {
            if (is_same_name(dirent_ptr->d_name, argv[1])) {
                print_entry(dirent_ptr->d_name);
            }

            dirent_ptr = readdir(dir_ptr);
        }
        
        closedir(dir_ptr);
    }
    
    return 0;
}