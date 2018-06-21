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
 * @brief       Implementation of OpenThread radio platform abstraction
 *
 * @author      Jose Ignacio Alamos <jialamos@uc.cl>
 * @}
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "byteorder.h"
#include "errno.h"
#include "net/ethernet/hdr.h"
#include "net/ethertype.h"
#include "net/ieee802154.h"
#include "net/netdev/ieee802154.h"
#include "openthread/platform/radio.h"
#include "ot.h"

#define ENABLE_DEBUG (1)
#include "debug.h"

#define RADIO_IEEE802154_FCS_LEN    (2U)

static otRadioFrame sTransmitFrame;
static otRadioFrame sReceiveFrame;
static int8_t Rssi;
static uint8_t sChannel = 0;
static int16_t sTxPower = 0;

static otRadioState sState = OT_RADIO_STATE_DISABLED;
static bool sIsReceiverEnabled = false;

static netdev_t *_dev;

/* set 15.4 channel */
static int _set_channel(uint16_t channel)
{
    int res = -1;
    if(sChannel != channel)
    {
        res = _dev->driver->set(_dev, NETOPT_CHANNEL, &channel,
                sizeof(uint16_t));
        sChannel = channel;
    }
    return res;
}

/* set transmission power */
static int _set_power(int16_t power)
{
    int res = -1;
    if(sTxPower != power)
    {
        res = _dev->driver->set(_dev, NETOPT_TX_POWER, &power, sizeof(int16_t));
        sTxPower = power;
    }
    return res;
}

/* get transmission power */
static int16_t _get_power(void)
{
    int16_t power;
    _dev->driver->get(_dev, NETOPT_TX_POWER, &power, sizeof(int16_t));
    return power;
}

/* set IEEE802.15.4 PAN ID */
static int _set_panid(uint16_t panid)
{
    return _dev->driver->set(_dev, NETOPT_NID, &panid, sizeof(uint16_t));
}

/* set extended HW address */
static int _set_long_addr(uint8_t *ext_addr)
{
    return _dev->driver->set(_dev, NETOPT_ADDRESS_LONG, ext_addr,
    IEEE802154_LONG_ADDRESS_LEN);
}

/* set short address */
static int _set_addr(uint16_t addr)
{
    return _dev->driver->set(_dev, NETOPT_ADDRESS, &addr, sizeof(uint16_t));
}

/* check the state of promiscuous mode */
static netopt_enable_t _is_promiscuous(void)
{
    netopt_enable_t en;

    _dev->driver->get(_dev, NETOPT_PROMISCUOUSMODE, &en, sizeof(en));
    return en == NETOPT_ENABLE ? true : false;;
}

/* set the state of promiscuous mode */
static int _set_promiscuous(netopt_enable_t enable)
{
    return _dev->driver->set(_dev, NETOPT_PROMISCUOUSMODE, &enable,
            sizeof(enable));
}

/* wrapper for setting device state */
static void _set_state(netopt_state_t state)
{
    _dev->driver->set(_dev, NETOPT_STATE, &state, sizeof(netopt_state_t));
}

/* wrapper for getting device state */
static netopt_state_t _get_state(void)
{
    netopt_state_t state;
    _dev->driver->get(_dev, NETOPT_STATE, &state, sizeof(netopt_state_t));
    return state;
}

/* sets device state to SLEEP/OFF (radio disabled) */
static void _set_sleep(void)
{
    if(sIsReceiverEnabled)
    {
        _set_state(NETOPT_STATE_SLEEP);
        sIsReceiverEnabled = false;
    }
}

/* set device state to IDLE (radio enabled) */
static void _set_idle(void)
{
    if(!sIsReceiverEnabled)
    {
        _set_state(NETOPT_STATE_IDLE);
        sIsReceiverEnabled = true;
    }
}

/* init framebuffers and initial state */
void openthread_radio_init(netdev_t *dev, uint8_t *tb, uint8_t *rb)
{
    sTransmitFrame.mLength = 0;
    sTransmitFrame.mPsdu = tb;
    sReceiveFrame.mLength = 0;
    sReceiveFrame.mPsdu = rb;
    _dev = dev;
    netopt_state_t state = _get_state();
    if(state != NETOPT_STATE_OFF || state != NETOPT_STATE_SLEEP)
    {
        _set_sleep();
    }
}

void radio_process(otInstance *aInstance)
{
    (void) aInstance;
}

