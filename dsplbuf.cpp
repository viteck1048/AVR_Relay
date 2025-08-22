#include "configboard.h"

void blnkCursor()
{
	LCD_COORD(cory.y - dsplbuf.view, cory.x);
	lcd.blink();
}


void dspl_buff::prnt_screen()
{
	byte bii;
#ifdef TIME_CONTROL
	digitalWrite(CONTROL_OUT_1, 0);
#endif
	if(time_slot != 0)
		return;
#ifdef TIME_CONTROL
	digitalWrite(CONTROL_OUT_4, 1);
#endif
	for(bii = 0; bii < 2; bii++) {
		LCD_COORD(bii, 0);
		for(byte j = 0; j < 16; j++) {
			lcd.write((byte)screen[bii + view][j]);
		}
	}
#ifdef TIME_CONTROL
	digitalWrite(CONTROL_OUT_4, 0);
#endif
#ifdef WIFI8266__
//	if(!resv_fl) {
		for(bii = 0; bii < 128; bii++) {
			send_buf[bii] = (byte)(*(&(screen[0][0]) + bii));
		}
//	}
#endif	
	if(blnk == 0) {
		lcd.noBlink();
	}else {
		blnkCursor();
#ifdef WIFI8266__
		if(!resv_fl) {
			send_buf[cory.get()] += 128;
		}
#endif
	}
}


void dspl_buff::dsplbuf_clear()
{
	for(bi = 0; bi <= 127; bi++) {
		*(&(screen[0][0]) + bi) = (char)32;
	}
}


dspl_buff::dspl_buff()
{
	dsplbuf_clear();
	blnk = 0;
	view = 0;
}



byte dspl_buff::prnt(byte y, byte x, char* str, int param, bool withParam)
{
	byte xx, bii;
	if(str == NULL) {
		xx = 1;
		if(param == 0xff)
			param = 10;
		buff[0] = (char)param;
	}
	else if(withParam == true) {
		xx = sprintf(buff, str, param);
	}else {
		xx = sprintf(buff, str);
	}
	for(bii = 0; bii < xx; bii++) {
		screen[y][x + bii] = buff[bii];
	}
	return x + xx;
}