/*
 * random_sensor.c
 *
 *  Created on: 6 Jun 2018
 *      Author: Dhruv Verma
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"
#include "periph_conf.h"
#include "log.h"
#include "mutex.h"
#include "thread.h"
#include "xtimer.h"
#include "sensor.h"
#include "net/gcoap.h"
#include "od.h"

#ifdef MODULE_HDC1000
#include "hdc1000.h"
#include "hdc1000_params.h"
static hdc1000_t dev_hdc1000;
#endif

#ifdef MODULE_TMP006
#include "tmp006.h"
static tmp006_t dev_tmp006;
#endif

#if !defined(MODULE_HDC1000) || !defined(MODULE_TMP006)
#include "random.h"
#endif

#define SENSOR_TIMEOUT_MS       (5000 * 1000)
#define SENSOR_NUM_SAMPLES      (10U)
#define SENSOR_THREAD_STACKSIZE (3 * THREAD_STACKSIZE_DEFAULT)

#define ADDR    "::1"
#define PORT    "5683"
#define PATH    "/lights"

static int16_t samples_humidity[SENSOR_NUM_SAMPLES];
static int16_t samples_temperature[SENSOR_NUM_SAMPLES];
static mutex_t mutex;

static char sensor_thread_stack[SENSOR_THREAD_STACKSIZE];

typedef struct {
    uint16_t refresh;
    uint16_t accuracy;
} SETTINGS;

typedef struct {
    uint16_t temp;
} TEMP;

SETTINGS value;
TEMP reading;

xtimer_t time;
int pid;
//static uint16_t req_count = 0;

/**
 * @brief get avg temperature over N samples in Celcius (C) with factor 100
 *
 * @return temperature
 */
uint16_t sensor_get_refresh(void){
    return value.refresh;
}

kernel_pid_t set_pid(void){
    return pid;
}

/**
 * @brief get avg humitity over N sampels in percent (%) with factor 100
 *
 * @return humidity
 */
uint16_t sensor_get_accuracy(void){
    return value.accuracy;
}

uint16_t sensor_get_temp(void){
    return reading.temp;
}

/**
 * @brief Measures the temperature and humitity with a HDC1000.
 *
 * @param[out] temp the measured temperature in degree celsius * 100
 * @param[out] hum the measured humitity in % * 100
 */

void sensor_set_refresh(uint16_t refresh){
    value.refresh = refresh;
    thread_wakeup(pid);
    xtimer_set_wakeup(&time, refresh * 1000000, thread_getpid());
    printf("Transmission rate reset: %u\n", value.refresh);
}

void sensor_set_accuracy(uint16_t accuracy){
    value.accuracy = accuracy;
    printf("Set accuracy: %u\n", value.accuracy);
}

static int16_t _get_humidity(void){
    int16_t h;
#ifdef MODULE_HDC1000
    int16_t t;
    LOG_DEBUG("[SENSOR] _hdc1000_measure\n");
    hdc1000_read(&dev_hdc1000, &t, &h);
#else
    h = (int16_t)random_uint32_range(0, 10000);
#endif /* MODULE_HDC1000 */
    return h;
}

/**
 * @brief Measures the temperature with a TMP006.
 *
 * @param[out] temp the measured temperature in degree celsius * 100
 */
static int16_t _get_temperature(void){
    LOG_DEBUG("[SENSOR] _get_temperature\n");
    int16_t t;
#ifdef MODULE_TMP006
    uint8_t drdy;
    int16_t raw_temp, raw_volt;
    float tamb, tobj;
    /* read sensor, quit on error */
    if (tmp006_read(&dev_tmp006, &raw_volt, &raw_temp, &drdy)) {
        LOG_ERROR("[SENSOR] tmp006_read failed\n");
        return 0;
    }
    tmp006_convert(raw_volt, raw_temp, &tamb, &tobj);
    t = (int16_t)(tobj*100);
#else
    t = (int16_t)random_uint32_range(0, 5000);
#endif /* MODULE_TMP006 */
    return t;
}

/**
 * @brief Intialise all sensores.
 *
 * @return 0 on success, anything else on error
 */
static int _init(void){
    LOG_DEBUG("[SENSOR] _init\n");
#ifdef MODULE_HDC1000
    /* initialise humidity sensor hdc1000 */
    if ((hdc1000_init(&dev_hdc1000, &hdc1000_params[0]) != 0)) {
        LOG_ERROR("[SENSOR] HDC1000 init");
        return 1;
    }
#endif /* MODULE_HDC1000 */
#ifdef MODULE_TMP006
    /* init temperature sensor tmp006 */
    if ((tmp006_init(&dev_tmp006, TMP006_I2C, TMP006_ADDR, TMP006_CONFIG_CR_DEF) != 0)) {
        LOG_ERROR("[SENSOR] TMP006 init");
        return 1;
    }
    if (tmp006_set_active(&dev_tmp006)) {
        LOG_ERROR("[SENSOR] TMP006 activate.");
        return 1;
    }
    if (tmp006_test(&dev_tmp006)) {
        LOG_ERROR("[SENSOR] TMP006 test.");
        return 1;
    }
#endif /* MODULE_TMP006 */
    mutex_lock(&mutex);
    for(unsigned i = 0; i < SENSOR_NUM_SAMPLES; i++) {
        samples_humidity[i] = _get_humidity();
        samples_temperature[i] = _get_temperature();
    }
    mutex_unlock(&mutex);
    return 0;
}

/* The commented part of the code after this is made taking examples from https://github.com/RIOT-Makers/climote/blob/master/lgv/coap.c
 * and is the correct way to communicate with the CoAP server
 */

