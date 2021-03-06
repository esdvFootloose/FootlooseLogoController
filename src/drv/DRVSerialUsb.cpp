/*************************************************
 * @file DRVSerialUsb.h
 * @brief Serial CDC driver interface
 *
 *************************************************/

#include "stdint.h"

#include "drv/DRVSerialUsb.h"
extern "C"
{
#include "usb_device.h"
#include "usbd_cdc_if.h"
}

DRVSerialUsb::DRVSerialUsb()
    : DRVSerial(false)
{
    /* init code for USB_DEVICE */
    MX_USB_DEVICE_Init();
}

DRVSerialUsb::~DRVSerialUsb() {}

// void DRVSerialUsb::send(char* buf) {}

void DRVSerialUsb::send(std::string buf)
{
    if (m_status == Open)
    {
        CDC_Transmit_FS((uint8_t*)buf.c_str(), buf.length());
    }
}

void DRVSerialUsb::send(uint8_t* buf, uint32_t length)
{
    if (m_status == Open)
    {
        CDC_Transmit_FS(buf, length);
    }
}

void DRVSerialUsb::sendByte(uint8_t data)
{
    if (m_status == Open)
    {
        CDC_Transmit_FS(&data, 1);
    }
}

uint32_t DRVSerialUsb::getBytesPending() { return 0; }

uint32_t DRVSerialUsb::readLine(char* buf, uint32_t bufferSize)
{
    (void)buf;
    (void)bufferSize;
    return 0;
}

uint32_t DRVSerialUsb::readBuffer(uint8_t* buf, uint32_t bufferSize)
{
    (void)buf;
    (void)bufferSize;
    return 0;
}

uint32_t DRVSerialUsb::readByte() { return 0; }
