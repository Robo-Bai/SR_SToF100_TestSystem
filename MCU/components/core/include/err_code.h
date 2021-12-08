#ifndef  __SR_ERR_CODE_H__
#define  __SR_ERR_CODE_H__

#include "core\include\sr_sys_fault_code.h"


/* OS error code definitions */
#define E_OK                           (0)               /**< There is no error */
#define E_ERROR                        (-1)               /**< A generic error happens */
#define E_ETIMEOUT                     (-2)               /**< Timed out */
#define E_EFULL                        (-3)               /**< The resource is full */
#define E_EEMPTY                       (-4)               /**< The resource is empty */
#define E_ENOMEM                       (-5)               /**< No memory */
#define E_ENOSYS                       (-6)               /**< No system */
#define E_EBUSY                        (-7)               /**< Busy */
#define E_EIO                          (-8)               /**< IO error */
#define E_EINTR                        (-9)               /**< Interrupted system call */
#define E_EINVAL                       (-10)              /**< Invalid argument */
#define E_ECAN_ID                       (-11)              /**< Invalid device id */
#define E_ECAN_TX_STA_FAILED         	(-1)				/*!< CAN transmission failed */
#define E_ECAN_TX_STA_PENDING       	(2) 				/*!< CAN transmission pending */
#define E_ECAN_TX_STA_NOMAILBOX      	(-4) 				/*!< CAN cell did not provide an empty mailbox */





#endif /* __SR_ERR_CODE_H__ */
