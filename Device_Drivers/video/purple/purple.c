#include <linux/fb.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>


inline uint32_t pixel_color(uint8_t r, uint8_t g, uint8_t b, struct fb_var_screeninfo *vinfo)
{
	return (r<<vinfo->red.offset) | (g<<vinfo->green.offset) | (b<<vinfo->blue.offset);
}

int main()
{
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;

	int fb_fd = open("/dev/fb0",O_RDWR);

	if (-1 == fb_fd)
	{
		printf("/dev/fb0/ opening failed\n");
	}

	// Get variable screen information
	if (-1 == ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo))
	{	
		printf("ioctl failed----1\n");
	}

	vinfo.grayscale=0;
	vinfo.bits_per_pixel=32;

	if (-1 == ioctl(fb_fd, FBIOPUT_VSCREENINFO, &vinfo))
	{
                printf("ioctl failed----2\n");
	}

	if (-1 == ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo))
	{
                printf("ioctl failed----3\n");
	}

	if (-1 == ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo))
	{
                printf("ioctl failed----4\n");
	}

	long screensize = vinfo.yres_virtual * finfo.line_length;
	
	printf("line length = %d\n", finfo.line_length);
	printf("Screensize = %ld\n", screensize);

	uint8_t *fbp = mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, (off_t)0);

	int x,y;

	for (x=0;x<vinfo.xres;x++) {
		for (y=0;y<vinfo.yres;y++)
		{
			long location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;
			*((uint32_t*)(fbp + location)) = pixel_color(0xFF,0x00,0x00, &vinfo);
			
		}
		usleep(1);
		//printf(__func__);
	}
	return 0;

}
