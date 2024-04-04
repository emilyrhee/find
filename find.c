#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>

int is_valid_entry(struct dirent *entry) {
    return (entry != NULL);
}

int is_same_str(const char *entry_name, const char *target_name) {
    return !strcmp(entry_name, target_name);
}

void print_file_info(const char *file_name) {
    char file_path[PATH_MAX];
    
    if (getcwd(file_path, sizeof(file_path)) != NULL) {
        printf("%s\n", file_path);
        printf("   %s\n", file_name);
    } else {
        perror("getcwd() error");
    }
}

int main(int argc, char* argv[]) {
    char* dir_name = ".";
    DIR* dir_ptr;
    struct dirent* dirent_ptr;
    dir_ptr = opendir(dir_name);

    if(dir_ptr == NULL) {
        perror(dir_name);
    } else {
        dirent_ptr = readdir(dir_ptr);

        while(is_valid_entry(dirent_ptr)) {
            if (is_same_str(dirent_ptr->d_name, argv[1])) {
                print_file_info(dirent_ptr->d_name);
            }

            dirent_ptr = readdir(dir_ptr);
        }
        
        closedir(dir_ptr);
    }
    
    return 0;
}
