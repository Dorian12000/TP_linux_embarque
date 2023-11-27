#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define N_LED 9
#define BUF_SIZE 128

typedef struct led_s {
    int fd_previous;
    int fd_next;
}led_t;

static void ledToggle(led_t);

int main(void)
{
    const char *path = "/sys/class/leds/fpga_led";
    led_t led = {-2, -2};

     while(1)
     {
        for(int i = 0; i <= N_LED; i++)
        {
            char file_name[BUF_SIZE];
            sprintf(file_name, "%s%d%s", path, i, "/brightness");
            led.fd_next = open(file_name, O_RDWR);
            if(led.fd_next == -1)
            {
                 perror("open file\n");
                 exit(EXIT_FAILURE);
            }

            ledToggle(led);

            if(led.fd_previous != -2)
                close(led.fd_previous);
            
            led.fd_previous = led.fd_next;
            usleep(1000 * 100);
        }
    }
    close(led.fd_next);

    return 0;
}

/**
 * @brief turn on the next led and turn off the previous one
 * 
 * @param led : -led fd to turn off
                -led fd to turn off
 */
static void ledToggle(led_t led) {
    write(led.fd_next, "1", 1);
    if(led.fd_previous != -2)
        write(led.fd_previous, "0", 1);
}