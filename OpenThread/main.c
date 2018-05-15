#include <stdio.h>
#include <stdlib.h>
#include <fmt.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include "net/ipv6/addr.h"
#include "openthread/ip6.h"
#include "openthread/thread.h"
#include "openthread/udp.h"
#include "ot.h"
#include "shell.h"
#include "shell_commands.h"

int main(void){
    //puts("Hello test!");


    printf("You are running RIOT on a(n) %s board.\n",  BOARD_REMOTE_REVB);
    printf("This board features a(n) %s MCU.\n", RIOT_MCU);


    //GPIO_NUM_TO_DEV(GPIO_PC5);
//      gpio_init(GPIO_PIN(2,5), GPIO_OUT);
//      gpio_set((gpio_t) 1074638815);
//
////    xtimer_usleep(1000);
//
//    printf("OR  %i.\n", gpio_read(PORTNUM_MASK | GPIO_PC1));
    //printf("%lu\n", GPIO_PIN(GPIO_PC5, GPIO_PC5));

    //i2c_poweroff(I2C_0_DEV);

    printf("Get PANID\n");
    uint16_t panid = 0;
    uint8_t res = ot_call_command("panid", NULL, (void*)&panid);
    printf("Current panid: 0x%x (res:%x)\n", panid, res);

    openthread_uart_run();

    return 0;
}