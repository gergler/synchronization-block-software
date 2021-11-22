#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define ALT_LWFPGASLVS_OFST 0xFF200000
#define LED_PIO_BASE             0x3000
#define LED_PIO_SPAN            0x10

int main(void)
{
          unsigned long *led_pio;
          int fd;
          int i;

          fd = open("/dev/mem", (O_RDWR | O_SYNC));


          //Map LED_PIO Physical Address to Virtual Address Space
          led_pio = mmap( NULL, LED_PIO_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, (ALT_LWFPGASLVS_OFST + LED_PIO_BASE) );  

	  //led_pio[1] = 0xf0;
          //Toggle all LEDs every 500ms a few times
	  int multiplier = 100;
          for (i=0; i < 1000000; i++)
          {  
	       *led_pio = 0xFC;
	       usleep(2*multiplier);
               *led_pio = 0xC0;
               usleep(8*multiplier);
          }


          //Unmap
          munmap(led_pio, LED_PIO_SPAN);

          close(fd);
          return(0);
}
