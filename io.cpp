#include <Arduino.h>
#include "r65emu.h"

#include "io.h"
#include "config.h"

void IO::init() {
  pinMode(KEY_UP, INPUT_PULLUP);
  pinMode(KEY_DOWN, INPUT_PULLUP);
  pinMode(KEY_LEFT, INPUT_PULLUP);
  pinMode(KEY_RIGHT, INPUT_PULLUP);  
  pinMode(P1_START, INPUT_PULLUP);
  pinMode(P2_START, INPUT_PULLUP);
  pinMode(COIN, INPUT_PULLUP); 
}

void IO::scan() {
  _up = digitalRead(KEY_UP) == HIGH;
  _down = digitalRead(KEY_DOWN) == HIGH;
  _left = digitalRead(KEY_LEFT) == HIGH;
  _right = digitalRead(KEY_RIGHT) == HIGH;
  _p1_start = digitalRead(P1_START) == HIGH;
  _p2_start = digitalRead(P2_START) == HIGH;
  _coin = digitalRead(COIN) == HIGH;
}

void IO::operator=(uint8_t b) {
 // 0x5060 - 0x506f (write sprite x,y coordinates)
 if (_acc >= SPRITE_START && _acc < SPRITE_START + SPRITE_LEN) {
   _tp[_acc] = b;
   return;
 }
	switch (_acc) {
	case INT_ENABLE:
		_int_enabled = (b & 0x01);
		break;
	case SOUND_ENABLE:
		_sound_enabled = (b & 0x01);
		break;
	case FLIP_SCREEN:
		_screen_flipped = (b & 0x01);
		break;
	}
}

IO::operator uint8_t() {
	if (_acc >= INPUTS_0 && _acc < INPUTS_1) {
		uint8_t v = 0x10;
		if (_up) v |= 0x01;
		if (_left) v |= 0x02;
		if (_right) v |= 0x04;
		if (_down) v |= 0x08;
		if (_coin) v |= 0x20;
		return v;
	}
	if (_acc >= INPUTS_1 && _acc < DIP_1) {
		uint8_t v = 0x10;
		if (_up) v |= 0x01;
		if (_left) v |= 0x02;
		if (_right) v |= 0x04;
		if (_down) v |= 0x08;
		if (_p1_start) v |= 0x20;
		if (_p2_start) v |= 0x40;
		return v;
	}
	if (_acc >= DIP_1)
		return NORMAL_NAMES | DIFFICULTY_NORMAL | BONUS_AT_20000 |
			ONE_LIFE_PER_GAME | ONE_COIN_ONE_GAME;
	return 0x00;
}
