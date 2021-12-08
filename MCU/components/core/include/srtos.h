#ifndef __SR_FREERTOS_H__
#define __SR_FREERTOS_H__

#ifdef __cplusplus
 extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>


/* app firmware version */
#define FIRMWARE_VERSION                    0L              /**< major version number */
#define FIRMWARE_SUBVERSION                 0L              /**< minor version number */
#define FIRMWARE_REVISION                   12L              /**< revise version number */

#define SR_FIRMWARE_VERSION                ((FIRMWARE_VERSION * 1000 * 1000) + \
                                         (FIRMWARE_SUBVERSION * 1000) + FIRMWARE_REVISION)
                                         
/* freertos kernel version */
#define KERNEL_VERSION                      9L              /**< major version number */
#define KERNEL_SUBVERSION                   0L              /**< minor version number */
#define KERNEL_REVISION                     0L              /**< revise version number */

/* Kernel version */
#define SR_KERNEL_VERSION                ((KERNEL_VERSION * 1000 * 1000) + \
                                         (KERNEL_SUBVERSION * 1000) + KERNEL_REVISION)
										 
#define	HW_VER_MAJOR 						HW_MAJOR 	
#define	HW_VER_MINOR 						HW_MINOR
#define	HW_VER_REVISE 						HW_REVISE

/* hw version */
#define SR_HW_VERSION                	((HW_VER_MAJOR * 1000 * 1000) + \
                                         (HW_VER_MINOR * 1000) + HW_VER_REVISE)
                                         
#define SR_HW_SERIAL					0x01

#ifndef  TRUE
#define TRUE		1
#endif

#ifndef FALSE
#define FALSE 		0
#endif


#undef NULL 
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif


#define CAN_DATA_MAX_LEN	8

/* 系统时钟节拍为1ms */
#define SYSTEM_TICK_MS		((configTICK_RATE_HZ/configTICK_RATE_HZ)*1)

/* system crypto */
#define SR_USING_CRC16_XMODEM


//#define	SR_USING_DMA_LOG
#define SR_USING_DEBUG
#define SR_CONSOLE_BUF 							512			/* debug log buf */
#define SR_SERIAL_COMM_BUF 						256
#define SR_SERIAL_COMM1_BUF 					256
#define SR_SERIAL_COMM3_BUF 					256

/* On-chip Peripheral Drivers */

#define SR_USING_UART1
#define SR_USING_UART2
#define SR_USING_UART3
#define SR_USING_UART4
#define SR_USING_UART5
//#define SR_USING_UART6 /* 已用于系统debug */

#define SR_NAME_MAX 							16

/* total task num is 8 and the memory occupied by each task is 40B */
#define SR_SYS_TASK_INFO_BUF_MAX				(12*40)


/* 电机使能	*/
#define SR_MOTOR_ENABLE   

//超声蔽障使能
#define SR_ULTRASONIC

//网络通信使能
#define SR_ENTHERNET

//WIFI通信使能，通过WiFi或者无线串口将数据发回控制台
#define SR_WIFI  

#define SR_MEC_DIFF_MODE     //麦伦差速模式
//#define SR_MEC_SIDESWAY_MODE  //麦伦横移模式


/**
 * device (I/O) class type
 */
enum sr_device_class_type {
    SR_Device_Class_Char = 0,                           /**< character device */
    SR_Device_Class_Block,                              /**< block device */
    SR_Device_Class_NetIf,                              /**< net interface */
    SR_Device_Class_MTD,                                /**< memory device */
    SR_Device_Class_CAN,                                /**< CAN device */
    SR_Device_Class_RTC,                                /**< RTC device */
    SR_Device_Class_Sound,                              /**< Sound device */
    SR_Device_Class_Graphic,                            /**< Graphic device */
    SR_Device_Class_I2CBUS,                             /**< I2C bus device */
    SR_Device_Class_USBDevice,                          /**< USB slave device */
    SR_Device_Class_USBHost,                            /**< USB host bus */
    SR_Device_Class_SPIBUS,                             /**< SPI bus device */
    SR_Device_Class_SPIDevice,                          /**< SPI device */
    SR_Device_Class_SDIO,                               /**< SDIO bus device */
    SR_Device_Class_PM,                                 /**< PM pseudo device */
    SR_Device_Class_Pipe,                               /**< Pipe device */
    SR_Device_Class_Portal,                             /**< Portal device */
    SR_Device_Class_Timer,                              /**< Timer device */
    SR_Device_Class_Miscellaneous,                      /**< Miscellaneous device */
    SR_Device_Class_Sensor,                             /**< Sensor device */
    SR_Device_Class_Unknown                             /**< unknown device */
};



/**
 * Double List structure
 */
struct sr_list_node {
    struct sr_list_node *next;                          /**< point to next node. */
    struct sr_list_node *prev;                          /**< point to prev node. */
};
typedef struct sr_list_node sr_list_t;                  /**< Type for lists. */

/**
 * Single List structure
 */
struct sr_slist_node {
    struct sr_slist_node *next;                         /**< point to next node. */
};
typedef struct sr_slist_node sr_slist_t;                /**< Type for single list. */

/**
 * Base structure of Kernel object
 */
struct sr_object {
	int8_t  name[SR_NAME_MAX];                       /**< name of kernel object */
	uint8_t type;                                    /**< type of kernel object */
	uint8_t flag;                                    /**< flag of kernel object */

	sr_list_t  list;                                    /**< list node of kernel object */
};
typedef struct sr_object *sr_object_t;                  /**< Type for kernel objects. */



