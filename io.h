#ifndef __IO_H__
#define __IO_H__

#define INT_ENABLE	0x00
#define SOUND_ENABLE	0x01
#define FLIP_SCREEN	0x03
#define ONE_PLAYER_START	0x04
#define TWO_PLAYER_START	0x05
#define COIN_LOCKOUT	0x06
#define COIN_COUNTER	0x07

#define SOUND_START	0x40
#define SOUND_LEN	0x20
#define SPRITE_START	0x60
#define SPRITE_LEN	0x10
#define WATCHDOG	0xc0

#define INPUTS_0	0x00
#define INPUTS_1	0x40
#define DIP_1		0x80
#define DIP_2		0xc0

#define FREE_PLAY		0b00000000
#define ONE_COIN_ONE_GAME	0b00000001
#define ONE_COIN_TWO_GAMES	0b00000010
#define TWO_COINS_ONE_GAME	0b00000011

#define ONE_LIFE_PER_GAME	0b00000000
#define TWO_LIVES_PER_GAME	0b00000100
#define THREE_LIVES_PER_GAME	0b00001000
#define FIVE_LIVES_PER_GAME	0b00001100

#define BONUS_AT_10000		0b00000000
#define BONUS_AT_15000		0b00010000
#define BONUS_AT_20000		0b00100000
#define NO_BONUS		0b00110000

#define DIFFICULTY_HARD		0b00000000
#define DIFFICULTY_NORMAL	0b01000000

#define ALTERNATE_NAMES		0b00000000
#define NORMAL_NAMES		0b10000000

class IO: public Memory::Device {
public:
  IO(Memory &mem): Memory::Device(sizeof(_tp)), _mem(mem) {
		_up = _down = _left = _right = true;
		_coin = _p1_start = _p2_start = true;
	}

	void operator=(uint8_t);
	operator uint8_t();
  uint8_t& operator[] (uint8_t index) { return _tp[index]; };

	void scan();
	void init();

	bool int_enabled() { return _int_enabled; }
	bool sound_enabled() { return _sound_enabled; }
	bool screen_flipped() { return _screen_flipped; }

private:
	uint8_t _sx;

	bool _up, _down, _left, _right, _coin, _p1_start, _p2_start;
	bool _int_enabled, _sound_enabled, _screen_flipped;

  // We only need 32 bytes for sprite x/y - 0x5060-0x506f
  // Range for the entire memory mapped register space is easier to deal with.
  uint8_t _tp[256]; 
  Memory &_mem;
};

#endif
