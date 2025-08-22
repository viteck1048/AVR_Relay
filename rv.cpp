#include "configboard.h"





int rele_chasu::operator --(int) {
	tmr_count();
	if(reakcija_res()) {
		if(spracjuvalo == false) {
			if(vidlik == true && time > 0) {
				if(min == true) {
					if(minsek != 0) {
						minsek--;
					}
					else {
						minsek = 60;
						time--;
					}
				}
				else {
					time--;
				}
			}
			if(spracjuvalo == false && reakcija_imp()) {
				set_relay_dija_1();
				vidlik = true;
				if((b5 & 0x8000) != 0) {
					time = time_s;
				}
			}
			if(time == 0) {
				spracjuvalo = true;
				vidlik = false;
				set_relay_dija_2();
				tmr_autoreset = ((byte)(b4 >> 10) & 0b00011000);
			}
		}
	}
	else {
		dija_vk_return();
	}
	if(tmr_autoreset == 1) {
		dija_vk_return();
	}
	return 0;
}

byte rele_chasu::setup(char key) {
	char* setstr = dsplbuf.getlaststr() + 8;
	if(set_st == 0) {
		dsplbuf.dsplbuf_clear();
		dsplbuf.prnt(6, 0, " Set vremerele  ", 0, 0);
		dsplbuf.prnt(7, 0, "0.2", 0, 0);
		dsplbuf.prnt(7, 3, NULL, 9, 0);
		dsplbuf.prnt(7, 4, "999", 0, 0);
		dsplbuf.prnt(7, 7, NULL, 5, 0);
		dsplbuf.prnt(7, 8, "%03d.", time_s / 25, 1);
		dsplbuf.prnt(7, 12, "%d", ((time_s / 5) << 1) % 10, 1);
		dsplbuf.prnt(7, 13, NULL, min == true ? 0x6d : 0x73, 0);
		dsplbuf.prnt(7, 14, NULL, 4, 0);
		cory.setDiap(7, 7, 8, 13);
		set_st = 1;
	}
	else if(set_st == 1) {
		byte cor = cory.getPoz();
		switch(key) {
			case VK_UP:{
				if(*(setstr + cor) == 0x73) {
					*(setstr + cor) = 0x6d;
					min = true;
					break;
				}
				else if(*(setstr + cor) == 0x6d) {
					*(setstr + cor) = 0x73;
					min = false;
					break;
				}
				else if(cor == 3) {
					break;
				}
				else if(*(setstr + cor) == 57) {
					*(setstr + cor) = 48;
				}
				else {
					*(setstr + cor) += 1;
				}
				break;
			}
			case VK_DOWN:{
				if(*(setstr + cor) == 0x73) {
					*(setstr + cor) = 0x6d;
					min = true;
					break;
				}
				else if(*(setstr + cor) == 0x6d) {
					*(setstr + cor) = 0x73;
					min = false;
					break;
				}
				else if(cor == 3) {
					break;
				}
				else if(*(setstr + cor) == 48) {
					*(setstr + cor) = 57;
				}
				else {
					*(setstr + cor) -= 1;
				}
				break;
			}
			case VK_RETURN:{
				
				short xx = ((short)*setstr - 48) * 100 + ((short)(*(setstr + 1)) - 48) * 10 + (short)(*(setstr + 2)) - 48;
				xx *= 25;
				xx += (((short)(*(setstr + 4)) - 48) >> 1) * 5;
				if(xx > 25000 || xx < 0) {
					//dsplbuf.prnt(7, 0, "err xx == % 6d", xx, 1);
					//return 1;
					xx = 125;
					min = false;
					time_s = xx;
					dija_vk_return();
					set_st = 0;
					return 0;
				}
				else {
					time_s = xx;
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
	return 1;
}


void rele_chasu::prnt(byte y, byte x) {
	dsplbuf.prnt(y, x, "%3d", time / 25, 1);
	dsplbuf.prnt(y, x + 3, ".%d", ((time / 5) << 1) % 10, 1);
	if((time / 5) == 0 && vidlik == true) {
		dsplbuf.prnt(y, x + 4, "1", 0, 0);
	}
	dsplbuf.prnt(y, x + 5, NULL, min == true ? 0x6d : 0x73, 0);
	dsplbuf.prnt(y, x + 6, NULL, vidlik == false ? (spracjuvalo == false ? 0xdb : 0xff) : 8, 1);
	return;
}


short int rele_chasu::load(int x) {
	x = batjarelay::load(x);
#ifdef NEW_MEM
	time_s = (short int)readEEPROM(x, _SHORT);
	x += _SHORT;
#else
	time_s = readData(x);
	x++;
#endif
	if(bit(time_s, 15) != 0) {
		time_s &= 0x7fff;
		min = true;
	}
	else{
		min = false;
	}
	dija_vk_return();
	return x;
}

#ifdef NEW_MEM

short int rele_chasu::save(int x, byte save_fl) {
	x = batjarelay::save(x, save_fl);
	if(save_fl == 102) {
#ifdef BACKLIGHT_CONTROL
	digitalWrite(BLC_PIN, 0);
#endif
		if(min == true) {
			time_s |= 0x8000;
		}
		writeEEPROM(x, _SHORT, (long int)time_s);
		time_s &= 0x7fff;
#ifdef BACKLIGHT_CONTROL
	digitalWrite(BLC_PIN, 1);
#endif
	}
	return x + _SHORT;
}

#else
	
short int rele_chasu::save(int x) {
	x = batjarelay::save(x);
	if(min == true) {
		time_s |= 0x8000;
	}
	writeData(x, (long int)time_s);
	time_s &= 0x7fff;
	return x + 1;
}

#endif


void rele_chasu::dija_vk_return() {
	tmr_autoreset = 0;
	spracjuvalo = false;
	minsek = 60;
	vidlik = false;
	time = time_s & 0x7fff;
	reset_relay();
	return;
}

