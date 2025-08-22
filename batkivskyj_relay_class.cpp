#include "configboard.h"


byte batjarelay::set_st;

batjarelay::batjarelay() {
	point = b0 = b1 = b2 = b3 = b4 = b5 = 0;
	set_st = 0;
	spracjuvalo = false;
	tmr_ignor = 0;
	tmr_autoreset = 0;
}


bool batjarelay::reakcija_res() {
	if(b0 == 0 && b2 == 0) {
		return false;
	}
	return ((~(b1 ^ in)) & b0) == b0 ? true : false;
}


bool batjarelay::_reakcija_imp() {
	if(b0 == 0 && b2 == 0) {
		return false;
	}
	if((b4 & 0x8000) == 0) {
		return b2 == (in_mem & b2) ? ((in & b2) == b3 ? true : false) : false;
	}
	else {
		return ((((~(in ^ b3)) & b2) & in_mem) != 0) && b2 != 0 ? true : false;
	}
}


bool batjarelay::reakcija_imp() {
	if(tmr_ignor == 0 && _reakcija_imp() == true) {
		tmr_ignor = ((byte)(b5 >> 10) & 0b00011000);
		return true;
	}
	return false;
}


void batjarelay::tmr_count() {
	if(tmr_ignor) {
		tmr_ignor--;
	}
	if(tmr_autoreset) {
		tmr_autoreset--;
	}
}


void batjarelay::set_relay_dija_1() {
	virt_out |= (b4 & 0b0001111111111111);
}


void batjarelay::set_relay_dija_2() {
	virt_out |= (b5 & 0b0001111111111111);
	virt_out &= ~((b4 & b5) & 0b0001111111111111);
}


void batjarelay::reset_relay() {
	virt_out &= ((b4 | b5) & 0b0001111111111111) ^ 0xffff;
}


byte batjarelay::admset(char key, byte navihacija_menu) {
	char* setstr = dsplbuf.getlaststr();
	unsigned short mask;
	cory.setDiap(7, 7, 0, 15);
	if(key == VK_LEFT || key == VK_RIGHT) {
		key = 0;
		return navihacija_menu;
	}
	byte i;
	if(set_st == 0) {
		dsplbuf.onblnk();
		dsplbuf.prnt(6, 0, "  static INPUT  ", 0, 0);
		mask = 0x8000;
		for(i = 0; i < 16; i++) {
			*(setstr + i) = (b0 & mask) == 0 ? 32 : ((b1 & mask) == 0 ? 48 : 49);
			mask >>= 1;
		}
		set_st = 1;
		return navihacija_menu;
	}
	else if(set_st == 1) {
		if(key == VK_DOWN || key == VK_UP) {
			i = cory.get() & 0x0f;
			mask = 0x8000 >> i;
			if(key == VK_UP && *(setstr + i) != 49) {
				*(setstr + i) = (*(setstr + i) == 32 ? 48 : 49);
			}
			else if(key == VK_DOWN && *(setstr + i) != 32) {
				*(setstr + i) = (*(setstr + i) == 49 ? 48 : 32);
			}
			if(*(setstr + i) == 32) {
				mask ^= 0xffff;
				b0 &= mask;
				b1 &= mask;
			}
			else if(*(setstr + i) == 48) {
				b0 |= mask;
				mask ^= 0xffff;
				b1 &= mask;
			}
			else {
				b0 |= mask;
				b1 |= mask;
			}
			return navihacija_menu;
		}
		if(key == VK_RETURN) {
			cory.set(7, 0);
			dsplbuf.prnt(6, 0, "  impuls INPUT  ", 0, 0);
			mask = 0x8000;
			for(i = 0; i < 16; i++) {
				*(setstr + i) = (b2 & mask) == 0 ? 32 : ((b3 & mask) == 0 ? 6 : 7);
				mask >>= 1;
			}
			set_st = 2;
			return navihacija_menu;
		}
	}
	else if(set_st == 2) {
		if(key == VK_DOWN || key == VK_UP) {
			i = cory.get() & 0x0f;
			mask = 0x8000 >> i;
			if(key == VK_UP && *(setstr + i) != 7) {
				*(setstr + i) = (*(setstr + i) == 32 ? 6 : 7);
			}
			else if(key == VK_DOWN && *(setstr + i) != 32) {
				*(setstr + i) = (*(setstr + i) == 7 ? 6 : 32);
			}
			if(*(setstr + i) == 32) {
				mask ^= 0xffff;
				b2 &= mask;
				b3 &= mask;
			}
			else if(*(setstr + i) == 6) {
				b2 |= mask;
				mask ^= 0xffff;
				b3 &= mask;
			}
			else {
				b2 |= mask;
				b3 |= mask;
			}
			return navihacija_menu;
		}
		if(key == VK_RETURN) {
			cory.set(7, 0);
			dsplbuf.prnt(6, 0, " dija 1 OUTPUT  ", 0, 0);
			mask = 0x8000;
			for(i = 0; i < 16; i++) {
				*(setstr + i) = (b4 & mask) == 0 ? 48 : 49;
				mask >>= 1;
			}
			set_st = 3;
			return navihacija_menu;
		}
	}
	else if(set_st == 3) {
		if(key == VK_DOWN || key == VK_UP) {
			i = cory.get() & 0x0f;
			mask = 0x8000 >> i;
			if(key == VK_UP) {
				*(setstr + i) = 49;
				b4 |= mask;
			}
			else if(key == VK_DOWN) {
				*(setstr + i) = 48;
				mask ^= 0xffff;
				b4 &= mask;
			}
			return navihacija_menu;
		}
		if(key == VK_RETURN) {
			cory.set(7, 0);
			dsplbuf.prnt(6, 0, " dija 2 OUTPUT  ", 0, 0);
			mask = 0x8000;
			for(i = 0; i < 16; i++) {
				*(setstr + i) = (b5 & mask) == 0 ? 48 : 49;
				mask >>= 1;
			}
			set_st = 4;
			return navihacija_menu;
		}
	}
	else if(set_st == 4) {
		if(key == VK_DOWN || key == VK_UP) {
			i = cory.get() & 0x0f;
			mask = 0x8000 >> i;
			if(key == VK_UP) {
				*(setstr + i) = 49;
				b5 |= mask;
			}
			else if(key == VK_DOWN) {
				*(setstr + i) = 48;
				mask ^= 0xffff;
				b5 &= mask;
			}
			return navihacija_menu;
		}
		if(key == VK_RETURN) {
			set_st = 0;
			navihacija_menu = 3;
			return navihacija_menu;
		}
	}
}


