// Notice:
// USB-Uart baudRate 9600
// USB-Vcom baudRate 115200

// USB-Uart's Output:
/*
USB FS clock enabled using ISR 48MHz
USB device CDC virtual com init OK
USB device is running

Sent[19]: Nice to meet you!

Sent[19]: Nice to meet you!

Received[3]: 123
Sent[23]: VCOM Send Test start:

Times - [1]
Sent[3]: 123
Times - [2]
Sent[3]: 123
Times - [3]
Sent[3]: 123
*/

// USB-Vcom's Output:
/*
Nice to meet you!
Nice to meet you!
VCOM Send Test start:
123123123
*/
#include "mbed.h"
#include "USBVCom.h"

/* Data buffer for receiving and sending*/
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t rec_buff[DATA_BUFF_SIZE];

USBVCom vcom;

void APPTask(void)
{
    char *str = "Nice to meet you!\r\n";

    uint32_t len = vcom.isReadable();
    if (len)
    {
        vcom.print(str);
        vcom.write((uint8_t *)str, strlen(str));

        vcom.read(rec_buff, len);
        printf("Received[%ld]: %s\r\n", len, (char *)rec_buff);

        vcom.print("VCOM Send Test start:\r\n");
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
