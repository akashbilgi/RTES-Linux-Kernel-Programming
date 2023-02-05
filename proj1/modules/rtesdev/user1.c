#include <stdio.h>
#include <linux/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

#define IOCTL_PRINT_HELLO _IO(0,0)

int main()
{
    int fd;

    fd = open("/dev/simple_etx_misc", O_RDWR);
  
    ioctl(fd, IOCTL_PRINT_HELLO, 0);
    
    close(fd);
    
    return 0;
}
