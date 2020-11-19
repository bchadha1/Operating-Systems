#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>

#define MSG_SIZE 250

int main()
{
	char buffer[MSG_SIZE];

	int fd;

	fd= open("/dev/process_list", O_RDONLY);
	if (fd < 0)
	{
		perror("ERROR opening device process_list");
		return 0;
	}

	int bytes_read ;
	while(bytes_read = read(fd, buffer, strlen(buffer)) > 0)
	{
		printf("%s\n", buffer);
		memset(buffer, 0, sizeof(char)*MSG_SIZE);
	}
	if(bytes_read < 0)
	{
		perror("ERROR reading from device Process_List!");
		return 0;
	}
	close(fd);

	return 0;
}

