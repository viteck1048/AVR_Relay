#include "configboard.h"





int rele_zalezhnosti::operator --(int) {
	tmr_count();
	if(reakcija_res()) {
		set_relay_dija_1();
		if(spracjuvalo == false && reakcija_imp()) {
			set_relay_dija_2();
			spracjuvalo = true;
			tmr_autoreset = ((byte)(b4 >> 10) & 0b00011000);
		}
	}
	else {
		reset_relay();
		spracjuvalo = false;
		tmr_autoreset = 0;
	}
	if(tmr_autoreset == 1) {
		tmr_autoreset = 0;
		spracjuvalo = false;
		reset_relay();
	}
	return 0;
}



byte rele_zalezhnosti::setup(char key) {
	return 0;
}


void rele_zalezhnosti::prnt(byte y, byte x) {
	dsplbuf.prnt(y, x, "R1", 0, 0);
	dsplbuf.prnt(y, x + 2, NULL, reakcija_res() == 1 ? 0xff : 0xdb, 0);
	dsplbuf.prnt(y, x + 3, " R2", 0, 0);
	dsplbuf.prnt(y, x + 6, NULL, spracjuvalo == true ? 0xff : 0xdb, 0);
	return;
}


short int rele_zalezhnosti::load(int x) {
	return batjarelay::load(x);
}


#ifdef NEW_MEM

short int rele_zalezhnosti::save(int x, byte save_fl) {
	return batjarelay::save(x, save_fl);
}

#else

short int rele_zalezhnosti::save(int x) {
	return batjarelay::save(x);
}

#endif


void rele_zalezhnosti::dija_vk_return() {
	spracjuvalo = false;
	tmr_autoreset = 0;
	reset_relay();
	return;
}



