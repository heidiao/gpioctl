#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define DEBUG 1
 
int main(int argc, char *argv[])
{
    int valuefd, exportfd, directionfd;
    char *num, *value, *direct;
    char directionpath[50]={0};
    char valuepath[50]={0};

    printf("GPIO test running...\n");

    if( argc !=4 ){
        printf("Usage: ./goipctl GPIO_NUMBER IN/OUT HIGH/LOW\n");
        printf("Example:  ./gpioctl 152 in/out 1  <-- set gpio pin 152 high\n");
        printf("          ./gpioctl 152 in/out 0  <-- set gpio pin 152 low\n");
        exit(1);
    }
    
    num = argv[1];
    direct = argv[2];
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
    directionfd = open(directionpath, O_RDWR);
    if (directionfd < 0)
    {
        printf("Cannot open GPIO direction it\n");
        exit(1);
    }
 
    write(directionfd, direct, strlen(direct));
    close(directionfd);
    printf("GPIO direction set as output successfully\n");
 
    // Get the GPIO value ready to be toggled
    valuefd = open(valuepath, O_RDWR);
    if (valuefd < 0)
    {
        printf("Cannot open GPIO value\n");
        exit(1);
    }
    printf("GPIO value opened, now toggling...\n");
    write(valuefd, value, strlen(value));
 

    exportfd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (exportfd < 0)
    {
        printf("Cannot open GPIO to unexport it\n");
        exit(1);
    }
 
    write(exportfd, num, strlen(num));
    close(exportfd);
    printf("GPIO exported successfully\n");
    exit(0);
}
