#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <stdlib.h>

#define ALT_LWFPGASLVS_OFST 0xFF200000


#define PERIOD_BASE 0x7000
#define PERIOD_SPAN 0x8
#define DUTY_BASE 0x8000
#define DUTY_SPAN 0x8

void *period, *duty;


int main(int argc, char* argv[])
{
          int fd;
          int i;

          fd = open("/dev/mem", (O_RDWR | O_SYNC));

	  period = mmap (NULL, PERIOD_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, (ALT_LWFPGASLVS_OFST + PERIOD_BASE));	
	  duty = mmap (NULL, DUTY_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, (ALT_LWFPGASLVS_OFST + DUTY_BASE));


	  printf("%d, %d (ns)\n", 
			  20* *reinterpret_cast<unsigned long*>(period),
			  20* *reinterpret_cast<unsigned long*>(duty));	  


          //Unmap
          munmap(period, PERIOD_SPAN);
	  munmap(duty, DUTY_SPAN);

          close(fd);
          return(0);
}