byte batjarelay::setup(char key) {
	return 0;
}


void batjarelay::prnt(byte y, byte x) {
	dsplbuf.prnt(y, x, "kuku   ", 0, 0);
	return;
}


#ifdef NEW_MEM
short int batjarelay::load(int x) {
	point = x;
	b0 = (short)readEEPROM(x, _SHORT);
	b1 = (short)readEEPROM(x + _SHORT * 1, _SHORT);
	b2 = (short)readEEPROM(x + _SHORT * 2, _SHORT);
	b3 = (short)readEEPROM(x + _SHORT * 3, _SHORT);
	b4 = (short)readEEPROM(x + _SHORT * 4, _SHORT);
	b5 = (short)readEEPROM(x + _SHORT * 5, _SHORT);
	set_st = 0;
	spracjuvalo = false;
	tmr_ignor = 0;
	tmr_autoreset = 0;
	return x + _SHORT * 6;
}


short int batjarelay::save(int x, byte save_fl) {
	x = point;
	if(save_fl == 101) {
		writeEEPROM(x, _SHORT, (long int)b0);
		writeEEPROM(x + _SHORT * 1, _SHORT, (long int)b1);
		writeEEPROM(x + _SHORT * 2, _SHORT, (long int)b2);
		writeEEPROM(x + _SHORT * 3, _SHORT, (long int)b3);
		writeEEPROM(x + _SHORT * 4, _SHORT, (long int)b4);
		writeEEPROM(x + _SHORT * 5, _SHORT, (long int)b5);
	}
	return x + _SHORT * 6;
}


#else
short int batjarelay::load(int x) {
	point = x;
	b0 = readData(x);
	b1 = readData(x + 1);
	b2 = readData(x + 2);
	b3 = readData(x + 3);
	b4 = readData(x + 4);
	b5 = readData(x + 5);
	set_st = 0;
	spracjuvalo = false;
	tmr_ignor = 0;
	tmr_autoreset = 0;
	return x + 6;
}



short int batjarelay::save(int x) {
	writeData(x + 0, (long int)b0);
	writeData(x + 1, (long int)b1);
	writeData(x + 2, (long int)b2);
	writeData(x + 3, (long int)b3);
	writeData(x + 4, (long int)b4);
	writeData(x + 5, (long int)b5);
	return x + 6;
}

#endif


void batjarelay::dija_vk_return() {
	return;
}