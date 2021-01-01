#include "hardware.h"

#include <SPI.h>

#include "memory.h"
#include "CPU.h"

Memory memory;

static CPU *_cpu;

bool hardware_reset() {
	bool success = true;
	_cpu->reset();
	return success;
}

void hardware_init(CPU &cpu) {
	_cpu = &cpu;
	memory.begin();
}

#if !defined(NO_CHECKPOINT)
void hardware_checkpoint(Stream &s) {
	unsigned ds = 0;
	for (unsigned i = 0; i < 0x10000; i += ds) {
		Memory::Device *dev = memory.get(i);
		dev->checkpoint(s);
		ds = dev->pages() * Memory::page_size;
	}
	_cpu->checkpoint(s);
}

void hardware_restore(Stream &s) {
	unsigned ds = 0;
	for (unsigned i = 0; i < 0x10000; i += ds) {
		Memory::Device *dev = memory.get(i);
		dev->restore(s);
		ds = dev->pages() * Memory::page_size;
	}
	_cpu->restore(s);
}
#endif
