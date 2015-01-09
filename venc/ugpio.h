/*
 *
 * Copyright 2007-2009 VN, Inc.  All rights reserved.
 *
 */

#ifndef __UGPIO_H__
#define __UGPIO_H__

#define UGPIO_DRIVER_VERSION	"1.0.0.0"
#define UGPIO_MAINTAINER		"Tassader Chen"
#define COMPANY					"VN Inc."
#define UGPIO_PLATFORM			"Mozart EVM"

#define N_UGPIO					6

#define UGPIO_MAGIC 			0xd0
/* support only partial functions of original GPIO */
#define UGPIO_MINNR				1
#define UGPIO_READ_DATAIN		_IOR(UGPIO_MAGIC, 2, int)
#define UGPIO_DATA_SET			_IOW(UGPIO_MAGIC, 3, int)
#define UGPIO_DATA_CLEAR		_IOW(UGPIO_MAGIC, 4, int)
#define UGPIO_MAXNR				5

/*
 * uart-gpio pin map:
 *
 *   device node		 pin	direction
 * /dev/uart-gpio0		nCTS		I
 * /dev/uart-gpio1		nDSR		I
 * /dev/uart-gpio2		 nRI		I
 * /dev/uart-gpio3		nDCD		I
 * /dev/uart-gpio4		nDTR		O
 * /dev/uart-gpio5		nRTS		O
 *
 */


#endif /* __UGPIO_H__ */