/* Called upon NETDEV_EVENT_RX_COMPLETE event */
void recv_pkt(otInstance *aInstance, netdev_t *dev)
{
    DEBUG("%s(): \n", __FUNCTION__);
    netdev_ieee802154_rx_info_t rx_info;

    /* Read frame length from driver */
    int len = dev->driver->recv(dev, NULL, 0, NULL);

    /* very unlikely */
    if((len < 0) || ((uint32_t)len > UINT16_MAX))
    {
        DEBUG("Invalid len: %d\n", len);
        otPlatRadioReceiveDone(aInstance, NULL, OT_ERROR_ABORT);
        return;
    }

    /* Fill OpenThread receive frame */
    /* Openthread needs a packet length with FCS included,
     * OpenThread do not use the data so we don't need to calculate FCS */
    sReceiveFrame.mLength = len + RADIO_IEEE802154_FCS_LEN;

    /* Read received frame */
    int res = dev->driver->recv(dev, (char *)sReceiveFrame.mPsdu, len,
            &rx_info);

    /* Get RSSI from a radio driver. RSSI should be in [dBm] */
    Rssi = (int8_t)rx_info.rssi;
    sReceiveFrame.mInfo.mRxInfo.mRssi = Rssi;

    DEBUG("Received message: len %d\n", (int ) sReceiveFrame.mLength);
    for(int i = 0; i < sReceiveFrame.mLength; ++i)
    {
        DEBUG("%x ", sReceiveFrame.mPsdu[i]);
    }
    DEBUG("\n");

    /* Tell OpenThread that receive has finished */
    otPlatRadioReceiveDone(aInstance, &sReceiveFrame,
            res > 0 ? OT_ERROR_NONE : OT_ERROR_ABORT);
}

/* Called upon TX event */
void send_pkt(otInstance *aInstance, netdev_t *dev, netdev_event_t event)
{
    (void)dev;

    DEBUG("openthread: send_pkt()\n");

    /* Tell OpenThread transmission is done depending on the NETDEV event */
    switch(event)
    {
    case NETDEV_EVENT_TX_COMPLETE:
        DEBUG("openthread: NETDEV_EVENT_TX_COMPLETE\n");
        otPlatRadioTxDone(aInstance, &sTransmitFrame, NULL, OT_ERROR_NONE);
        break;
    case NETDEV_EVENT_TX_COMPLETE_DATA_PENDING:
        DEBUG("openthread: NETDEV_EVENT_TX_COMPLETE_DATA_PENDING\n");
        otPlatRadioTxDone(aInstance, &sTransmitFrame, &sReceiveFrame,
                OT_ERROR_NONE);
        break;
    case NETDEV_EVENT_TX_NOACK:
        DEBUG("openthread: NETDEV_EVENT_TX_NOACK\n");
        otPlatRadioTxDone(aInstance, &sTransmitFrame, NULL, OT_ERROR_NO_ACK);
        break;
    case NETDEV_EVENT_TX_MEDIUM_BUSY:
        DEBUG("openthread: NETDEV_EVENT_TX_MEDIUM_BUSY\n");
        otPlatRadioTxDone(aInstance, &sTransmitFrame, NULL,
                OT_ERROR_CHANNEL_ACCESS_FAILURE);
        break;
    default:
        break;
    }
}

/* OpenThread will call this for setting PAN ID */
void otPlatRadioSetPanId(otInstance *aInstance, uint16_t panid)
{
    (void)aInstance;
    DEBUG("openthread: otPlatRadioSetPanId: setting PAN ID to %04x\n", panid);
    _set_panid(panid);
}

/* OpenThread will call this for setting extended address */
void otPlatRadioSetExtendedAddress(otInstance *aInstance,
        const otExtAddress *aExtendedAddress)
{
    (void)aInstance;

    DEBUG("openthread: otPlatRadioSetExtendedAddress\n");
    uint8_t reversed_addr[IEEE802154_LONG_ADDRESS_LEN];
    for(unsigned i = 0; i < IEEE802154_LONG_ADDRESS_LEN; i++)
    {
        reversed_addr[i] = aExtendedAddress->m8[IEEE802154_LONG_ADDRESS_LEN - 1
                - i];
    }
    _set_long_addr(reversed_addr);
}

