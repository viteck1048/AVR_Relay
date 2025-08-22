#include "configboard.h"





int lichylnyk_imp::operator --(int) {
	tmr_count();
	if(reakcija_res()) {
		if(spracjuvalo == false && reakcija_imp()) {
			set_relay_dija_1();// LCD_COORD(1, 0);
			imp--;
			if(imp == 0) {
				spracjuvalo = true;
				set_relay_dija_2();
				tmr_autoreset = ((byte)(b4 >> 10) & 0b00011000);
			}
		}
	}
	else {
		tmr_autoreset = 0;
		reset_relay();
		spracjuvalo = false;
		imp = br_imp;
	}
	if(tmr_autoreset == 1) {
		tmr_autoreset = 0;
		spracjuvalo = false;
		imp = br_imp;
		reset_relay();
	}
	return 0;
}

byte lichylnyk_imp::setup(char key) {
	char* setstr = dsplbuf.getlaststr() + 9;
	if(set_st == 0) {
		dsplbuf.dsplbuf_clear();
		dsplbuf.prnt(6, 0, "Set broy impylsi", 0, 0);
		dsplbuf.prnt(7, 0, "1", 0, 0);
		dsplbuf.prnt(7, 1, NULL, 9, 0);
		dsplbuf.prnt(7, 2, "999", 0, 0);
		dsplbuf.prnt(7, 8, NULL, 5, 0);
		dsplbuf.prnt(7, 9, "%03d", br_imp, 1);
		dsplbuf.prnt(7, 12, NULL, 4, 0);
		cory.setDiap(7, 7, 9, 11);
		set_st = 1;
	}
	else if(set_st == 1) {
		byte cor = cory.getPoz();
		switch(key) {
			case VK_UP:{
				if(*(setstr + cor) == 57) {
					*(setstr + cor) = 48;
				}
				else {
					*(setstr + cor) += 1;
				}
				break;
			}
			case VK_DOWN:{
				if(*(setstr + cor) == 48) {
					*(setstr + cor) = 57;
				}
				else {
					*(setstr + cor) -= 1;
				}
				break;
			}
			case VK_RETURN:{
				
				short xx = ((short)*setstr - 48) * 100 + ((short)(*(setstr + 1)) - 48) * 10 + (short)(*(setstr + 2)) - 48;
				if(xx > 999 || xx < 0) {
					dsplbuf.prnt(7, 0, "err xx == % 6d", xx, 1);
					set_st = 2;
					return 0;
				}
				else {
					br_imp = xx;
					dija_vk_return();
					set_st = 0;
#ifdef NEW_MEM
					save(0, (byte)102);
#endif
#ifdef WIFI8266
					updt_setup = 1;
#endif
					return 0;
				}
				break;
			}
			default:{
				break;
			}
		}
	}
	if(set_st == 2) {
		if(key == VK_RETURN) {
			set_st = 0;
			return 1;
		}
		else {
			return 2;
		}
	}
	return 1;
}


void lichylnyk_imp::prnt(byte y, byte x) {
	dsplbuf.prnt(y, x, "%3d ", br_imp, 1);
	dsplbuf.prnt(y, x + 4, "%3d", imp, 1);
	return;
}


short int lichylnyk_imp::load(int x) {
	x = batjarelay::load(x);
#ifdef NEW_MEM
	br_imp = (short int)readEEPROM(x, _SHORT);
	x += _SHORT;
	imp = (short int)readEEPROM(x, _SHORT);
	x += _SHORT;
#else
	br_imp = readData(x);
	imp = readData(x + 1);
	x += 2;
#endif
	if(imp <= 0 || imp > br_imp) {
		dija_vk_return();
	}
	return x;
}

#ifdef NEW_MEM

short int lichylnyk_imp::save(int x, byte save_fl) {
	x = batjarelay::save(x, save_fl);
	if(save_fl == 102) {
#ifdef BACKLIGHT_CONTROL
		digitalWrite(BLC_PIN, 0);
#endif
		writeEEPROM(x, _SHORT, (long int)br_imp);
		writeEEPROM(x + _SHORT, _SHORT, (long int)br_imp);
#ifdef BACKLIGHT_CONTROL
		digitalWrite(BLC_PIN, 1);
#endif
	}
	if(save_fl == 103) {
		if(imp != 0)
			writeEEPROM(x + _SHORT, _SHORT, (long int)imp);
		else
			writeEEPROM(x + _SHORT, _SHORT, (long int)br_imp);
	}
	return x + _SHORT + _SHORT;
}

#else
	
short int lichylnyk_imp::save(int x) {
	x = batjarelay::save(x);
	writeData(x, (long int)br_imp);
	writeData(x + 1, (long int)imp);
	return x + 2;
}

#endif


void lichylnyk_imp::dija_vk_return() {
	tmr_autoreset = 0;
	spracjuvalo = false;
	imp = br_imp;
	reset_relay();
	return;
}

