#ifndef DDK_U2S_H
#define DDK_U2S_H

#ifdef __KERNEL__
#include "ddk.h"

struct ddk_device
{
	int in_ep, out_ep;
	int in_buf_size, out_buf_size;
	unsigned char *in_buf, *out_buf;
	int in_buf_left_over;
	struct usb_device *device;
	struct usb_class_driver class;
};
#endif

#endif