/* OpenThread will call this for setting short address */
void otPlatRadioSetShortAddress(otInstance *aInstance, uint16_t aShortAddress)
{
    (void)aInstance;
    DEBUG("openthread: otPlatRadioSetShortAddress: setting address to %04x\n",
            aShortAddress);
    _set_addr(((aShortAddress & 0xff) << 8) | ((aShortAddress >> 8) & 0xff));
}

/* OpenThread will call this for enabling the radio */
otError otPlatRadioEnable(otInstance *aInstance)
{
    if(!otPlatRadioIsEnabled(aInstance))
    {
        DEBUG("%s: State=OT_RADIO_STATE_SLEEP\n", __FUNCTION__);
        sState = OT_RADIO_STATE_SLEEP;
    }

    return OT_ERROR_NONE;
}

/* OpenThread will call this for disabling the radio */
otError otPlatRadioDisable(otInstance *aInstance)
{
    if(otPlatRadioIsEnabled(aInstance))
    {
        DEBUG("%s: State=OT_RADIO_STATE_DISABLED\n", __FUNCTION__);
        sState = OT_RADIO_STATE_DISABLED;
    }

    return OT_ERROR_NONE;
}

bool otPlatRadioIsEnabled(otInstance *aInstance)
{
    DEBUG("%s\n", __FUNCTION__);
    (void)aInstance;
    return (sState != OT_RADIO_STATE_DISABLED) ? true : false;
}

/* OpenThread will call this for setting device state to SLEEP */
otError otPlatRadioSleep(otInstance *aInstance)
{
    otError error = OT_ERROR_INVALID_STATE;
    (void)aInstance;

    if(sState == OT_RADIO_STATE_SLEEP || sState == OT_RADIO_STATE_RECEIVE)
    {
        DEBUG("%s: State=OT_RADIO_STATE_SLEEP\n", __FUNCTION__);
        error = OT_ERROR_NONE;
        sState = OT_RADIO_STATE_SLEEP;
        _set_sleep();
    }
    return error;
}

/*OpenThread will call this for waiting the reception of a packet */
otError otPlatRadioReceive(otInstance *aInstance, uint8_t aChannel)
{
    otError error = OT_ERROR_INVALID_STATE;
    (void)aInstance;

    if(sState != OT_RADIO_STATE_DISABLED)
    {
        DEBUG("%s(channel %u): State=OT_RADIO_STATE_RECEIVE\n", __FUNCTION__,
                aChannel);

        error = OT_ERROR_NONE;
        sState = OT_RADIO_STATE_RECEIVE;
        _set_channel(aChannel);
        sReceiveFrame.mChannel = aChannel;
        _set_idle();
    }

    return error;
}

/* OpenThread will call this function to get the transmit buffer */
otRadioFrame *otPlatRadioGetTransmitBuffer(otInstance *aInstance)
{
    (void)aInstance;
    DEBUG("%s\n", __FUNCTION__);
    return &sTransmitFrame;
}

/* OpenThread will call this function to set the transmit power */
otError otPlatRadioSetTransmitPower(otInstance *aInstance, int8_t aPower)
{
    (void)aInstance;
    DEBUG("openthread: otPlatRadioSetTransmitPower\n");
    _set_power(aPower);

    return OT_ERROR_NONE;
}

otError otPlatRadioGetTransmitPower(otInstance *aInstance, int8_t *aPower)
{
    (void)aInstance;
    DEBUG("openthread: otPlatRadioGetTransmitPower\n");
    *aPower = _get_power();

    return OT_ERROR_NONE;
}

/* OpenThread will call this for transmitting a packet*/
otError otPlatRadioTransmit(otInstance *aInstance, otRadioFrame *aFrame)
{
    otError error = OT_ERROR_INVALID_STATE;
    (void)aInstance;

    /* Populate iolist with transmit data
     * Unlike RIOT, OpenThread includes two bytes FCS (0x00 0x00) so
     * these bytes are removed
     */
    iolist_t iolist =
    {.iol_base = aFrame->mPsdu, .iol_len = (aFrame->mLength
            - RADIO_IEEE802154_FCS_LEN)};

    if(sState == OT_RADIO_STATE_RECEIVE)
    {
        error = OT_ERROR_NONE;
        sState = OT_RADIO_STATE_TRANSMIT;
        //sTransmitError = OT_ERROR_NONE;
    }

    /*Set channel and power based on transmit frame */
    DEBUG("otPlatRadioTransmit->channel: %i, length %d\n",
            (int ) aFrame->mChannel, (int )aFrame->mLength);
    for(int i = 0; i < aFrame->mLength; ++i)
    {
        DEBUG("%x ", aFrame->mPsdu[i]);
    }
    DEBUG("\n");

    _set_channel(aFrame->mChannel);
    _set_power(aFrame->mInfo.mRxInfo.mRssi);

    otPlatRadioTxStarted(aInstance, aFrame);
    /* send packet though netdev */
    _dev->driver->send(_dev, &iolist);
    otPlatRadioTxDone(aInstance, &sTransmitFrame, &sReceiveFrame, OT_ERROR_NONE);

    return error;
}

