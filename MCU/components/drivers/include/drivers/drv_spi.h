#ifndef __SPI_H__
#define __SPI_H__

#include <stdlib.h>
#include <stdint.h>
#include "stm32f4xx.h" 

#ifdef __cplusplus
extern "C"{
#endif

#define SR_SPI_CPHA     (1<<0)                             /* bit[0]:CPHA, clock phase */
#define SR_SPI_CPOL     (1<<1)                             /* bit[1]:CPOL, clock polarity */
/**
 * At CPOL=0 the base value of the clock is zero
 *  - For CPHA=0, data are captured on the clock's rising edge (low->high transition)
 *    and data are propagated on a falling edge (high->low clock transition).
 *  - For CPHA=1, data are captured on the clock's falling edge and data are
 *    propagated on a rising edge.
 * At CPOL=1 the base value of the clock is one (inversion of CPOL=0)
 *  - For CPHA=0, data are captured on clock's falling edge and data are propagated
 *    on a rising edge.
 *  - For CPHA=1, data are captured on clock's rising edge and data are propagated
 *    on a falling edge.
 */
#define SR_SPI_LSB      (0<<2)                             /* bit[2]: 0-LSB */
#define SR_SPI_MSB      (1<<2)                             /* bit[2]: 1-MSB */

#define SR_SPI_MASTER   (0<<3)                             /* SPI master device */
#define SR_SPI_SLAVE    (1<<3)                             /* SPI slave device */

#define SR_SPI_MODE_0       (0 | 0)                        /* CPOL = 0, CPHA = 0 */
#define SR_SPI_MODE_1       (0 | RT_SPI_CPHA)              /* CPOL = 0, CPHA = 1 */
#define SR_SPI_MODE_2       (RT_SPI_CPOL | 0)              /* CPOL = 1, CPHA = 0 */
#define SR_SPI_MODE_3       (RT_SPI_CPOL | RT_SPI_CPHA)    /* CPOL = 1, CPHA = 1 */

#define SR_SPI_MODE_MASK    (SR_SPI_CPHA | SR_SPI_CPOL | SR_SPI_MSB)

#define SR_SPI_BUS_MODE_SPI         (1<<0)       
#define SR_SPI_BUS_MODE_QSPI        (1<<1)       

#define SR_SPI_CS_HIGH  (1<<4)                             /* Chipselect active high */
#define SR_SPI_NO_CS    (1<<5)                             /* No chipselect */
#define SR_SPI_3WIRE    (1<<6)                             /* SI/SO pin shared */
#define SR_SPI_READY    (1<<7)                             /* Slave pulls low to pause */

/**
 * SPI message structure
 */
struct sr_spi_message
{
    const void *send_buf;
    void *recv_buf;
    size_t length;
    struct sr_spi_message *next;

    unsigned cs_take    : 1;
    unsigned cs_release : 1;
};

/**
 * SPI configuration structure
 */
struct sr_spi_configuration
{
    uint8_t mode;
    uint8_t data_width;
    uint16_t reserved;

    uint32_t max_hz;
};



int32_t spi_bus_init(void);



#ifdef __cplusplus
}
#endif

#endif
