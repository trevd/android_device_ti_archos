
#ifndef _CHECKFS_GRAPHICS_H_
#define _CHECKFS_GRAPHICS_H_
#include "minui/minui.h"

#define HORIZONTAL_OFFSET_DEFAULT 20
#define VERTICAL_OFFSET_DEFAULT 20
static int char_width;
static int char_height;

static int voffset=0;
static int hoffset=0;
static int fb_fd = 0 ;


static void initialize_graphics();
static void clear_screen(void);
static void draw_text(const char *str, int x, int y);
static void draw_formatted_text(const char *fmt, ...);
static void draw_formatted_text_static(const char *fmt, ...);
static void draw_formatted_text_horizontal(const char *fmt, ...);
static void draw_text_line(const char* text);

#endif