typedef struct sr_device_ops *sr_device_t;

/**
 * Device structure
 */
struct sr_device_ops {

    /* common device interface */
    int32_t  (*init)   (sr_device_t dev);
    int32_t  (*open)   (sr_device_t dev, uint16_t oflag);
    int32_t  (*close)  (sr_device_t dev);
    int32_t (*read)   (sr_device_t dev, int32_t pos, void *buffer, uint32_t size);
    int32_t (*write)  (sr_device_t dev, int32_t pos, const void *buffer, uint32_t size);
    int32_t  (*control)(sr_device_t dev, int cmd, void *args);

    void                     *user_data;                /**< device private data */
};

extern struct kservice_manager kservice_manage;

#define DBG_LOG		1
#define DBG_WARNING	2
#define	DBG_ERROR	3
#define CUR_LEVEL	DBG_WARNING


//pcTaskGetName(xTaskGetCurrentTaskHandle())	
#define dbg_log(level, format, ...)  					\
	do {												\
		if ((level) == DBG_LOG){						\
			sr_kprintf(format, ##__VA_ARGS__);			\
		} else if ((level) >= CUR_LEVEL) {				\
			sr_kprintf("file:%s, line:%d, "format, __FILE__, __LINE__, ##__VA_ARGS__);	\
		}												\
	} while(0)


enum sr_timer_id
{
	SR_TIMER_CAN = 1,
	SR_TIMER_SERIAL  = 2,
	SR_TIMER_DEV_MANAGE = 3,
	SR_TIMER_DEV_NODE_PROTECT = 4,
	SR_TIMER_SPI = 5
};


/**
 * sr_container_of - return the member address of ptr, if the type of ptr is the
 * struct type.
 */
#define sr_container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))
    

#define __offsetof(type,member)	(size_t) &(((type *)0)->member)
	
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))


/*
 * DEFINE_CACHE_ALIGN_BUFFER() is similar to ALLOC_CACHE_ALIGN_BUFFER, but it's
 * purpose is to allow allocating aligned buffers outside of function scope.
 * Usage of this macro shall be avoided or used with extreme care!
 */
#define DEFINE_ALIGN_BUFFER(type, name, size, align)            \
    static char __##name[roundup(size * sizeof(type), align)]   \
            __aligned(align);               \
                                    \
    static type *name = (type *)__##name
#define DEFINE_CACHE_ALIGN_BUFFER(type, name, size)         \
    DEFINE_ALIGN_BUFFER(type, name, size, ARCH_DMA_MINALIGN)



/* handy sizes */
#define SZ_1K               0x00000400
#define SZ_2K               0x00000800
#define SZ_4K               0x00001000
#define SZ_8K               0x00002000
#define SZ_16K              0x00004000
#define SZ_32K              0x00008000
#define SZ_64K              0x00010000
#define SZ_128K             0x00020000
#define SZ_256K             0x00040000
#define SZ_512K             0x00080000

#define SZ_1M               0x00100000
#define SZ_2M               0x00200000
#define SZ_4M               0x00400000
#define SZ_8M               0x00800000
#define SZ_16M              0x01000000
#define SZ_24M              0x01800000
#define SZ_31M              0x01F00000
#define SZ_32M              0x02000000
#define SZ_64M              0x04000000
#define SZ_128M             0x08000000
#define SZ_256M             0x10000000
#define SZ_512M             0x20000000

#define SZ_1G               0x40000000
#define SZ_2G               0x80000000


/*
 * Bits
 */
#define BIT0  (1<<0)
#define BIT1  (1<<1)
#define BIT2  (1<<2)
#define BIT3  (1<<3)
#define BIT4  (1<<4)
#define BIT5  (1<<5)
#define BIT6  (1<<6)
#define BIT7  (1<<7)
#define BIT8  (1<<8)
#define BIT9  (1<<9)
#define BIT10 (1<<10)
#define BIT11 (1<<11)
#define BIT12 (1<<12)
#define BIT13 (1<<13)
#define BIT14 (1<<14)
#define BIT15 (1<<15)
#define BIT16 (1<<16)
#define BIT17 (1<<17)
#define BIT18 (1<<18)
#define BIT19 (1<<19)
#define BIT20 (1<<20)
#define BIT21 (1<<21)
#define BIT22 (1<<22)
#define BIT23 (1<<23)
#define BIT24 (1<<24)
#define BIT25 (1<<25)
#define BIT26 (1<<26)
#define BIT27 (1<<27)
#define BIT28 (1<<28)
#define BIT29 (1<<29)
#define BIT30 (1<<30)
#define BIT31 (1<<31)


extern void sr_data_print(const char *name, const uint8_t *data, uint16_t len);

/**
 * This function will print a formatted string on system console
 *
 * @param fmt the format
 */
extern int32_t sr_kprintf(const char *fmt, ...);
extern int32_t sr_log2sros(const char *fmt, ...);

/**
 * This function will show the version of freertos
 */
extern void sr_show_version(void);


/* 寄存器为8字节大小 */
uint32_t get_sdk_commit_id(void);

/**
 * 获取当前sdk提交的分支及分支字符长度
 *
 * @param[in]	None.
 * @param[out]	branch:	当前sdk提交的分支
 * @param[out]	len	:	分支字符长度
 *
 * @return		0成功，其他失败
 */
int32_t get_sdk_branch(char* branch, uint8_t *len);

int32_t get_sdk_compile_time(char *local_time);
void timer_tick2log(uint32_t tick_ms);

#ifdef __cplusplus
}
#endif
#endif /* __SR_FREERTOS_H__ */

