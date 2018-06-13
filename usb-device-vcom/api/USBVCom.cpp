#include "USBVCom.h"

usb_cdc_vcom_struct_t s_cdcVcom;

volatile uint32_t s_recvSize = 0;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t s_currRecvBuf[DATA_BUFF_SIZE];

USBVCom::USBVCom()
{
    USB_Init();

    _checkInit();
}

void USBVCom::process(void)
{
    USB_DeviceTaskFn(s_cdcVcom.deviceHandle);
}

bool USBVCom::_isAttached(void)
{
    // when the USB is attached to host then attach == 1
    return (1 == s_cdcVcom.attach) ? true : false;
}

bool USBVCom::_isStarted(void)
{
    // when the USB VCOM port is opened then startTransactions == 1
    return (1 == s_cdcVcom.startTransactions) ? true : false;
}

void USBVCom::_checkInit(void)
{
    if (_isAttached() && _isStarted())
        _initOK = true;
    else
        _initOK = false;
}

void USBVCom::print(char *msg)
{
    uint32_t len = strlen(msg);
    // Don't print too more at one time
    if (len > DATA_BUFF_SIZE)
        len = DATA_BUFF_SIZE;

    write((uint8_t *)msg, len);
}

uint32_t USBVCom::isReadable(void)
{
    if (_initOK)
    {
        if ((0 != s_recvSize) && (0xFFFFFFFFU != s_recvSize))
            return s_recvSize;
    }

    _checkInit();
    return 0;
}

usb_status_t USBVCom::write(uint8_t *buffer, uint32_t length)
{
    if (_initOK)
    {
        if (USB_DeviceCdcAcmSend(s_cdcVcom.cdcAcmHandle, USB_CDC_VCOM_BULK_IN_ENDPOINT, buffer, length) == kStatus_USB_Success)
        {
            printf("Sent[%ld]: %s\r\n", length, (char *)buffer);
        }

        // process() must be called after sending via VCOM to clear the flag - isBusy
        // by calling USB_DeviceCdcAcmBulkIn()
        process();
        return kStatus_USB_Success;
    }

    _checkInit();
    return kStatus_USB_Error;
}

usb_status_t USBVCom::read(uint8_t *buffer, uint32_t length)
{
    if (_initOK)
    {
        if (length > s_recvSize)
        {
            // Don't Read too more
            length = s_recvSize;
        }

        /* Copy Buffer to Send Buff */
        for (uint32_t i = 0; i < length; i++)
            buffer[i] = s_currRecvBuf[i];

        s_recvSize = 0;

        return kStatus_USB_Success;
    }

    _checkInit();
    return kStatus_USB_Error;
}