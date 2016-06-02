#ifndef DDK_MEM_H
#define DDK_MEM_H

#include <linux/ioctl.h>

#ifdef __KERNEL__
#include "ddk.h"

struct ddk_device
{
	int in_ep, out_ep;
	int in_buf_size, out_buf_size;
	struct usb_device *device;
	struct usb_class_driver class;
};
struct ddk_device_file
{
	unsigned char *in_buf, *out_buf;
	int in_buf_left_over;
};
#endif

#define DDK_RD_OFF_GET _IOR('u', 3, int *)
#define DDK_RD_OFF_SET _IOW('u', 4, int)
#define DDK_WR_OFF_GET _IOR('u', 5, int *)
#define DDK_WR_OFF_SET _IOW('u', 6, int)
#define DDK_MEM_SIZE_GET _IOW('u', 7, int)

#endif
