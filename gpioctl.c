#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

 
int main(int argc, char *argv[])
{
    int valuefd, exportfd, directionfd;
    char *num, *value, *direct;
    char directionpath[50]={0};
    char valuepath[50]={0};

    printf("GPIO test running...\n");

    if( argc < 2 || argc > 4 ){
        printf("Usage: ./goipctl GPIO_NUMBER [IN/OUT] [HIGH/LOW]\n");
        printf("Example:  ./gpioctl 152 out 1  <-- set gpio pin 152 output high\n");
        printf("          ./gpioctl 152 in     <-- set gpio pin 152 input\n");
        exit(1);
    }
    
    num = argv[1];
    if ( argc >= 3 )
        direct = argv[2];
    if ( argc == 4 )
        value = argv[3];

    sprintf(directionpath,"/sys/class/gpio/gpio%s/direction",num);
    sprintf(valuepath,"/sys/class/gpio/gpio%s/value",num);

 
    // The GPIO has to be exported to be able to see it
    // in sysfs
 
    exportfd = open("/sys/class/gpio/export", O_WRONLY);
    if (exportfd < 0)
    {
        printf("Cannot open GPIO to export it\n");
        exit(1);
    }
    write(exportfd, num, strlen(num));
    close(exportfd);
    printf("GPIO exported successfully\n");
 
    // Update the direction of the GPIO to be an output
    if ( argc >= 3 ){
        directionfd = open(directionpath, O_RDWR);
        if (directionfd < 0)
        {
            printf("Cannot open GPIO direction it\n");
            exit(1);
        }
        write(directionfd, direct, strlen(direct));
        printf("GPIO direction is [%s]\n", direct);
        close(directionfd);
    }
 
    // Get the GPIO value ready to be toggled
    if ( argc == 4 ){
        valuefd = open(valuepath, O_RDWR);
        if (valuefd < 0)
        {
            printf("Cannot open GPIO value\n");
            exit(1);
        }
        write(valuefd, value, strlen(value));
        printf("GPIO value is [%s]\n", value);
        close(valuefd);
    }
 
/*
    exportfd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (exportfd < 0)
    {
        printf("Cannot open GPIO to unexport it\n");
        exit(1);
    }
 
    write(exportfd, num, strlen(num));
    close(exportfd);
    printf("GPIO exported successfully\n");
*/
    exit(0);
}
