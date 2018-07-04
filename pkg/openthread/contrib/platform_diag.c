/*
 * Copyright (C) 2017 Fundacion Inria Chile
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @{
 * @ingroup     net
 * @file
 * @brief       Implementation of OpenThread diagnostics platform abstraction
 *
 * @author      Jose Ignacio Alamos <jialamos@uc.cl>
 * @}
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "openthread/platform/diag.h"

static bool sDiagMode = false;

void otPlatDiagProcess(otInstance *aInstance, int argc, char *argv[],
        char *aOutput, size_t aOutputMaxLen){
    // Add more plarform specific diagnostics features here.
    snprintf(aOutput, aOutputMaxLen, "diag feature '%s' is not supported\r\n",
            argv[0]);

    (void)argc;
    (void)aInstance;
}

//void otPlatDiagProcess(int argc, char *argv[], char *aOutput, size_t aOutputMaxLen)
//{
//    /* add more plarform specific diagnostics features here */
//    (void)argc;
//    (void)argv;
//    (void)aOutput;
//    (void)aOutputMaxLen;
//
//}

void otPlatDiagModeSet(bool aMode){
    sDiagMode = aMode;
}

bool otPlatDiagModeGet(void){
    return sDiagMode;
}

void otPlatDiagChannelSet(uint8_t aChannel){
    (void)aChannel;
}

void otPlatDiagTxPowerSet(int8_t aTxPower){
    (void)aTxPower;
}

void otPlatDiagRadioReceived(otInstance *aInstance, otRadioFrame *aFrame,
        otError aError){
    (void)aInstance;
    (void)aFrame;
    (void)aError;
}

void otPlatDiagAlarmCallback(otInstance *aInstance){
    (void)aInstance;
}
