#ifndef DDK_USB_H
#define DDK_USB_H

#include <linux/ioctl.h>

#ifdef __KERNEL__
#include "ddk.h"

#define MEM_IN 0
#define MEM_OUT 1
#define SER_IN 2
#define SER_OUT 3

struct ddk_device
{
	int ep[MAX_ENDPOINTS];
	int attrib[MAX_ENDPOINTS];
	int buf_size[MAX_ENDPOINTS];
	unsigned char *buf[MAX_ENDPOINTS];
	int buf_left_over[MAX_ENDPOINTS];
	struct usb_device *device;
	struct usb_class_driver class;
};
#endif

typedef struct
{
	int len;
	char data[8];
} IoctlData;

#define DDK_LED_GET _IOR('u', 1, int *)
#define DDK_LED_SET _IOW('u', 2, int)
#define DDK_RD_OFF_GET _IOR('u', 3, int *)
#define DDK_RD_OFF_SET _IOW('u', 4, int)
#define DDK_WR_OFF_GET _IOR('u', 5, int *)
#define DDK_WR_OFF_SET _IOW('u', 6, int)
#define DDK_MEM_SIZE_GET _IOW('u', 7, int)

#define DDK_DATA_GET _IOR('u', 8, IoctlData *)
#define DDK_DATA_SET _IOW('u', 9, IoctlData *)

#endif
