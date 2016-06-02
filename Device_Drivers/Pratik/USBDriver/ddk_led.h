#ifndef DDK_LED_H
#define DDK_LED_H

#include <linux/ioctl.h>

#ifdef __KERNEL__
#include "ddk.h"

struct ddk_device
{
	struct usb_device *device;
	struct usb_class_driver class;
};
#endif

#define DDK_LED_GET _IOR('u', 1, int *)
#define DDK_LED_SET _IOW('u', 2, int)

#endif
