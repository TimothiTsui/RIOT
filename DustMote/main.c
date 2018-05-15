#include <stdio.h>
#include <fmt.h>
#include <periph/gpio.h>
#include <periph/i2c.h>

int main(void){
    //puts("Hello test!");


    printf("You are running RIOT on a(n) %s board.\n",  BOARD_REMOTE_REVB);
    printf("This board features a(n) %s MCU.\n", RIOT_MCU);




    return 0;
}
