/**   
 * @file             
 * @brief      
 * @version    1.0  
 * @author     liujm
 * @date       2014/12/23 
 */  
 
#include <stdio.h> 
#include <string.h>
#include <sys/ioctl.h> 
#include <sys/file.h>
#include <signal.h> 
#include "serialport.h" 

#define SERIAL_DEV "/dev/ttyS0"

static int g_serial_fd = -1;
static pthread_mutex_t g_serial_mutex;

static int serial_set_opt(int fd, int speed, int bits, char stopbits, int parity)
{
    int speed_arr[] = { B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300};
    int name_arr[] = { 115200, 38400, 19200, 9600, 4800, 2400, 1200, 300};
    struct termios opts;
    int i;
    int status;

    tcgetattr(fd, &opts);
    opts.c_iflag = 0;
    opts.c_oflag = 0;
    opts.c_lflag = 0;
    
    //设置波特率
    for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++)
    {
        if (speed == name_arr[i])
        {
            tcflush(fd, TCIOFLUSH);
            cfsetispeed(&opts, speed_arr[i]);       //设置输入波特率
            cfsetospeed(&opts, speed_arr[i]);       //设置输出波特率
            status = tcsetattr(fd, TCSANOW, &opts); //改变终端属性，TCSANOW：不等数据传输完毕就立即改变属性
            if ( 0 != status)
            {
                printf("tcsetattr error, set speed error\n");
                return -1;
            }
            tcflush(fd, TCIOFLUSH);                 //TCOFLUSH  刷清输出队列
        }
    }

    opts.c_cflag |= (CLOCAL | CREAD);

    //设置数据位
    opts.c_cflag &= ~CSIZE;
    switch (bits)
    {
        case 7:
            opts.c_cflag |= CS7;
            break;
            
        case 8:
            opts.c_cflag |= CS8;
            break;
            
        default:
            printf("data bit error\n");
            return -1;
    }

    //设置校验位
    switch (parity)
    {
        case 'n':
        case 'N':  //无奇偶校验
            opts.c_cflag &= ~PARENB;             
            opts.c_iflag &= ~INPCK;             
            break;
            
        case 'o':
        case 'O':  //奇效验
            opts.c_cflag |= PARODD | PARENB;   
            opts.c_iflag |= INPCK | ISTRIP;              
            break;
            
        case 'e':
        case 'E':  //偶校验
            opts.c_cflag |= PARENB;              
            opts.c_cflag &= ~PARODD;             
            opts.c_iflag |= INPCK | ISTRIP;               
            break;
            
        default:
            printf("parity error\n");
            return -1;
    }

    //设置停止位
    switch (stopbits)
    {
        case 1:
            opts.c_cflag &= ~CSTOPB;
            break;
            
        case 2:
            opts.c_cflag |= CSTOPB;
            break;
            
        default:
            printf("stop bits error\n");
            return -1;
    }


    // 设置等待时间和最小接收字符
    //opts.c_cc[VTIME] = 0;    
    //opts.c_cc[VMIN] = 0;    

    tcflush(fd, TCIFLUSH);
    
    if (tcsetattr(fd, TCSANOW, &opts) != 0)
    {
        printf("tcsetattr error\n");
        return -1;
    }

    printf("serial set done\n");

    return 0;
}


int serial_init(void)
{
    g_serial_fd = open(SERIAL_DEV, O_RDWR | O_NDELAY | O_NOCTTY);
    
    if (g_serial_fd < 0)
    {
        printf("%s open error\n", SERIAL_DEV);
        return -1;
    }

    if (serial_set_opt(g_serial_fd, 38400, 8, 1, 'N') != 0)
    {
        printf("serial set option error\n");
        close(g_serial_fd);
        return -1;
    }

    if (pthread_mutex_init(&g_serial_mutex, NULL) != 0)
    {
        printf("serial mutex init error\n");
        close(g_serial_fd);
        return -1;
    }

    return 0;
}



int serial_recv(unsigned char *buf, int len)
{
    int rlen = 0;
    struct timeval tv;
    fd_set rfdset;
    int i;
    
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    FD_ZERO(&rfdset);
    FD_SET(g_serial_fd, &rfdset);

    switch (select(g_serial_fd + 1, &rfdset, NULL, NULL, &tv))
    {
        case -1:    //error
            break;
            
        case 0:     //timeout
            break;
            
        default:
            if (FD_ISSET(g_serial_fd, &rfdset))
            {
                rlen = read(g_serial_fd, buf, len);

                if(rlen > 0)
                {
                    printf("recv data from uart: \n");
                    for(i = 0; i < rlen; i++)
                    {
                        printf("0x%02X ", buf[i]);
                    }
                    printf("\n");
                }
            }
            break;
    }

    return rlen;
}

int serial_send(unsigned char *buf, int len)
{
    int wlen;
    
    pthread_mutex_lock(&g_serial_mutex);
    wlen = write(g_serial_fd, buf, len);
    pthread_mutex_unlock(&g_serial_mutex);

    return wlen;
}




// 接口测试 
#if 0
int main(int argc,char **argv) 
{ 
    //int test_fd;
    int rlen;
    unsigned char buf[128];
    
    if (serial_init() < 0)
    {
        return -1;
    }

    #if 0
	test_fd = open("/tmp/hello", O_RDWR | O_CREAT);
	if (test_fd < 0)
	{
		printf("open test_fd error\n");
		return -1;
	}
    #endif
    
    while (1) 
    { 
        if ((rlen = serial_recv(buf, 128)) > 0)
        {
            //write(test_fd, buf, rlen);
            //serial_send(buf, rlen);
        } 
    } 

    //close(test_fd);
    return 0;
    
} 

#endif

