#ifndef __SERIAL_PORT_H__ 
#define __SERIAL_PORT_H__ 


#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <termios.h> 
#include <errno.h> 
#include <pthread.h> 

int serial_init(void);
int serial_send(unsigned char *buf, int len);
int serial_recv(unsigned char *buf, int len);


#endif 