/* OpenThread will call this for getting the radio caps */
otRadioCaps otPlatRadioGetCaps(otInstance *aInstance)
{
    (void)aInstance;
    DEBUG("openthread: otPlatRadioGetCaps\n");
    /* all drivers should handle ACK, including call of NETDEV_EVENT_TX_NOACK */
    return OT_RADIO_CAPS_NONE;
}

/* OpenThread will call this for getting the state of promiscuous mode */
bool otPlatRadioGetPromiscuous(otInstance *aInstance)
{
    (void)aInstance;
    DEBUG("openthread: otPlatRadioGetPromiscuous\n");
    return _is_promiscuous();
}

/* OpenThread will call this for setting the state of promiscuous mode */
void otPlatRadioSetPromiscuous(otInstance *aInstance, bool aEnable)
{
    (void)aInstance;
    DEBUG("openthread: otPlatRadioSetPromiscuous\n");
    _set_promiscuous((aEnable) ? NETOPT_ENABLE : NETOPT_DISABLE);
}

int8_t otPlatRadioGetRssi(otInstance *aInstance)
{
    DEBUG("otPlatRadioGetRssi\n");
    (void)aInstance;
    return Rssi;
}

void otPlatRadioEnableSrcMatch(otInstance *aInstance, bool aEnable)
{
    DEBUG("otPlatRadioEnableSrcMatch\n");
    (void)aInstance;
    (void)aEnable;
}

otError otPlatRadioAddSrcMatchShortEntry(otInstance *aInstance,
        const uint16_t aShortAddress)
{
    DEBUG("otPlatRadioAddSrcMatchShortEntry\n");
    (void)aInstance;
    (void)aShortAddress;
    return OT_ERROR_NONE;
}

otError otPlatRadioAddSrcMatchExtEntry(otInstance *aInstance,
        const otExtAddress *aExtAddress)
{
    DEBUG("otPlatRadioAddSrcMatchExtEntry\n");
    (void)aInstance;
    (void)aExtAddress;
    return OT_ERROR_NONE;
}

otError otPlatRadioClearSrcMatchShortEntry(otInstance *aInstance,
        const uint16_t aShortAddress)
{
    DEBUG("otPlatRadioClearSrcMatchShortEntry\n");
    (void)aInstance;
    (void)aShortAddress;
    return OT_ERROR_NONE;
}

otError otPlatRadioClearSrcMatchExtEntry(otInstance *aInstance,
        const otExtAddress *aExtAddress)
{
    DEBUG("otPlatRadioClearSrcMatchExtEntry\n");
    (void)aInstance;
    (void)aExtAddress;
    return OT_ERROR_NONE;
}

void otPlatRadioClearSrcMatchShortEntries(otInstance *aInstance)
{
    DEBUG("otPlatRadioClearSrcMatchShortEntries\n");
    (void)aInstance;
}

void otPlatRadioClearSrcMatchExtEntries(otInstance *aInstance)
{
    DEBUG("otPlatRadioClearSrcMatchExtEntries\n");
    (void)aInstance;
}

otError otPlatRadioEnergyScan(otInstance *aInstance, uint8_t aScanChannel,
        uint16_t aScanDuration)
{
    DEBUG("otPlatRadioEnergyScan\n");
    (void)aInstance;
    (void)aScanChannel;
    (void)aScanDuration;
    return OT_ERROR_NOT_IMPLEMENTED;
}

void otPlatRadioGetIeeeEui64(otInstance *aInstance, uint8_t *aIeee64Eui64)
{
    (void)aInstance;
    _dev->driver->get(_dev, NETOPT_IPV6_IID, aIeee64Eui64, sizeof(eui64_t));
}

int8_t otPlatRadioGetReceiveSensitivity(otInstance *aInstance)
{
    (void)aInstance;
    return -100;
}
