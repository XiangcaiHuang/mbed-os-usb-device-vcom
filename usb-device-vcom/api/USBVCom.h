#ifndef _USB_VCOM_H_
#define _USB_VCOM_H_

#include "mbed.h"
#include "virtual_com.h"

#define MAX_DATA_SIZE_SEND (DATA_BUFF_SIZE - 20)

typedef void (*vcomRecHandler)(uint32_t);

class USBVCom
{
public:
	USBVCom();
	USBVCom(vcomRecHandler isr, uint8_t *buff);

	void process(void);

	void print(char *msg);

	bool isOpened(void);

	uint32_t isReadable(void);

	usb_status_t write(uint8_t *buffer, uint32_t length);

	usb_status_t read(uint8_t *buffer, uint32_t length);

private:
	bool _checkInit(void);

	bool _isAttached(void);

	bool _isStarted(void);
};
#endif