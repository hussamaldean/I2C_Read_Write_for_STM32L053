#include "i2c.h"
#include "uart.h"
#include "stdio.h"
#include "stdlib.h"
#define slave_add (0x68)


uint8_t data[3],data_send[3];

int bcd_to_decimal(unsigned char x) {
    return x - 6 * (x >> 4);
}

int main()
	{
	uart_init();
	i2c_init(0x00708);
	while(1)
		{
		i2_read(slave_add,0x00,data,3);
		for (volatile int i=0;i<3;i++)
			{
			data[i]=bcd_to_decimal(data[i]);

			}
		printf("rtc data %d %d %d\r\n",data[2],data[1],data[0]);
		for (volatile int i=0;i<100000;i++);
		}

	}
