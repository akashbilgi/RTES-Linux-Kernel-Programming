#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define IOCTL_GET_TIME_NS 0x12345678

int main(void) {
  int fd;
  long current_time;

  fd = open("/dev/simple_etx_misc", O_RDONLY);
  if (fd < 0) {
    perror("Failed to open /dev/rtesdev");
    return 1;
  }

  if (ioctl(fd, IOCTL_GET_TIME_NS, &current_time) < 0) {
    perror("Failed to get current time");
    close(fd);
    return 1;
  }

  printf("Current time in ns: %ld\n", current_time);

  close(fd);
  return 0;
}

