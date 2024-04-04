#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <sys/stat.h>

int is_valid_entry(struct dirent *entry) {
    return (entry != NULL);
}

void mode_to_string(int mode, char str[]) {
    strcpy(str,"----------");
    if(S_ISDIR(mode)) str[0]='d'; // directory?
    if(S_ISCHR(mode)) str[0]='c'; // terminal?
    if(S_ISBLK(mode)) str[0]='b'; // disk?
    if(S_ISLNK(mode)) str[0]='l'; // link?

    if(mode & S_IRUSR) str[1]='r'; // bits for the user
    if(mode & S_IWUSR) str[2]='w';
    if(mode & S_IXUSR) str[3]='x';

    if(mode & S_IRGRP) str[4]='r'; // bits for the group
    if(mode & S_IWGRP) str[5]='w';
    if(mode & S_IXGRP) str[6]='x';

    if(mode & S_IROTH) str[7]='r'; // bits for the others
    if(mode & S_IWOTH) str[8]='w';
    if(mode & S_IXOTH) str[9]='x';
}

void print_file_info(const char *file_name) {
    char file_path[PATH_MAX];
    struct stat file_stat;
    
    if (getcwd(file_path, sizeof(file_path)) != NULL && stat(file_name, &file_stat) == 0) {
        char mode_str[11];
        int mode_number = file_stat.st_mode & 0777;

        printf("%s\n", file_path);
        mode_to_string(file_stat.st_mode, mode_str);
        printf("   %s  (%04o/%s)\n", file_name, mode_number, mode_str);
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
            if (strstr(dirent_ptr->d_name, argv[1])) {
                print_file_info(dirent_ptr->d_name);
            }

            dirent_ptr = readdir(dir_ptr);
        }
        
        closedir(dir_ptr);
    }
    
    return 0;
}
