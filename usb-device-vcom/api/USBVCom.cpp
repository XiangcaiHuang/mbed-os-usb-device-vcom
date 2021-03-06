#include "USBVCom.h"

usb_cdc_vcom_struct_t s_cdcVcom;

volatile uint32_t s_recvSize = 0;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t s_currRecvBuf[DATA_BUFF_SIZE];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t _txBuf[DATA_BUFF_SIZE];

static uint8_t *_userRxBuf = NULL;
static vcomRecHandler _userRxISR = NULL;

USBVCom::USBVCom()
{
    USB_Init();
    _checkInit();
}

USBVCom::USBVCom(vcomRecHandler isr, uint8_t *buff)
{
    USB_Init();
    _checkInit();

    _userRxBuf = buff;
    _userRxISR = isr;
}

void vcom_handle_received(void)
{
    if (_userRxBuf)
    {
        for (int i = 0; i < s_recvSize; ++i)
            _userRxBuf[i] = s_currRecvBuf[i];

        if (_userRxISR)
            _userRxISR(s_recvSize);

        s_recvSize = 0;
    }
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

bool USBVCom::isOpened(void)
{
    return (_isAttached() && _isStarted());
}

bool USBVCom::_checkInit(void)
{
    return _isAttached();
}

void USBVCom::print(char *msg)
{
    write((uint8_t *)msg, strlen(msg));
}

uint32_t USBVCom::isReadable(void)
{
    if (_checkInit())
    {
        if ((0 != s_recvSize) && (0xFFFFFFFFU != s_recvSize))
            return s_recvSize;
    }

    return 0;
}

// size of data sentat one time must be less than MAX_DATA_SIZE_SEND
usb_status_t USBVCom::write(uint8_t *buffer, uint32_t length)
{
    if (_checkInit())
    {
        if (length > MAX_DATA_SIZE_SEND)
        {
            length = MAX_DATA_SIZE_SEND;
        }

        for (uint32_t i = 0; i < length; ++i)
            _txBuf[i] = buffer[i];

        if (USB_DeviceCdcAcmSend(s_cdcVcom.cdcAcmHandle,
                             USB_CDC_VCOM_BULK_IN_ENDPOINT,
                             _txBuf, length) != kStatus_USB_Success)
        {
            printf("USBVCom::write: write failed!\r\n");
            return kStatus_USB_Error;
        }

        // must delay here for making sure all data has enough time to be sent
        wait_ms(5);
        memset(_txBuf, 0, sizeof(uint8_t) * length);

        // process() must be called after sending via VCOM to clear the flag - isBusy
        // by calling USB_DeviceCdcAcmBulkIn()
        process();
        return kStatus_USB_Success;
    }

    return kStatus_USB_Error;
}

usb_status_t USBVCom::read(uint8_t *buffer, uint32_t length)
{
    if (_checkInit())
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

    return kStatus_USB_Error;
}