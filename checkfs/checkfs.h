#define ERROR_NUMBER_SUCCESS  0
#define ERROR_NUMBER_BOOTMODE -99
#define ERROR_NUMBER_NOT_ENOUGH_SPACE -98
#define ERROR_NUMBER_CREATE_RAW -97
#define ERROR_MESSAGE_SUCCESS  "SUCCESS"
#define ERROR_MESSAGE_BOOTMODE "Cannot boot into android as the setup is incomplete" 
#define ERROR_MESSAGE_NOT_ENOUGH_SPACE "There is not enough space to create the filesystem partitions available:%dMB required:%dMB"

// Initialize space required for all partitions
#define FULL_SPACE_REQUIRED 3000
#define PARTITION_INDEX_SYSTEM 0
#define PARTITION_INDEX_DATA 1
#define PARTITION_INDEX_CACHE 2
#define PARTITION_BLOCK_SIZE_4KB 4096
#define PARTITION_BLOCK_SIZE_32KB 32768
#define BLOCK_COUNT_2000_32KB 62500
#define BLOCK_COUNT_500_32KB 15625
#define ARRAYSIZE(a) ((int)(sizeof(a) / sizeof(*(a)))) 

static int reboot_queued =0;
static int check_available_space_mb();

static void check_partition_image(int index);
static int check_bootmode_recovery();

static void create_raw_partition_image(int index);
static void create_partition_image(int index);
static void create_loopback_device(int index);
static void create_ext4fs(int index);

static void print_error_and_reboot(int message_index);


// Early version, make some dirty assumuption about what is where
static struct partition_image_info{
	char* 	friendly_name;
	char* 	image_name;
	char*   loop_mount;
	int 	size_mb;
	int 	block_size_kb;
	long	block_count;
	int	create	;
}partition_image[3] = {
					 {"data","/._/DATA","/dev/block/loop0",2000,PARTITION_BLOCK_SIZE_32KB,BLOCK_COUNT_2000_32KB,0},
					 {"cache","/._/CAC","/dev/block/loop1",500,PARTITION_BLOCK_SIZE_32KB,BLOCK_COUNT_500_32KB,0},
					 {"system","/._/SYS","/dev/block/loop2",500,PARTITION_BLOCK_SIZE_32KB,BLOCK_COUNT_500_32KB,0}
					};
static const int partition_total = ARRAYSIZE(partition_image);
static int space_required_mb = FULL_SPACE_REQUIRED;
static int available_space_mb = 0;
static const char* parition_storage_mountpoint = "/._";
static char bootmode[PROPERTY_VALUE_MAX] = {'\0'};

