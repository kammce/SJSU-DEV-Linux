#include <stdio.h>      // printf() / scanf()
#include <stdlib.h>
#include "io.hpp"       // Board sensors
#include "utilities.h"  // delay_ms()

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

void ctoba(char * arr, char num) {
	arr[4] = 0;
	for(int i = 3; i >= 0; i--) {
		if(num & (8 >> i)) {
			arr[i] = '1';
		} else {
			arr[i] = '0';
		}
	}
}

int main (void)
{
	char buff[5] = {0};
	//void (*LED[2])( void ) = { &LE.on, &LE.off };
	while(1)
	{
		printf("Hello World\n");
		// for(int i = 0; i < 16; i++)
		// {
		// 	ctoba(buff, (char)i);
		// 	printf("Hello Preet i = 0b%s\n", buff);
		// 	for(int j = 4; j >= 1; j--)
		// 	{
		// 		//LED[CHECK_BIT(i,j)]();
		// 		if(CHECK_BIT(i,j))
		// 		{
		// 			LE.on(j);
		// 		}
		// 		else
		// 		{
		// 			LE.off(j);
		// 		}
		// 	}
		// 	LD.setNumber(i);
		// 	delay_ms(1500);
		// }
		delay_ms(1500);
	}
	return 0;
}
