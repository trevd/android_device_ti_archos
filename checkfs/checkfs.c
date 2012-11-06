/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <linux/loop.h>
#include <make_ext4fs.h>
#include <cutils/properties.h>
#include "minui/minui.h"

#define LOOP_SET_FD 0x4C00
#define IMAGE_EXISTS 0
#define IMAGE_NOT_FOUND -1

#define CACHE_IMAGE "/._/CAC"
#define CACHE_LOOP "/dev/block/loop1"

#define DATA_IMAGE "/._/DATA"
#define DATA_LOOP "/dev/block/loop0"

#define SYS_IMAGE "/._/SYS"
#define SYS_LOOP "/dev/block/loop2"

#define DATA_BLOCK_COUNT 500000
#define BLOCK_COUNT_500 = 125000

extern int __system(const char *command);

static int char_width;
static int char_height;
static int voffset;

int create_raw_image(const char* image_file, int count,const char* loop_position);
void draw_text_line(const char* text);


static void clear_screen(void)
{
    gr_color(255, 0, 0, 0);
};

static int draw_text(const char *str, int x, int y)
{
    int str_len_px = gr_measure(str);

    if (x < 0)
        x = (gr_fb_width() - str_len_px) / 2;
    if (y < 0)
        y = (gr_fb_height() - char_height) / 2;
    gr_text(x, y, str);

    return y + char_height;
}

static void android_green(void)
{
    gr_color(0xa4, 0xc6, 0x39, 255);
}

/* returns the last y-offset of where the surface ends */
static int draw_surface_centered(gr_surface surface)
{
    int w;
    int h;
    int x;
    int y;

    w = gr_get_width(surface);
    h = gr_get_height(surface);
    x = (gr_fb_width() - w) / 2 ;
    y = (gr_fb_height() - h) / 2 ;
    gr_blit(surface, 0, 0, w, h, x, y);
    return y + h;
}
static void redraw_screen()
{
    clear_screen();
    gr_flip();
}
void draw_text_line(const char* text)
{
		draw_text(text,20,voffset);
		voffset += char_height;
		gr_flip();
	return;
	
}
int initialize_graphics(){

		int res = gr_init();
	    gr_font_size(&char_width, &char_height);
		voffset = 20 ;
		clear_screen();
		return res;		
}
static int loop_setup(const char* device,const char * backing_file)
{
   
    int flags = (MS_RDONLY) ? O_RDONLY : O_RDWR;
    int file_fd = open(backing_file, flags);
	int device_fd = open(device, flags);
	draw_text_line("Setting up loop device");
    if (file_fd < -1) {
        draw_text_line("open backing file failed");
        return 1;
    }
    if (device_fd < -1) {
        draw_text_line("open loop device failed");
        close(file_fd);
        return 1;
    }
    if (ioctl(device_fd, LOOP_SET_FD, file_fd) < 0) {
        draw_text_line("ioctl LOOP_SET_FD failed");
        close(file_fd);
        close(device_fd);
        return 1;
    }
    close(file_fd);
    close(device_fd);
	return 0;
}
int create_raw_image(const char* image_file, int count,const char* loop_position)
{
	 // smash
    char cmd[4096];
    sprintf(cmd, "/sbin/dd if=/dev/zero of=%s bs=4096 count=%d", image_file,count);
	draw_text_line(cmd);
    if(__system(cmd)) {
		draw_text_line("Fail");
        return -1;
	}
	loop_setup(loop_position,image_file);
	draw_text_line("creating ext4 filesystem");
	return 0;
}   
int check_image_file(const char* image_file)
{
    struct stat file_info;
	return stat(image_file, &file_info);
}
int main(int argc, char **argv)
{

	int graphics_initialized = -1;
	int image_status = check_image_file(CACHE_IMAGE);
	if( image_status ==  IMAGE_NOT_FOUND ){
		graphics_initialized = initialize_graphics();
		create_raw_image(CACHE_IMAGE,125000,CACHE_LOOP);
	}
	draw_text_line("Now watch this drive!");
	sleep(5);
	property_set("service.checkfs.complete","1");
	return 0;
   
}
