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
 * @author      Baptiste Clenet <bapclenet@gmail.com>
 * @}
 */

#include <ctype.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "openthread/config.h"
#include "openthread/platform/logging.h"

/* adapted from OpenThread posix example:
 * See: https://github.com/openthread/openthread/blob/master/examples/platforms/posix/logging.c */
<<<<<<< HEAD
void otPlatLog(otLogLevel aLogLevel, otLogRegion aLogRegion,
        const char *aFormat, ...){
=======
__attribute__((__format__ (__printf__, 3, 4)))
void otPlatLog(otLogLevel aLogLevel, otLogRegion aLogRegion, const char *aFormat, ...)
{
    (void) aLogLevel;
    (void) aLogRegion;
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
    va_list args;
<<<<<<< HEAD

    (void) aLogLevel;
    (void) aLogRegion;


//    switch(aLogLevel) {
//    case OT_LOG_LEVEL_NONE:
//        fprintf(stderr, "NONE ");
//        break;
//
//    case OT_LOG_LEVEL_CRIT:
//        fprintf(stderr, "CRIT ");
//        break;
//
//    case OT_LOG_LEVEL_WARN:
//        fprintf(stderr, "WARN ");
//        break;
//
//    case OT_LOG_LEVEL_INFO:
//        fprintf(stderr, "INFO ");
//        break;
//
//    case OT_LOG_LEVEL_DEBG:
//        fprintf(stderr, "DEBG ");
//        break;
//    }
//
//    switch(aLogRegion) {
//    case OT_LOG_REGION_API:
//        fprintf(stderr, "API  ");
//        break;
//
//    case OT_LOG_REGION_MLE:
//        fprintf(stderr, "MLE  ");
//        break;
//
//    case OT_LOG_REGION_ARP:
//        fprintf(stderr, "ARP  ");
//        break;
//
//    case OT_LOG_REGION_NET_DATA:
//        fprintf(stderr, "NETD ");
//        break;
//
//    case OT_LOG_REGION_IP6:
//        fprintf(stderr, "IPV6 ");
//        break;
//
//    case OT_LOG_REGION_ICMP:
//        fprintf(stderr, "ICMP ");
//        break;
//
//    case OT_LOG_REGION_MAC:
//        fprintf(stderr, "MAC  ");
//        break;
//
//    case OT_LOG_REGION_MEM:
//        fprintf(stderr, "MEM  ");
//        break;
//
//    case OT_LOG_REGION_NCP:
//        fprintf(stderr, "NCP  ");
//        break;
//
//    case OT_LOG_REGION_MESH_COP:
//        fprintf(stderr, "MESH_COP  ");
//        break;
//
//    case OT_LOG_REGION_NET_DIAG:
//        fprintf(stderr, "NET_DIAG  ");
//        break;
//
//    case OT_LOG_REGION_PLATFORM:
//        fprintf(stderr, "PLATFORM  ");
//        break;
//
//    case OT_LOG_REGION_COAP:
//        fprintf(stderr, "COAP  ");
//        break;
//
//    case OT_LOG_REGION_CLI:
//        fprintf(stderr, "CLI  ");
//        break;
//
//    case OT_LOG_REGION_CORE:
//        fprintf(stderr, "CORE  ");
//        break;
//
//    case OT_LOG_REGION_UTIL:
//        fprintf(stderr, "UTIL  ");
//        break;
//
//    default:
//        break;
//    }

=======
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
    va_start(args, aFormat);
    vfprintf(stderr, aFormat, args);
    fprintf(stderr, "\n");
    va_end(args);
}
