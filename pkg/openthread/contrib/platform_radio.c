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
#include "openthread/config.h"
#include "openthread/openthread.h"
#include "openthread/platform/diag.h"
#include "openthread/platform/radio.h"
#include "ot.h"

#define ENABLE_DEBUG (0)
#include "debug.h"

#define RADIO_IEEE802154_FCS_LEN    (2U)

<<<<<<< HEAD
#define RFCORE_XREG_FRMFILT0_FRAME_FILTER_EN    0x00000001  // Enables frame filtering

static otRadioFrame sTransmitFrame;
static otRadioFrame sReceiveFrame;
static otError sTransmitError;
static otError sReceiveError;

=======
static otRadioFrame sTransmitFrame;
static otRadioFrame sReceiveFrame;
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
static int8_t Rssi;
static uint8_t sChannel = 0;
static int16_t sTxPower = 0;

enum
{
    IEEE802154_MIN_LENGTH = 5,
    IEEE802154_MAX_LENGTH = 127,
    IEEE802154_ACK_LENGTH = 5,
    IEEE802154_FRAME_TYPE_MASK = 0x7,
    IEEE802154_FRAME_TYPE_ACK = 0x2,
    IEEE802154_FRAME_PENDING = 1 << 4,
    IEEE802154_ACK_REQUEST = 1 << 5,
    IEEE802154_DSN_OFFSET = 2,
};

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

static int _get_power(void)
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

<<<<<<< HEAD
/* sets device state to SLEEP/OFF (radio disabled) */
=======
static void _set_off(void)
{
    _set_state(NETOPT_STATE_OFF);
}

/* sets device state to SLEEP */
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
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
    dev->driver->recv(dev, (char *)sReceiveFrame.mPsdu, len, &rx_info);

    /* Get RSSI from a radio driver. RSSI should be in [dBm] */
    Rssi = (int8_t)rx_info.rssi;
<<<<<<< HEAD
    sReceiveFrame.mInfo.mRxInfo.mRssi = Rssi;
    /* Get LQI from a radio driver. LQI should be in [....] */
    sReceiveFrame.mInfo.mRxInfo.mLqi = (int8_t)rx_info.lqi;

    DEBUG("Received message: len %d\n", (int ) sReceiveFrame.mLength);
    for(int i = 0; i < sReceiveFrame.mLength; ++i)
    {
        DEBUG("%x ", sReceiveFrame.mPsdu[i]);
=======
    if (ENABLE_DEBUG) {
        DEBUG("Received message: len %d\n", (int) sReceiveFrame.mLength);
        for (int i = 0; i < sReceiveFrame.mLength; ++i) {
            DEBUG("%x ", sReceiveFrame.mPsdu[i]);
        }
        DEBUG("\n");
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
    }

<<<<<<< HEAD
//    /* Tell OpenThread that receive has finished */
//    otPlatRadioReceiveDone(aInstance, &sReceiveFrame,
//            res > 0 ? OT_ERROR_NONE : OT_ERROR_ABORT);
=======
    /* Tell OpenThread that receive has finished */
    otPlatRadioReceiveDone(aInstance, res > 0 ? &sReceiveFrame : NULL, res > 0 ? OT_ERROR_NONE : OT_ERROR_ABORT);
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
}

