#include "kc_dms.h"
#include <unistd.h>

/* 开灯 */
void open_dev(const char* dev)
{
    int fd = open(dev, O_RDWR);
    if (fd < 0) {
        log_error("open device failed");
        exit(EXIT_FAILURE);
    }
    unsigned char data = 1;
    write(fd, &data, sizeof(data));
    close(fd);
}

/* 关灯 */
void close_dev(const char* dev)
{
    int fd = open(dev, O_RDWR);
    if (fd < 0) {
        log_error("close device failed");
        exit(EXIT_FAILURE);
    }
    unsigned char data = 0;
    write(fd, &data, sizeof(data));
    close(fd);
}

/* 延时 */
void delay_s_dev(const char* dev, int delay)
{
    open_dev(dev);
    sleep(delay);
    close_dev(dev);
}