#include "mbed.h"
#include "USBVCom.h"

/* Data buffer for receiving and sending*/
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t rec_buff[DATA_BUFF_SIZE];

USBVCom vcom;

void APPTask(void)
{
    if (vcom.isAttached() && vcom.isStarted())
    {
        uint32_t len = vcom.isReadable();
        if (len)
        {
            vcom.read(rec_buff, len);
            printf("Received[%ld]: %s\r\n", len, (char *)rec_buff);

            if (vcom.write(rec_buff, len) != kStatus_USB_Success)
                printf("Failed to send\r\n");
            else
                printf("Sent[%ld]: %s\r\n", len, (char *)rec_buff);

            memset(rec_buff, 0, sizeof(uint8_t) * len);
            len = 0;
        }
    }
}


int main(void)
{
    while (1)
    {
        APPTask();
        vcom.run();
    }
}
