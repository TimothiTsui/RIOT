/*
 * Copyright (c) 2015-2016 Ken Bannister. All rights reserved.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       gcoap example
 *
 * @author      Ken Bannister <kb2ma@runbox.com>
 *
 * @}
 */

#include <stdio.h>
#include "msg.h"
#include "shell.h"

#include "net/gcoap.h"
#include "kernel_types.h"
#include "xtimer.h"
#include "sensor.h"

#define MAIN_QUEUE_SIZE (4)
//static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
//
//extern int gcoap_cli_cmd(int argc, char **argv);
extern void gcoap_cli_init(void);


//static const shell_command_t shell_commands[] = {
//    { "coap", "CoAP example", gcoap_cli_cmd },
//    { NULL, NULL, NULL }
//};


int main(void)
{
    /* for the thread running the shell */
//    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    gcoap_cli_init();
    puts("gcoap example app");
    int ret = sensor_init();
//    set_pid(ret);
    printf("Value returned: %d\n", ret);

    /* start shell */
//    puts("All up, running the shell now");
//    char line_buf[SHELL_DEFAULT_BUFSIZE];
//    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

//
//    while(1){
//
//
//        printf("main loop\n");
//        xtimer_usleep(10000000);
//    }

    /* should never be reached */


    return 0;
}
