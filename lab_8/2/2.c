#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>


void main() {
    int fd, to_read;
    fd = open("/dev/fibdev", O_RDONLY | O_APPEND);

    scanf(" %d", &to_read);

    if (fd < 0) {
        printf("nie ma urzadzenia!\n");
        abort();
    }

    unsigned long long val = lseek(fd, to_read, 0);
    printf("odczytano %lld\n", val);

    close(fd);
}
