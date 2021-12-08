#ifndef _SR_KFIFO_H
#define _SR_KFIFO_H

#include <stdint.h>
#include <stddef.h>
 
struct kfifo {
	uint8_t *buffer;	/* the buffer holding the data */
	uint32_t size;	/* the size of the allocated buffer */
	uint32_t in;	/* data is added at offset (in % size) */
	uint32_t out;	/* data is extracted from off. (out % size) */
};
 
/*
 * Macros for declaration and initialization of the kfifo datatype
 */
 
 
void kfifo_init(struct kfifo *fifo, void *buffer,
			uint32_t size);
uint32_t kfifo_in(struct kfifo *fifo,
				const void *from, uint32_t len);
uint32_t kfifo_out(struct kfifo *fifo,
				void *to, uint32_t len);
 
/**
 * kfifo_initialized - Check if kfifo is initialized.
 * @fifo: fifo to check
 * Return %true if FIFO is initialized, otherwise %false.
 * Assumes the fifo was 0 before.
 */
static inline int kfifo_initialized(struct kfifo *fifo)
{
	return fifo->buffer != NULL;
}
 
/**
 * kfifo_reset - removes the entire FIFO contents
 * @fifo: the fifo to be emptied.
 */
inline void kfifo_reset(struct kfifo *fifo)
{
	fifo->in = fifo->out = 0;
}
 
/**
 * kfifo_reset_out - skip FIFO contents
 * @fifo: the fifo to be emptied.
 */
static inline void kfifo_reset_out(struct kfifo *fifo)
{
	fifo->out = fifo->in;
}
 
/**
 * kfifo_size - returns the size of the fifo in bytes
 * @fifo: the fifo to be used.
 */
static inline uint32_t kfifo_size(struct kfifo *fifo)
{
	return fifo->size;
}
 
/**
 * kfifo_len - returns the number of used bytes in the FIFO
 * @fifo: the fifo to be used.
 */
inline uint32_t kfifo_len(struct kfifo *fifo)
{
	register uint32_t	out;
 
	out = fifo->out;
	
	return fifo->in - out;
}
 
/**
 * kfifo_is_empty - returns true if the fifo is empty
 * @fifo: the fifo to be used.
 */
static inline int kfifo_is_empty(struct kfifo *fifo)
{
	return fifo->in == fifo->out;
}
 
/**
 * kfifo_is_full - returns true if the fifo is full
 * @fifo: the fifo to be used.
 */
static inline int kfifo_is_full(struct kfifo *fifo)
{
	return kfifo_len(fifo) == kfifo_size(fifo);
}
 
/**
 * kfifo_avail - returns the number of bytes available in the FIFO
 * @fifo: the fifo to be used.
 */
static inline uint32_t kfifo_avail(struct kfifo *fifo)
{
	return kfifo_size(fifo) - kfifo_len(fifo);
}
 
extern void kfifo_skip(struct kfifo *fifo, uint32_t len);
 
/*
 * __kfifo_add_out internal helper function for updating the out offset
 */
static inline void __kfifo_add_out(struct kfifo *fifo,
				uint32_t off)
{
	fifo->out += off;
}
 
/*
 * __kfifo_add_in internal helper function for updating the in offset
 */
static inline void __kfifo_add_in(struct kfifo *fifo,
				uint32_t off)
{
	fifo->in += off;
}
 
/*
 * __kfifo_off internal helper function for calculating the index of a
 * given offeset
 */
static inline uint32_t __kfifo_off(struct kfifo *fifo, uint32_t off)
{
	return off & (fifo->size - 1);
}
 
int32_t kfifo_judge(struct kfifo *pkfifo);
 
#endif	/* _SR_KFIFO_H */
