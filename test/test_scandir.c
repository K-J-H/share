    //    #include <dirent.h>

    //    int scandir(const char *dirp, struct dirent ***namelist,
    //           int (*filter)(const struct dirent *),
    //           int (*compar)(const struct dirent **, const struct dirent **));

    //    int alphasort(const void *a, const void *b);

    //    int versionsort(const void *a, const void *b);

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int filter(const struct dirent *file_info){
    int length = 0;
    while(file_info->d_name[length]){
        length ++;
    }
    char* last_four_character = file_info->d_name + length - 4;
    if(strcmp(last_four_character,".bmp") == 0){
        return 1;
    }
    return 0;
}

int
main(void)
{
    // "/kjh/PHOTO/PICTURES"
    char* path_pictures ="/mnt/hgfs/share/PHOTO/PICTURES";
    struct dirent **namelist;
    int n;
    n = scandir(path_pictures, &namelist, filter, alphasort);
    if (n < 0)
        perror("scandir");
    else {
        while (n--) {
            printf("%s\n", namelist[n]->d_name );
            free(namelist[n]);
        }
        free(namelist);
    }
}
