// Notice:
// USB-Uart baudRate 9600
// USB-Vcom baudRate 115200

#include "mbed.h"
#include "USBVCom.h"

#define EXAMPLE_ISR   0
#define EXAMPLE_QUERY 1
#define EXAMPLE_APP   EXAMPLE_ISR

static uint8_t userRxBuff[DATA_BUFF_SIZE];

#if (EXAMPLE_APP == EXAMPLE_ISR)
// USB-Uart output:
/*
USB FS clock enabled using ISR 48MHz
USB device CDC virtual com init OK
USB device is running
Sent[16]: Read and write testing start:
userRxBuff[6]: 123456
Sent[16]: Read and write testing start:
userRxBuff[4]: abed
*/

// USB-VCOM output:
/*
Read and write testing start:
123456
Read and write testing start:
abed
*/
static void userUSBVComISR(uint32_t rxLen);
USBVCom mVcom(userUSBVComISR, userRxBuff);

static void userUSBVComISR(uint32_t rxLen)
{
    mVcom.print("Read and write testing start:\r\n");
    mVcom.write(userRxBuff, rxLen);

    printf("userRxBuff[%d]: %s", rxLen, (char *)userRxBuff);
    memset(userRxBuff, 0, sizeof(uint8_t) * rxLen);
}

int main(void)
{
    while (1)
    {
        mVcom.process();
    }
}

#else
// USB-Uart output:
/*
USB FS clock enabled using ISR 48MHz
USB device CDC virtual com init OK
USB device is running
Sent[29]: Read and write testing start:
Received[7]: abcdefg
Sent[7]: abcdefg
Sent[29]: Read and write testing start:
Received[3]: 123
Sent[3]: 123
*/

// USB-VCOM output:
/*
Read and write testing start:
abcdefg
Read and write testing start:
123
*/
USBVCom mVcom;

static void read_query(void)
{
    uint32_t rxLen = mVcom.isReadable();
    if (rxLen)
    {
        mVcom.print("Read and write testing start:\r\n");

        mVcom.read(userRxBuff, rxLen);
        printf("Received[%d]: %s\r\n", rxLen, (char *)userRxBuff);

        mVcom.write(userRxBuff, rxLen);
        memset(userRxBuff, 0, sizeof(uint8_t) * rxLen);

        rxLen = 0;
    }
}

int main(void)
{
    while (1)
    {
        read_query();
        mVcom.process();
    }
}
#endif // EXAMPLE_APP
