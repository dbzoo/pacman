#ifndef __CONFIG_H__
#define __CONFIG_H__

#define P1_START	WIO_KEY_C	// left
#define P2_START	WIO_KEY_B	// middle
#define COIN		  WIO_KEY_A	// right

#define ORIENT  reverse_portrait
// joystick map for orientation
// the WIO library defines these in landscape terms
#define KEY_LEFT	WIO_5S_DOWN
#define KEY_RIGHT	WIO_5S_UP
#define KEY_UP		WIO_5S_LEFT
#define KEY_DOWN	WIO_5S_RIGHT

#endif
