#include <stdio.h>
#include <linux/fs.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>


#define CDEV_PATH "/dev/m-dev"
#define LED_ON _IOW('b' , '1', uint8_t*)
#define LED_OFF _IOW('b', '0', uint8_t*)

void print_menu(void)
{
    printf("************* IOCTL ************\n");
    printf("********  0. LED OFF ************\n");
    printf("********  1. LED ON ************\n");
    printf("********  2. Exit ************\n");
    printf(">>>> ");
}

int main(int argc, const char argv[])
{
    int fd, option;

    fd = open(CDEV_PATH, O_RDWR);
    if(fd == -1) {
        printf("Failed to open character device file.\n");
        return -1;
    }

    while(1)
    {
        print_menu();
        scanf("%d", &option);

        switch(option)
        {
            case 1:
                printf("Turn LED ON.\n");
                ioctl(fd, LED_ON, NULL);
                break;
            case 0:
                printf("Turn LED OFF.\n");
                ioctl(fd, LED_OFF, NULL);
                break;
            case 2:
                printf("Exit program.\n");
                exit(EXIT_SUCCESS);
                break;
            default:
                printf("Invalid option.\n");
                break;
        }
    }
    return 0;
}
