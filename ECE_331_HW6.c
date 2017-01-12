/* Joseph Garcia */
/* ECE 331 HW 6 */
/* Sheaff */

1.

#include <bcm2835.h>  /* library for bcm2835 functions */
#include <stdio.h>
#include <unistd.h>   /* standard I/O, ability to pass in environment vars, etc.*/
#include <stdlib.h>
#include <inttypes.h> /* library to include uint types */

int main(int argc, char *argv[])
{
	if (bcm2835_init() != 1) {
		printf("bcm2835 library not successfully opened.\n");
		return 1;
        }
	const uint8_t RED = 22;
	const uint8_t GREEN = 23;
	const uint8_t BLUE = 24;  /* create constants representing GPIO pins */
	const uint8_t CLK = 25;    
	const int NBITS = 11;
	int blue = 0;
	int red = 0;             /* integer values of colors */
	int green = 0;
	int redb = 0;
	int greenb = 0;         /* result of color integers being bitshifted right and then anded with 1 to give appropriate rgb combo to clock */
	int blueb = 0;
	int k = 0; /* looping variable */

	/* initialize state of pins */
	bcm2835_gpio_write(RED,LOW);
        bcm2835_gpio_write(GREEN,LOW); 
        bcm2835_gpio_write(BLUE,LOW);
        bcm2835_gpio_write(CLK,LOW);
	
	/* intialize R,G,B pins for output */
	bcm2835_gpio_fsel(RED,BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(GREEN,BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(BLUE,BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(CLK,BCM2835_GPIO_FSEL_OUTP);

	/* pass in RGB on cmd line. N.B. Enter ./program 0 0 0 on command line to have lights go off */
	red = atoi(argv[1]);
	green = atoi(argv[2]);   /* convert input strings into numbers */ 
	blue = atoi(argv[3]);

	if (red < 0 || green < 0 || blue < 0 || red > 2047 || green > 2047 || blue > 2047) {
		printf("One of your numbers is invalid. Exiting Program.\n");     /* make sure numbers are valid */
		return 2;
	}
	printf("Passed in numbers for R = %d, G = %d, B = %d\n",red,green,blue);

	red = ~red;
	green = ~green;  /* complement values for later bitshifting */
	blue = ~blue;	

	for(k = 0; k < NBITS; k++) {
		redb = (red >> (10 - k)) & 1; /* shift red color value msb to 0-bit position, and with 1 to get proper output signal (0 or 1) */
		bcm2835_gpio_write(RED, redb);
		greenb = (green >> (10 - k)) & 1;    /* same as above */
		bcm2835_gpio_write(GREEN, greenb);
		blueb = (blue >> (10 - k)) & 1;     /* same as above */
		bcm2835_gpio_write(BLUE, blueb);

		/* create HIGH-LOW clock cycle */
		bcm2835_gpio_write(CLK,HIGH);  
		bcm2835_delayMicroseconds(14);   /* delay for 14 us to account for "twitching" in signal. */
		bcm2835_gpio_write(CLK,LOW);
		bcm2835_delayMicroseconds(14);  /* then move to next iteration */
	}
	
	/* close the program */
	if (bcm2835_close() == 0) {
		printf("Unable to deallocate memory and close program.\n");
		return 3;
	}
	return 0;  /* successful program execution */
}

2.
CC = gcc     # recursive assignment of gcc as compiler.
TARGET = led  # recursive assignment of led as executable
OPTS = -g -Wall        # allow gcc to debug and print nearly all errors
OBJ_FILES = led.o      # object file made from led.c after compiling
ADDLIBS = lbcm2835     # add lbcm2835 since it is not in the default search path
.PHONY: clean             # clean is a non-file target (a command to be executed)

${TARGET}: ${OBJ_FILES}                # use OBJ_FILES to create TARGET
	${CC} ${OPTS} -o ${TARGET} ${OBJ_FILES} -${ADDLIBS}     #command for gcc to make executable, format just like the command line gcc 

clean:
	@echo "The object files and target executable have been removed."   # clean out executable and object files and start new
	-rm -f ${TARGET} ${OBJS}

3.
#include <stdio.h>
#include <linux/fs.h>               /* for sd card geometry */
#include <linux/hdreg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>         /* for file control */
#include <sys/ioctl.h>
#include <errno.h>    /* for error returns */

int main(int argc, char *argv[])
{
	struct hd_geometry geostruct;

	int fd = 0; 

	fd = open("/dev/mmcblk0",O_RDONLY|O_NONBLOCK|S_IRUSR);	/* open as read only, with read permissions for users. heard O_NONBLOCK makes opening less troublesome */
	if (fd == -1) {
		printf("Unable to open the sd file location.\n");  /* make sure we are able to open sd card device file */
		return -1;
	}
	if (ioctl(fd,HDIO_GETGEO,&geostruct) == -1) {
		printf("Could not instantiate geometry struct.\n");  /* make sure we fill geometry struct with valid information */
		printf("errno = %d", errno);
		return errno;
	} 
	printf("amt of heads is %x, sectors is %x, cylinders is %u and start is %lu\n",(unsigned char) geostruct.heads, (unsigned char) geostruct.sectors,(unsigned short int) geostruct.cylinders, geostruct.start);
	close(fd);  /* close the file opened earlier */
	return 0; /* successful program execution */
}

4.
obj-m += rgbled.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

5.
Followed the instructions at the listed URL.

6.
Completed. Used command minicom -b -115200 -D -r /dev/ttyAMA0. Text was echoed.

7.
#include <stdio.h>
#include <errno.h>
#include <termios.h> /* use for termios */
#include <unistd.h>  /* use for termios */
#include <fcntl.h>

int main(int argc, char *argv[])
{
	const int BUFSIZE = 20; 
	char buf[BUFSIZE];    /* buffer to read data from STDIN */
	int opAMA0 = 0; /* initialize result of opening of ttyAMAO device file */
	struct termios srlport;	/* initialize instance of struct representing serial port configuration */

	ssize_t readres = 0;
	ssize_t writeres = 0;      /* file reading and writing variable initializations. */
	int k = 0;
	opAMA0 = open("/dev/ttyAMA0",O_RDWR|O_APPEND, S_IRUSR|S_IWUSR); /* read from /write to serial port with the proper permissions, append data when we write to it. */

	/* First instantiate termios struct by getting serial port parameters */
	if (tcgetattr(opAMA0, &srlport) < 0) {
	        perror("Unable to find parameters (attributes) for the desired device file.\n");
        	return errno ;
 	}   	

	/* Set the speed data in the structure */
    	if (cfsetispeed(&srlport, B115200) < 0) {
	        perror("invalid baud rate");       /* ensure input Baud rate is correct */
        	return errno;
    	}

	if (cfsetospeed(&srlport, B115200) < 0) {
		perror("invalid baud rate");     /* ensure output Baud rate is correct */
		return errno;
	}
    	/* Apply the settings.*/
	if(tcsetattr(STDIN_FILENO, TCSANOW, &srlport) < 0) {
	        perror("stdin");                          /* activate changes immediately. No reading or writing from file descriptors yet. */
        	return errno;
    	}
    	
	srlport.c_cflag = (srlport.c_cflag & ~CSIZE) | CS8; /* make 8 data bits */
	srlport.c_cflag &= ~(PARENB | PARODD);      /* shut off parity */  /* Taken from wallyk at stackoverflow.com */
        srlport.c_cflag |= 0;                /* make parity N */
        srlport.c_cflag &= ~CSTOPB;              /* make 1 stop bit */
	
	read(STDIN_FILENO,buf,sizeof(buf));
	if (readres < 0) {
		printf("Unable to read from STDIN.\n");  /* ensure ability to read from STDIN */
		return errno;
	}
	writeres = write(opAMA0, buf, sizeof(buf));     /* write to serial port */
	if (writeres < 0) {
		printf("Unable to write to serial port.\n"); /* write check */
		return errno;
	}
	usleep((sizeof(buf)) * 100);             /* sleep, allowing chars to transmit. Inspired by wallyk on stackoverflow.com. */
	readres = read(opAMA0, buf,sizeof(buf));
	if (readres < 0) {
		printf("Unable to read from serial port.\n"); /* check that read from serial port was OK. */
		return errno;
	}
	printf("String is %s\n",buf);  /* print char buffer to STDOUT */
	close(opAMA0);    /* close file descriptor */
	return 0;  /* successful program execution */
}

