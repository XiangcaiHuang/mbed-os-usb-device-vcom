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

// uint8_t userRxBuff[DATA_BUFF_SIZE];

// void userUSBVComISR(uint32_t rxLen)
// {
//     printf("userRxBuff[%d]: %s\r\n", rxLen, (char *)userRxBuff);
// }

USBVCom vcom;
// USBVCom vcom(userUSBVComISR, userRxBuff);

extern USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t s_currRecvBuf[DATA_BUFF_SIZE];
// extern volatile uint32_t s_recvSize;

void vcom_handle_received(void)
{
    printf("s_currRecvBuf: %s\r\n", (char *)s_currRecvBuf);
    // printf("s_currRecvBuf[%d]: %s\r\n", s_recvSize, (char *)s_currRecvBuf);
    s_recvSize = 0;
}

int main(void)
{
    while (1)
    {
        vcom.process();
    }
}
