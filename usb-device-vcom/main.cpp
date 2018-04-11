#include "mbed.h"
#include "virtual_com.h"

usb_cdc_vcom_struct_t s_cdcVcom;

volatile uint32_t s_recvSize = 0;
volatile static uint32_t s_sendSize = 0;

/* Data buffer for receiving and sending*/
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t s_currRecvBuf[DATA_BUFF_SIZE];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_currSendBuf[DATA_BUFF_SIZE];

void APPTask(void)
{
    usb_status_t error = kStatus_USB_Error;

    // when the USB is attached to host then attach == 1
    // when the USB VCOM port is opened then startTransactions == 1
    if ((1 == s_cdcVcom.attach) && (1 == s_cdcVcom.startTransactions))
    {
        if ((0 != s_recvSize) && (0xFFFFFFFFU != s_recvSize))
        {
            printf("Data received[%ld]: %s\r\n", s_recvSize, (char *)s_currRecvBuf);
            uint32_t i;

            /* Copy Buffer to Send Buff */
            for (i = 0; i < s_recvSize; i++)
            {
                s_currSendBuf[s_sendSize++] = s_currRecvBuf[i];
            }
            s_recvSize = 0;
        }

        if (s_sendSize)
        {
            uint32_t size = s_sendSize;
            s_sendSize = 0;

            printf("Try to send[%ld]: %s\r\n", size, (char *)s_currSendBuf);
            error = USB_DeviceCdcAcmSend(s_cdcVcom.cdcAcmHandle, USB_CDC_VCOM_BULK_IN_ENDPOINT, s_currSendBuf, size);
            if (error != kStatus_USB_Success)
            {
                printf("Failed to send\r\n");
            }
        }
    }
}


int main(void)
{
    APPInit();

    while (1)
    {
        APPTask();
        USB_DeviceTaskFn(s_cdcVcom.deviceHandle);
    }
}
