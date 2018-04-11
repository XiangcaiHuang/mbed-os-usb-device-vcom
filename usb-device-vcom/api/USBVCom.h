#ifndef _USB_VCOM_H_
#define _USB_VCOM_H_

#include "mbed.h"
#include "virtual_com.h"

class USBVCom
{
public:
	USBVCom();

	void run(void);

	bool isAttached(void);

	bool isStarted(void);

	uint32_t isReadable(void);

	usb_status_t write(uint8_t *buffer, uint32_t length);

	usb_status_t read(uint8_t *buffer, uint32_t length);

private:

};
#endif