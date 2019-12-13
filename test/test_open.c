#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "../common/Function.h"
        int flag_save_photo = 1;
        int main(){
            int dest = 1234;
        printf("read_frame: save photo...\n");
		char buff_jpegname[64] = {0};
		getTimeNow(buff_jpegname);
		strcat(buff_jpegname,".jpg");
		char path_jpeg[128] = {0};
		sprintf(path_jpeg,"./%s",buff_jpegname);
		printf("%s\n",path_jpeg);
		int fd_save_jpeg = open(path_jpeg,O_RDWR | O_CREAT,S_IRWXU);
		if(fd_save_jpeg == -1){
			printf("open fail.\n");
			flag_save_photo = 0;
			return -1;
		}
		write(fd_save_jpeg,dest,sizeof(dest));
		
		flag_save_photo = 0;
        }