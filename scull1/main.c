#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	char *str = "Hello,world!";
	int iCount;


	fd = open("/dev/my_mem_device", O_WRONLY | O_CREAT);
	iCount = write(fd, str, 12);      /* str is 12 bytes length */
	printf("%d bytes written!\n", iCount);
	close(fd);
}
