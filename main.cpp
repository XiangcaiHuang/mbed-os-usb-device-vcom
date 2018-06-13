// Notice:
// USB-Uart baudRate 9600
// USB-Vcom baudRate 115200

// USB-Uart's Output:
/*
USB FS clock enabled using ISR 48MHz
USB device CDC virtual com init OK
USB device is running
Received[6]: 123456
Sent[6]: 123456
*/
#include "mbed.h"
#include "USBVCom.h"

/* Data buffer for receiving and sending*/
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t rec_buff[DATA_BUFF_SIZE];

USBVCom vcom;

void APPTask(void)
{
    uint32_t len = vcom.isReadable();
    if (len)
    {
        vcom.read(rec_buff, len);
        printf("Received[%ld]: %s\r\n", len, (char *)rec_buff);

        vcom.print("VCOM Test start:\r\n");
        for (int i = 0; i < 3; ++i)
        {
            printf("Times - [%d]\r\n", i + 1);
            vcom.write(rec_buff, len);
        }

        memset(rec_buff, 0, sizeof(uint8_t) * len);
        len = 0;
    }
}

int main(void)
{
    while (1)
    {
        APPTask();
        vcom.process();
    }
}
