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
 * @brief       Implementation of OpenThread logging platform abstraction
 *
 * @author      Jose Ignacio Alamos <jialamos@uc.cl>
 * @}
 */

#include <ctype.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "openthread/platform/logging.h"

/* adapted from OpenThread posix example:
 * See: https://github.com/openthread/openthread/blob/master/examples/platforms/posix/logging.c */
void otPlatLog(otLogLevel aLogLevel, otLogRegion aLogRegion,
        const char *aFormat, ...){
    va_list args;

    switch(aLogLevel) {
    case OT_LOG_LEVEL_NONE:
        fprintf(stderr, "NONE ");
        break;

    case OT_LOG_LEVEL_CRIT:
        fprintf(stderr, "CRIT ");
        break;

    case OT_LOG_LEVEL_WARN:
        fprintf(stderr, "WARN ");
        break;

    case OT_LOG_LEVEL_INFO:
        fprintf(stderr, "INFO ");
        break;

    case OT_LOG_LEVEL_DEBG:
        fprintf(stderr, "DEBG ");
        break;
    }

    switch(aLogRegion) {
    case OT_LOG_REGION_API:
        fprintf(stderr, "API  ");
        break;

    case OT_LOG_REGION_MLE:
        fprintf(stderr, "MLE  ");
        break;

    case OT_LOG_REGION_ARP:
        fprintf(stderr, "ARP  ");
        break;

    case OT_LOG_REGION_NET_DATA:
        fprintf(stderr, "NETD ");
        break;

    case OT_LOG_REGION_IP6:
        fprintf(stderr, "IPV6 ");
        break;

    case OT_LOG_REGION_ICMP:
        fprintf(stderr, "ICMP ");
        break;

    case OT_LOG_REGION_MAC:
        fprintf(stderr, "MAC  ");
        break;

    case OT_LOG_REGION_MEM:
        fprintf(stderr, "MEM  ");
        break;

    case OT_LOG_REGION_NCP:
        fprintf(stderr, "NCP  ");
        break;

    case OT_LOG_REGION_MESH_COP:
        fprintf(stderr, "MESH_COP  ");
        break;

    case OT_LOG_REGION_NET_DIAG:
        fprintf(stderr, "NET_DIAG  ");
        break;

    case OT_LOG_REGION_PLATFORM:
        fprintf(stderr, "PLATFORM  ");
        break;

    case OT_LOG_REGION_COAP:
        fprintf(stderr, "COAP  ");
        break;

    case OT_LOG_REGION_CLI:
        fprintf(stderr, "CLI  ");
        break;

    case OT_LOG_REGION_CORE:
        fprintf(stderr, "CORE  ");
        break;

    case OT_LOG_REGION_UTIL:
        fprintf(stderr, "UTIL  ");
        break;

    default:
        break;
    }

    va_start(args, aFormat);
    vfprintf(stderr, aFormat, args);
    fprintf(stderr, "\r");
    va_end(args);
}