//static void _resp_handler(unsigned req_state, coap_pkt_t* pdu,
//        sock_udp_ep_t *remote){
//    (void)remote; /* not interested in the source currently */
//
//    if(req_state == GCOAP_MEMO_TIMEOUT) {
//        printf("gcoap: timeout for msg ID %02u\n", coap_get_id(pdu));
//        return;
//    }
//    else if(req_state == GCOAP_MEMO_ERR) {
//        printf("gcoap: error in response\n");
//        return;
//    }
//
//    char *class_str =
//            (coap_get_code_class(pdu) == COAP_CLASS_SUCCESS) ?
//                    "Success" : "Error";
//    printf("gcoap: response %s, code %1u.%02u", class_str,
//            coap_get_code_class(pdu), coap_get_code_detail(pdu));
//    printf("\nPayload length: %u\n", pdu->payload_len);
//    if(pdu->payload_len) {
//        if(pdu->content_type == COAP_FORMAT_TEXT
//                || pdu->content_type == COAP_FORMAT_LINK
//                || coap_get_code_class(pdu) == COAP_CLASS_CLIENT_FAILURE
//                || coap_get_code_class(pdu) == COAP_CLASS_SERVER_FAILURE) {
//            /* Expecting diagnostic payload in failure cases */
//            printf(", %u bytes\n%.*s\n", pdu->payload_len, pdu->payload_len,
//                    (char *)pdu->payload);
//        }
//        else {
//            printf(", %u bytes\n", pdu->payload_len);
//            od_hex_dump(pdu->payload, pdu->payload_len, OD_WIDTH_DEFAULT);
//        }
//    }
//    else {
//        printf(", empty payload\n");
//    }
//}
//
//static size_t _send(uint8_t *buf, size_t len, char *addr_str, char *port_str){
//    ipv6_addr_t addr;
//    size_t bytes_sent;
//    sock_udp_ep_t remote;
//
//    remote.family = AF_INET6;
//    remote.netif = SOCK_ADDR_ANY_NETIF;
//
//    if(ipv6_addr_from_str(&addr, addr_str) == NULL) {
//        puts("gcoap_cli: unable to parse destination address");
//        return 0;
//    }
//    memcpy(&remote.addr.ipv6[0], &addr.u8[0], sizeof(addr.u8));
//
//    /* parse port */
//    remote.port = (uint16_t)atoi(port_str);
//    if(remote.port == 0) {
//        puts("gcoap_cli: unable to parse destination port");
//        return 0;
//    }
//
//    bytes_sent = gcoap_req_send2(buf, len, &remote, _resp_handler);
//    if(bytes_sent > 0) {
//        req_count++;
//    }
//    return bytes_sent;
//}

void send_values(void){

//    uint8_t buf[GCOAP_PDU_BUF_SIZE];
//    coap_pkt_t pdu;
//    size_t len;

    printf("Sending values\n");
    char payload[50];
    memset(payload, '\0', 50);
    size_t payload_len = sprintf(payload,
            "{\"type\": \"temp\", \"ID\": \"7070\", \"Message\": \"%u\"}",
            sensor_get_temp());
//    unsigned msg_type = COAP_TYPE_CON;
//
//    printf("Payload: %s\n", payload);
//
//    gcoap_req_init(&pdu, &buf[0], GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, PATH);
//    coap_hdr_set_type(pdu.hdr, msg_type);
//
//    memcpy(pdu.payload, payload, strlen(payload));
//    len = gcoap_finish(&pdu, strlen(payload), COAP_FORMAT_JSON);
//
//    size_t res = _send(&buf[0], len, ADDR, PORT);
//    printf("Res: %u\n", res);
//
//    if(!res) {
//        puts("gcoap_cli: msg send failed");
//    }

    /* Commenting due to testing */
    char *request = (char*)malloc(sizeof(char) * (50 + payload_len));
    sprintf(request, "coap-client -m post coap://[::1]:5683/lights -e '%s'",
            payload);
    printf("Request: %s", request);
    system(request);
    free(request);
}

/**
 * @brief udp receiver thread function
 *
 * @param[in] arg   unused
 */
static void *sensor_thread(void *arg){
    (void)arg;
    printf("Sensor thread starting\n");
    int count = 0;
    printf("PID sensor: %d\n", thread_getpid());
    while(1) {
        printf("Reading values\n");
        /* get latest sensor data */
        mutex_lock(&mutex);
        reading.temp = count;
        mutex_unlock(&mutex);

        printf("Value: %d\n", reading.temp);

        count = (count + 1) % 5000;
        uint16_t sensor_val = sensor_get_refresh();
        printf("Refresh rate set: %u seconds\n", sensor_val);
        if(sensor_val > 0) {
            xtimer_set_wakeup(&time, sensor_val * 1000000, thread_getpid());
            thread_sleep();
        }
        else if(sensor_val == 0) {
            //5 second default
            xtimer_set_wakeup(&time, SENSOR_TIMEOUT_MS, thread_getpid());
            thread_sleep();
        }
        else {
            break;
        }
        send_values();
    }
    return NULL;
}

/**
 * @brief start udp receiver thread
 *
 * @return PID of sensor control thread
 */
int sensor_init(void){
    /* init sensors */
    if(_init() != 0) {
        return -1;
    }
    /* start sensor thread for periodic measurements */
    pid = thread_create(sensor_thread_stack, sizeof(sensor_thread_stack),
    THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, sensor_thread, NULL,
            "sensor_thread");
    printf("PID: %d\n", pid);

    return pid;
}

