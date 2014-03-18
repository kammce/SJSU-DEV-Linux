#include <stdio.h>      // printf() / scanf()
#include <stdlib.h>
#include "io.hpp"       // Board sensors
#include "utilities.h"  // delay_ms()

void * ctoba(char * arr, char num) {
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
	char str[200] = {0};
	char buff[5] = {0};
	while(1) {
		for(int i = 0; i < 16; i++) {
			ctoba(buff, (char)i);
			printf("Akash i = 0b%s\n", buff);
			for(int j = 4; j >= 1; j--) {
				if(i & (8 >> j-1)) {
					LE.on(j);
				} else {
					LE.off(j);
				}
			}
			LD.setNumber(i);
			delay_ms(1500);
		}
	}
	return 0;
}