/* Called upon TX event */
void send_pkt(otInstance *aInstance, netdev_t *dev, netdev_event_t event)
{
    (void)dev;

    DEBUG("openthread: send_pkt()\n");

    /* Tell OpenThread transmission is done depending on the NETDEV event */
<<<<<<< HEAD
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
=======
    switch (event) {
        case NETDEV_EVENT_TX_COMPLETE:
            DEBUG("openthread: NETDEV_EVENT_TX_COMPLETE\n");
            otPlatRadioTxDone(aInstance, &sTransmitFrame, NULL, OT_ERROR_NONE);
            break;
        case NETDEV_EVENT_TX_COMPLETE_DATA_PENDING:
            DEBUG("openthread: NETDEV_EVENT_TX_COMPLETE_DATA_PENDING\n");
            otPlatRadioTxDone(aInstance, &sTransmitFrame, NULL, OT_ERROR_NONE);
            break;
        case NETDEV_EVENT_TX_NOACK:
            DEBUG("openthread: NETDEV_EVENT_TX_NOACK\n");
            otPlatRadioTxDone(aInstance, &sTransmitFrame, NULL, OT_ERROR_NO_ACK);
            break;
        case NETDEV_EVENT_TX_MEDIUM_BUSY:
            DEBUG("openthread: NETDEV_EVENT_TX_MEDIUM_BUSY\n");
            otPlatRadioTxDone(aInstance, &sTransmitFrame, NULL, OT_ERROR_CHANNEL_ACCESS_FAILURE);
            break;
        default:
            break;
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
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
<<<<<<< HEAD
    uint8_t reversed_addr[IEEE802154_LONG_ADDRESS_LEN];
    for(unsigned i = 0; i < IEEE802154_LONG_ADDRESS_LEN; i++)
    {
        reversed_addr[i] = aExtendedAddress->m8[IEEE802154_LONG_ADDRESS_LEN - 1
                - i];
=======
    char reversed_addr[IEEE802154_LONG_ADDRESS_LEN];
    for (unsigned i = 0; i < IEEE802154_LONG_ADDRESS_LEN; i++) {
        reversed_addr[i] = (uint8_t) ((uint8_t *)aExtendedAddress)[IEEE802154_LONG_ADDRESS_LEN - 1 - i];
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
    }
    if (ENABLE_DEBUG) {
        for (unsigned i = 0; i < IEEE802154_LONG_ADDRESS_LEN; ++i) {
            DEBUG("%x ", (uint8_t) ((uint8_t *)reversed_addr)[i]);
        }
        DEBUG("\n");
    }
    _set_long_addr((uint8_t*) reversed_addr);
}

/* OpenThread will call this for setting short address */
void otPlatRadioSetShortAddress(otInstance *aInstance, uint16_t aShortAddress)
{
    (void)aInstance;
<<<<<<< HEAD
    DEBUG("openthread: otPlatRadioSetShortAddress: setting address to %04x\n",
            aShortAddress);
=======
    DEBUG("openthread: otPlatRadioSetShortAddress: setting address to %04x\n", aShortAddress);
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
    _set_addr(((aShortAddress & 0xff) << 8) | ((aShortAddress >> 8) & 0xff));
}

/* OpenThread will call this for enabling the radio */
otError otPlatRadioEnable(otInstance *aInstance)
{
<<<<<<< HEAD
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
=======
    DEBUG("openthread: otPlatRadioEnable\n");
    (void)aInstance;

    if (!otPlatRadioIsEnabled(aInstance)) {
        _set_sleep();
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
    }

    return OT_ERROR_NONE;
<<<<<<< HEAD
=======
}

/* OpenThread will call this for disabling the radio */
otError otPlatRadioDisable(otInstance *aInstance)
{
    DEBUG("openthread: otPlatRadioDisable\n");
    (void)aInstance;

    if (otPlatRadioIsEnabled(aInstance)) {
        _set_off();
    }

    return OT_ERROR_NONE;
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
}

bool otPlatRadioIsEnabled(otInstance *aInstance)
{
<<<<<<< HEAD
    DEBUG("%s\n", __FUNCTION__);
    (void)aInstance;
    return (sState != OT_RADIO_STATE_DISABLED) ? true : false;
=======
    DEBUG("otPlatRadioIsEnabled\n");
    (void)aInstance;
    netopt_state_t state = _get_state();
    if (state == NETOPT_STATE_OFF) {
        return false;
    } else {
        return true;
    }
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
}

/* OpenThread will call this for setting device state to SLEEP */
otError otPlatRadioSleep(otInstance *aInstance)
{
<<<<<<< HEAD
    otError error = OT_ERROR_INVALID_STATE;
=======
    DEBUG("otPlatRadioSleep\n");
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
    (void)aInstance;

<<<<<<< HEAD
    if(sState == OT_RADIO_STATE_SLEEP || sState == OT_RADIO_STATE_RECEIVE)
    {
        DEBUG("%s: State=OT_RADIO_STATE_SLEEP\n", __FUNCTION__);
        error = OT_ERROR_NONE;
        sState = OT_RADIO_STATE_SLEEP;
        _set_sleep();
    }
    return error;
=======
    _set_sleep();
    return OT_ERROR_NONE;
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
}

/*OpenThread will call this for waiting the reception of a packet */
otError otPlatRadioReceive(otInstance *aInstance, uint8_t aChannel)
{
<<<<<<< HEAD
    otError error = OT_ERROR_INVALID_STATE;
=======
    DEBUG("openthread: otPlatRadioReceive. Channel: %i\n", aChannel);
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
    (void)aInstance;

<<<<<<< HEAD
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
=======
    _set_idle();
    _set_channel(aChannel);
    sReceiveFrame.mChannel = aChannel;
    return OT_ERROR_NONE;
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
}

/* OpenThread will call this function to get the transmit buffer */
otRadioFrame *otPlatRadioGetTransmitBuffer(otInstance *aInstance)
{
    (void)aInstance;
<<<<<<< HEAD
    DEBUG("%s\n", __FUNCTION__);
=======
    DEBUG("openthread: otPlatRadioGetTransmitBuffer\n");
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
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

otError otPlatRadioGetTransmitPower(otInstance *aInstance, int8_t *aPower)
{
    (void)aInstance;
    if (aPower == NULL) {
        return OT_ERROR_INVALID_ARGS;
    }

    *aPower = _get_power();

    return OT_ERROR_NONE;
}

otError otPlatRadioSetTransmitPower(otInstance *aInstance, int8_t aPower)
{
    (void)aInstance;
    _set_power(aPower);

    return OT_ERROR_NONE;
}

/* OpenThread will call this for transmitting a packet*/
<<<<<<< HEAD
otError otPlatRadioTransmit(otInstance *aInstance, otRadioFrame *aFrame)
=======
otError otPlatRadioTransmit(otInstance *aInstance, otRadioFrame *aPacket)
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
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

<<<<<<< HEAD
    if(sState == OT_RADIO_STATE_RECEIVE)
    {
        error = OT_ERROR_NONE;
        sState = OT_RADIO_STATE_TRANSMIT;
        sTransmitError = OT_ERROR_NONE;

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
//        otLogDebgPlat(aInstance, "Transmitted %d bytes", aFrame->mLength);

//        if(sState == OT_RADIO_STATE_TRANSMIT)
//        {
//            if((sTransmitFrame.mPsdu[0] & IEEE802154_ACK_REQUEST) == 0)
//            {
//                sState = OT_RADIO_STATE_RECEIVE;
//                otPlatRadioTxDone(aInstance, &sTransmitFrame, NULL,
//                        OT_ERROR_NONE);
//            }
//            else if(sReceiveFrame.mLength == IEEE802154_ACK_LENGTH
//                    && (sReceiveFrame.mPsdu[0] & IEEE802154_FRAME_TYPE_MASK)
//                            == IEEE802154_FRAME_TYPE_ACK
//                    && (sReceiveFrame.mPsdu[IEEE802154_DSN_OFFSET]
//                            == sTransmitFrame.mPsdu[IEEE802154_DSN_OFFSET]))
//            {
//                sState = OT_RADIO_STATE_RECEIVE;
//                otPlatRadioTxDone(aInstance, &sTransmitFrame, &sReceiveFrame,
//                        OT_ERROR_NONE);
//            }
//        }
//        sReceiveFrame.mLength = 0;
=======
    /*Set channel and power based on transmit frame */
    if (ENABLE_DEBUG) {
        DEBUG("otPlatRadioTransmit->channel: %i, length %d\n",
              (int) aPacket->mChannel, (int)aPacket->mLength);
        for (size_t i = 0; i < aPacket->mLength; ++i) {
            DEBUG("%x ", aPacket->mPsdu[i]);
        }
        DEBUG("\n");
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
    }
<<<<<<< HEAD
    return error;
}
=======
    _set_channel(aPacket->mChannel);
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git

<<<<<<< HEAD
void radio_process(otInstance *aInstance)
{
    if((sState == OT_RADIO_STATE_RECEIVE && sReceiveFrame.mLength > 0)
            || (sState == OT_RADIO_STATE_TRANSMIT
                    && sReceiveFrame.mLength > IEEE802154_ACK_LENGTH))
    {
#if OPENTHREAD_ENABLE_DIAG
=======
    /* send packet though netdev */
    _dev->driver->send(_dev, &iolist);
    otPlatRadioTxStarted(aInstance, aPacket);
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git

<<<<<<< HEAD
        if (otPlatDiagModeGet())
        {
            otPlatDiagRadioReceiveDone(aInstance, &sReceiveFrame, sReceiveError);
        }
        else
#endif
        {
            // signal MAC layer for each received frame if promiscous is enabled
            // otherwise only signal MAC layer for non-ACK frame
            if(((RFCORE_XREG_FRMFILT0 & RFCORE_XREG_FRMFILT0_FRAME_FILTER_EN)
                    == 0) || (sReceiveFrame.mLength > IEEE802154_ACK_LENGTH))
            {
//                otLogDebgPlat(sInstance, "Received %d bytes",
//                        sReceiveFrame.mLength);
                otPlatRadioReceiveDone(aInstance, &sReceiveFrame,
                        sReceiveError);
            }
        }
    }

    if(sState == OT_RADIO_STATE_TRANSMIT)
    {
        if(sTransmitError != OT_ERROR_NONE
                || (sTransmitFrame.mPsdu[0] & IEEE802154_ACK_REQUEST) == 0)
        {
//            if(sTransmitError != OT_ERROR_NONE)
//            {
//                otLogDebgPlat(sInstance, "Transmit failed ErrorCode=%d",
//                        sTransmitError);
//            }

            sState = OT_RADIO_STATE_RECEIVE;

#if OPENTHREAD_ENABLE_DIAG

            if (otPlatDiagModeGet())
            {
                otPlatDiagRadioTransmitDone(aInstance, &sTransmitFrame, sTransmitError);
            }
            else
#endif
            {
                otPlatRadioTxDone(aInstance, &sTransmitFrame, NULL,
                        sTransmitError);
            }
        }
        else if(sReceiveFrame.mLength == IEEE802154_ACK_LENGTH
                && (sReceiveFrame.mPsdu[0] & IEEE802154_FRAME_TYPE_MASK)
                        == IEEE802154_FRAME_TYPE_ACK
                && (sReceiveFrame.mPsdu[IEEE802154_DSN_OFFSET]
                        == sTransmitFrame.mPsdu[IEEE802154_DSN_OFFSET]))
        {
            sState = OT_RADIO_STATE_RECEIVE;

            otPlatRadioTxDone(aInstance, &sTransmitFrame, &sReceiveFrame,
                    sTransmitError);
        }
    }

    sReceiveFrame.mLength = 0;
=======
    return OT_ERROR_NONE;
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
}

/* OpenThread will call this for getting the radio caps */
otRadioCaps otPlatRadioGetCaps(otInstance *aInstance)
{
    (void)aInstance;
    DEBUG("openthread: otPlatRadioGetCaps\n");
    /* all drivers should handle ACK, including call of NETDEV_EVENT_TX_NOACK */
<<<<<<< HEAD
    return OT_RADIO_CAPS_NONE;
=======
    return OT_RADIO_CAPS_TRANSMIT_RETRIES | OT_RADIO_CAPS_ACK_TIMEOUT;
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
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

<<<<<<< HEAD
otError otPlatRadioAddSrcMatchShortEntry(otInstance *aInstance,
        const uint16_t aShortAddress)
=======
otError otPlatRadioAddSrcMatchShortEntry(otInstance *aInstance, const uint16_t aShortAddress)
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
{
    DEBUG("otPlatRadioAddSrcMatchShortEntry\n");
    (void)aInstance;
    (void)aShortAddress;
    return OT_ERROR_NONE;
}

<<<<<<< HEAD
otError otPlatRadioAddSrcMatchExtEntry(otInstance *aInstance,
        const otExtAddress *aExtAddress)
=======
otError otPlatRadioAddSrcMatchExtEntry(otInstance *aInstance, const uint8_t *aExtAddress)
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
{
    DEBUG("otPlatRadioAddSrcMatchExtEntry\n");
    (void)aInstance;
    (void)aExtAddress;
    return OT_ERROR_NONE;
}

<<<<<<< HEAD
otError otPlatRadioClearSrcMatchShortEntry(otInstance *aInstance,
        const uint16_t aShortAddress)
=======
otError otPlatRadioClearSrcMatchShortEntry(otInstance *aInstance, const uint16_t aShortAddress)
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
{
    DEBUG("otPlatRadioClearSrcMatchShortEntry\n");
    (void)aInstance;
    (void)aShortAddress;
    return OT_ERROR_NONE;
}

<<<<<<< HEAD
otError otPlatRadioClearSrcMatchExtEntry(otInstance *aInstance,
        const otExtAddress *aExtAddress)
=======
otError otPlatRadioClearSrcMatchExtEntry(otInstance *aInstance, const uint8_t *aExtAddress)
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
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

<<<<<<< HEAD
otError otPlatRadioEnergyScan(otInstance *aInstance, uint8_t aScanChannel,
        uint16_t aScanDuration)
=======
otError otPlatRadioEnergyScan(otInstance *aInstance, uint8_t aScanChannel, uint16_t aScanDuration)
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git
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
    return -97;
}
