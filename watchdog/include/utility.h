#ifndef UTILITY_H
#define UTILITY_H

#define YES 1
#define NO 0

#define SUCCESS 0
#define FAILURE 1

#define TRUE 1
#define FALSE 0

#define ON 1
#define OFF 0

#define NEGATIVE -1

#define TO_INT(x) *(int *)&x
#define TO_CHAR(x) *(char *)&x
#define TO_LONG(x) *(long *)&x
#define TO_DOUBLE(x) *(double *)&x

#define PTR_TO_INT(x) *(int *)x
#define PTR_TO_CHAR(x) *(char *)x
#define PTR_TO_LONG(x) *(long *)x
#define PTR_TO_DOUBLE(x) *(double *)x

#define WORD_SIZE sizeof(size_t)
#define IS_ALIGNED(x) ((size_t)x % WORD_SIZE == 0)

#define UNUSED(x) (void)x

#define BITS_IN_BYTE 8

#define MAX_ASCII 128

#define BIGGEST3(x, y, z) ((x > y && x > z) ? x : (y > z) ? y : z)


typedef unsigned char one_byte_t;



#endif /*UTILITY_H*/