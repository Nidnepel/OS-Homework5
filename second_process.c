#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void read_mes(char* name) {
	int     fd;
   	size_t  size;
   	int buf_size = 20;
   	char    str_buf[buf_size];

   	// Только читает из созданного канала
   	if((fd = open(name, O_RDONLY)) < 0){
  	    printf("Can\'t open FIFO for reading\n");
  	    exit(-1);
   	}
   	size = read(fd, str_buf, buf_size);
   	if(size < 0){
  	    printf("Can\'t read string from FIFO\n");
  	    exit(-1);
   	}
   	printf("Second_process read message: %s\n", str_buf);
   	if(close(fd) < 0){
   	   printf("Second_process: Can\'t close FIFO\n"); exit(-1);
   	}
}

void write_mes(char* name) {
   int     fd, result;
   size_t  size;
   char msg[] = "Hello, first process!";
   int msg_size = sizeof(msg);

   (void)umask(0);

   // Вариант с проверкой требует удаления именованного канала
   // if(mknod(name, S_IFIFO | 0666, 0) < 0){
   //   printf("Can\'t create FIFO\n");
   //   exit(-1);
   // }

   // Вариант без проверки позволяет использовать уже созданный FIFO
   mknod(name, S_IFIFO | 0666, 0);

   if((fd = open(name, O_WRONLY)) < 0){
      printf("Can\'t open FIFO for writting\n");
      exit(-1);
   }
   size = write(fd, msg, msg_size);
   if(size != msg_size){
      printf("Can\'t write all string to FIFO\n");
      exit(-1);
   }
   if(close(fd) < 0){
      printf("Writer: Can\'t close FIFO\n"); exit(-1);
   }
   printf("Second process send mess and exit\n");
}

int main(int argc, char *argv[]) {
   read_mes(argv[1]);
   write_mes(argv[2]);
   return 0;
}
