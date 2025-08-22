#include "configboard.h"

char getkeyreturn()
{
	if(!digitalRead(KEY_PIN_DIGITAL)) {
		if(analogRead(KEY_PIN) <= 5) {
			return 1;
		}
	}
	return 0;
}


char getkey()
{
#ifdef TIME_CONTROL
//	digitalWrite(CONTROL_OUT_2, 1);
#endif
	static char st, old = 0;
	int ff = analogRead(KEY_PIN);
	if(ff >= 730 && ff <= 742) st = VK_RIGHT;//736, 738
	else if(ff >= 536 && ff <= 548) st = VK_DOWN;//541, 542
	else if(ff >= 295 && ff <= 307) st = VK_UP;//301, 301
	else if(ff >= 170 && ff <= 182) st = VK_LEFT;//176, 175
	else if(ff <= 5) st = VK_RETURN;//0
	else st = 0;
	if(st == old && old != 0) st = 0;
	else if(st != 0) old = st;
	if(ff == 1023) old = 0;
	
	if(st) {
		napruga_z_klavy = ff;
	}
#ifdef TIME_CONTROL
//	digitalWrite(CONTROL_OUT_2, 0);
#endif
	return st;
}
