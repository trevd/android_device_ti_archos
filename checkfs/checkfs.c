/*

	checkfs - designed to provide automated creation of ext4 fileimage
	based system layout. this 
 
*/
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/mount.h>
#include <linux/loop.h>
#include <make_ext4fs.h>
#include <cutils/properties.h>
#include <cutils/android_reboot.h>
#include <fcntl.h>
#include "checkfs.h"
#include "graphics.h"


#define LOOP_SET_FD 0x4C00

static void create_ext4fs(int index)
{
	draw_formatted_text("making ext4fs for %d\n",partition_image[index].friendly_name);
	make_ext4fs(	partition_image[index].loop_mount, 0, NULL,0);
}
static void create_loopback_device(int index)
{
   draw_formatted_text("doing loop_setup");
    int flags =  O_RDWR;
    int file_fd = open(partition_image[index].image_name,flags);
	int device_fd = open(partition_image[index].loop_mount, flags);
	draw_text_line("Setting up loop device");
    if (file_fd < -1) {
        draw_text_line("open backing file failed");
        return ;
    }
    if (device_fd < -1) {
        draw_text_line("open loop device failed");
        close(file_fd);
        return ;
    }
    if (ioctl(device_fd, LOOP_SET_FD, file_fd) < 0) {
        draw_text_line("ioctl LOOP_SET_FD failed");
        close(file_fd);
        close(device_fd);
        return ;
    }
    close(file_fd);
    close(device_fd);
	return ;
}
static void create_partition_image(int index){
	printf("create_partition_image:%d : %s %s\n",index,partition_image[index].image_name, partition_image[index].friendly_name);
	draw_formatted_text("create_partition_image:%d : %s %s %lu",index,partition_image[index].image_name, partition_image[index].friendly_name,partition_image[index].block_count);
	create_raw_partition_image(index);
	create_loopback_device(index);
	create_ext4fs(index);
	partition_image[index].create =0;
	return;
}
static void create_raw_partition_image(int index){
		
		int blk_32k[PARTITION_BLOCK_SIZE_32KB]= {0};
		int fd_out = creat(partition_image[index].image_name,O_RDWR);
		if(fd_out<0) return ;

		long counter=0; long bs = partition_image[index].block_size_kb; long total = partition_image[index].block_count+1;
		// acey says 10 %		
		long ACEY =	total / 10;int percent_complete =1;
		
		for(counter=0;counter<total;counter++){
			write(fd_out,blk_32k,bs);
			if(counter == (ACEY* percent_complete)){
				draw_formatted_text_horizontal("%d",counter);
				percent_complete +=1;
			}	
		}
		hoffset = HORIZONTAL_OFFSET_DEFAULT;
		if(fd_out){
			fsync(fd_out);
			close(fd_out);}
}
static void check_partition_image(int index){
	
	struct stat file_info;
	if(stat(partition_image[index].image_name, &file_info)==-1)
		partition_image[index].create=1;
	else // if the image already exists then remove the size from the requirement
		space_required_mb -= partition_image[index].size_mb;		
	return;

}
static int check_avilable_space_mb(){
    struct statfs st;
	int freespace_mb = -1;
    if (statfs(parition_storage_mountpoint, &st) < 0) {
        fprintf(stderr, "%s: %s\n", parition_storage_mountpoint, strerror(errno));
		return -1;
    } else {
        if (st.f_blocks == 0 )
            return -1;  
		freespace_mb = (int)(((((long long)st.f_bfree * (long long)st.f_bsize))/1024)/1024);
    }
	return freespace_mb;
}

static int check_bootmode_recovery(){
	// make sure we are in recovery mode
	property_get("ro.bootmode",bootmode,NULL);
	int res = strncmp(bootmode,"recovery",strlen(bootmode));
	if(!res)
		return 1;
	return 0;
}
static void print_error_and_reboot(int error_index){
	initialize_graphics();
		if(error_index == ERROR_NUMBER_BOOTMODE)
		draw_text_line(ERROR_MESSAGE_BOOTMODE);

		if(error_index == ERROR_NUMBER_NOT_ENOUGH_SPACE)
				draw_formatted_text(ERROR_MESSAGE_NOT_ENOUGH_SPACE,available_space_mb,space_required_mb);
		
		sleep(5);
		android_reboot(ANDROID_RB_RESTART2, 0, "recovery");
		return;
}
int main(int argc, char **argv)
{
	int index, bootmode_recovery; 
	// do some checks to see if we need to 
	// display anything, 
	// 1. Check partition existance
	for( index =0; index < partition_total; index++)
		check_partition_image(index);

	// all partitions exist. do nothing
	if(!space_required_mb) 
			goto early_exit;		

	bootmode_recovery =check_bootmode_recovery();
	available_space_mb  =check_avilable_space_mb();
	
	if(available_space_mb<space_required_mb){
		print_error_and_reboot(ERROR_NUMBER_NOT_ENOUGH_SPACE);
		goto early_exit;	
	}
			
		

initialize:
	// we need to create at least one partition
	// first check we are in recovery
	if(!bootmode_recovery){
		print_error_and_reboot(ERROR_NUMBER_BOOTMODE);
		goto exit;
	}

	initialize_graphics();		

	for( index =0; index < partition_total; index++){
		if(partition_image[index].create){
		    create_partition_image(index);
			reboot_queued =1 ;
		}
	}
	if(reboot_queued) android_reboot(ANDROID_RB_RESTART2, 0, "recovery");	
	goto exit;	
	
	
exit:
	sleep(5);
early_exit:
	property_set("service.checkfs.complete","1");
	return 0;   
}

//######### GRAPHICS ###################################
static void initialize_graphics(){

	if(fb_fd) return ;
		printf("graphics init\n");
		fb_fd = gr_init();
	    gr_font_size(&char_width, &char_height);
		voffset = 20 ;
		printf("graphics init done\n");
		clear_screen();


}
static void clear_screen(void)
{
    gr_color(255, 255, 0, 255);
	//
};
static void draw_formatted_text(const char *fmt, ...)
{
	char *strp = NULL;
	va_list args;
	va_start(args, fmt);
	vasprintf(&strp, fmt, args);
	va_end(args);
	printf("draw_formatted_text:%s\n",strp);
	draw_text_line(strp);
	return ;
}
static void draw_text_line(const char* text){
	draw_text(text,20,voffset);
	voffset+=char_height;
}
static void draw_text(const char *str, int x, int y)
{
    int str_len_px = gr_measure(str);

    if (x < 0)
        x = (gr_fb_width() - str_len_px) / 2;
    if (y < 0)
        y = (gr_fb_height() - char_height) / 2;
    gr_text(x, y, str);
	gr_flip();
  //  return y + char_height;
}
static void draw_formatted_text_static(const char *fmt, ...)
{
	char *strp = NULL;
	va_list args;
	va_start(args, fmt);
	vasprintf(&strp, fmt, args);
	va_end(args);
	printf("draw_formatted_text_static:%s\n",strp);
	draw_text(strp,20,voffset);
	return ;
}
static void draw_formatted_text_horizontal(const char *fmt, ...)
{
	char *strp = NULL;
	va_list args;
	va_start(args, fmt);
	vasprintf(&strp, fmt, args);
	va_end(args);
	printf("draw_formatted_text_static:%s\n",strp);
	draw_text(strp,hoffset,voffset);
	hoffset += char_width*strlen(strp)+10;
	return ;
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
