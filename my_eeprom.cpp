
#include "configboard.h"

#include <EEPROM.h>

void res_eepr()
{
	for(int i = 0; i < MAX_MEM_EPROM; i++)
		EEPROM.update(i, (char)0);
	load();
}


#ifdef WIFI8266

void writeStringEEPROM(unsigned int p, int length, char* str)
{
	for(byte ii = 0; ii < length && ii < 32; ii++) {
		EEPROM.update(p + ii, str[ii]);
	}
}


int readStringEEPROM(unsigned int p, int length, char* str)
{
	byte ii = 0;
	for(; ii < length && ii < 32; ii++) {
		str[ii] = (char)EEPROM.read(p + ii);
	}
	if(length > ii)
		str[ii] = 0;
	return (int)ii;
}

#endif


unsigned long int readEEPROM(int p, int vtype)
{
	unsigned long int vznach = 0;
	do {
		vtype--;
		vznach <<= 8;
		vznach += (unsigned char)EEPROM.read(p + vtype);
	}while(vtype);
	return vznach;
}


void writeEEPROM(unsigned int p, int vtype, long int vznach)
{
	
	for(; vtype; vtype--) {
		EEPROM.update(p, (char)vznach);
		p++;
		vznach >>= 8;
	}
}

#ifndef NEW_MEM
int inic_eepr(bool start, long int br, ...)
{
	
	unsigned int p;
	unsigned int pp;
	unsigned int qq = (unsigned int)EEPROM[0];
	if(qq <= 0 || qq > 200 || start != 0) {
		p = 1;
		pp = MAX_MEM_EPROM;
		qq = 0;
		EEPROM.update(0, 0);
	}
	else {
		p = (qq - 1) * 3 + 1;
		pp = readEEPROM(p + 1, _SHORT);
		p += 3;
	}
	
	if(br + qq > 200) {
		return -1;
	}
	va_list argList;
	va_start(argList, br);
	
	qq += br;
	
	long int vtype;
	long int vznach;
	for(; br; br--) {
		vtype = (long int)va_arg(argList, long int);
		vznach = (long int)va_arg(argList, long int);
		writeEEPROM(p, _BYTE, vtype);
		pp -= (int)vtype;
		p++;
		writeEEPROM(p, _SHORT, pp);
		p += 2;
		if(pp <= p) {
			va_end(argList);
			return -1;
		}
		writeEEPROM(pp, (int)vtype, vznach);
	}
	va_end(argList);
	writeEEPROM(0, _BYTE, qq);
	return 0;
}


long int readData(unsigned int p)
{
	if(p >= (unsigned char)EEPROM[0]) {
		return -1;
	}
	p = p * 3 + 1;
	int vtype = (int)EEPROM[p];
	p = (unsigned int)readEEPROM(p + 1, _SHORT);
	return readEEPROM(p, vtype);
}


int writeData(unsigned int p, long int vznach)
{
	if(p >= (unsigned char)EEPROM[0]) {
		return -1;
	}
	p = p * 3 + 1;
	int vtype = (int)EEPROM[p];
	p = (unsigned int)readEEPROM(p + 1, _SHORT);
	writeEEPROM(p, vtype, vznach);
	return 0;
}


int addParam(int vtype, long int vznach)
{
	unsigned int qq = (unsigned char)EEPROM[0];
	if(qq > 200) {
		return -1;
	}
	unsigned int p;
	unsigned int pp;
	if(qq == 0) {
		p = 1;
		pp = MAX_MEM_EPROM - vtype;
	}
	else {
		qq--;
		p = qq * 3 + 1;
		pp = readEEPROM(p + 1, _SHORT) - vtype;
		p += 3;
	}
	if(p + 3 >= pp) {
		return -1;
	}
	qq += 2;
	EEPROM.update(0, qq);
	writeEEPROM(p, _BYTE, vtype);
	writeEEPROM(p + 1, _SHORT, pp);
	writeEEPROM(pp, vtype, vznach);
	return 0;
}

#endif


int valid_eeprom()
{
	if(EEPROM[MAX_MEM_EPROM - 1] != (unsigned char)VER_MEM) {
		return 0;
	}
#ifndef NEW_MEM
	for(char i = 0; i < EEPROM[0]; i++) {
		char t = EEPROM[i * 3 + 1];
		unsigned short int p = (unsigned short int)readEEPROM(i * 3 + 2, _SHORT);
		if(t != _BYTE && t != _SHORT && t != _LONG) {
			return 0;
		}
		if(p >= MAX_MEM_EPROM || p < EEPROM[0] * 3 + 1) {
			return 0;
		}
	}
#endif
	return 1;
}

#ifndef NEW_MEM

void load()
{
	if(!valid_eeprom()) {
		inic_eepr(true, 1, _BYTE, (long)VER_MEM);
#ifdef _5A283
		obscht_r = readEEPROM(MAX_MEM_EPROM - _BYTE - _LONG, _LONG);
		inic_eepr(false, 1, _LONG, (long)obscht_r);
		tek_r = readEEPROM(MAX_MEM_EPROM - _BYTE - _LONG - _LONG, _LONG);
		inic_eepr(false, 1, _LONG, (long)tek_r);
		
		inic_eepr(false, 2, _LONG, (long)2, _LONG, (long)8);
		inic_eepr(false, 4, _LONG, (long)55, _LONG, (long)1, _LONG, (long)2, _LONG, (long)3);
		inic_eepr(false, 1, _SHORT, (long)0x1954);
		
		inic_eepr(false, 1, _BYTE, (long)0b1100);
		inic_eepr(false, 1, _BYTE, (long)0b1101);
		inic_eepr(false, 1, _BYTE, (long)0b0100);
		inic_eepr(false, 1, _BYTE, (long)0b0000);
		inic_eepr(false, 1, _BYTE, (long)0b1110);
		inic_eepr(false, 1, _BYTE, (long)0b0001);
		inic_eepr(false, 1, _BYTE, (long)0b1000);
		
		for(byte i = 7; i < 16; i++) {
			inic_eepr(false, 1, _BYTE, (long)0x0f);
		}
		
		inic_eepr(false, 8, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0x4000, _SHORT, (long)0, _SHORT, (long)0x6000, _SHORT, (long)0x6400, _SHORT, (long)15, _SHORT, (long)15);	//ri
		inic_eepr(false, 8, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)999, _SHORT, (long)999);	//ri
		inic_eepr(false, 8, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)999, _SHORT, (long)999);	//ri
		
		inic_eepr(false, 7, _SHORT, (long)0x8000, _SHORT, (long)0, _SHORT, (long)0x0400, _SHORT, (long)0x0400, _SHORT, (long)0x6000, _SHORT, (long)0x6200, _SHORT, (long)7500);	//rv
		inic_eepr(false, 7, _SHORT, (long)0x8000, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0x6000, _SHORT, (long)0x6200, _SHORT, (long)0x9194);	//rv
		inic_eepr(false, 7, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)7500);	//rv
		
		inic_eepr(false, 6, _SHORT, (long)0x0200, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0x0800, _SHORT, (long)0);	//rz
		inic_eepr(false, 6, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0);	//rz
		inic_eepr(false, 6, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0);	//rz
#elif defined(ONLY_TIME)
		//inic_eepr(false, 4, _LONG, (long)0, _LONG, (long)0, _BYTE, (long)2, _BYTE, (long)8);
		obscht_r = readEEPROM(MAX_MEM_EPROM - _BYTE - _LONG, _LONG);
		inic_eepr(false, 1, _LONG, (long)obscht_r);
		tek_r = readEEPROM(MAX_MEM_EPROM - _BYTE - _LONG - _LONG, _LONG);
		inic_eepr(false, 1, _LONG, (long)tek_r);
		inic_eepr(false, 2, _LONG, (long)2, _LONG, (long)8);
		inic_eepr(false, 4, _LONG, (long)55, _LONG, (long)1, _LONG, (long)2, _LONG, (long)3);
		inic_eepr(false, 1, _SHORT, (long)0x0000);
		
		inic_eepr(false, 1, _BYTE, (long)0b1101);
		inic_eepr(false, 1, _BYTE, (long)0b1100);
		
//		inic_eepr(false, 1, _BYTE, (long)0b1110);
		
		for(byte i = 2; i < 16; i++) {
			inic_eepr(false, 1, _BYTE, (long)0x0f);
		}
		
		inic_eepr(false, 8, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)999, _SHORT, (long)999);	//ri
		inic_eepr(false, 8, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)999, _SHORT, (long)999);	//ri
		inic_eepr(false, 8, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)999, _SHORT, (long)999);	//ri
		
		inic_eepr(false, 7, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)7500);	//rv
		inic_eepr(false, 7, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)7500);	//rv
		inic_eepr(false, 7, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)7500);	//rv
		
		inic_eepr(false, 6, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0);	//rz
		inic_eepr(false, 6, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0);	//rz
		inic_eepr(false, 6, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0);	//rz
		
#else
		inic_eepr(false, 4, _LONG, (long)0, _LONG, (long)0, _BYTE, (long)10, _BYTE, (long)8);
		inic_eepr(false, 4, _LONG, (long)55, _LONG, (long)1, _LONG, (long)2, _LONG, (long)3);
		inic_eepr(false, 1, _SHORT, (long)0);
		
		for(byte i = 0; i < 16; i++) {
			inic_eepr(false, 1, _BYTE, (long)0x0f);
		}
		
		inic_eepr(false, 8, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)999, _SHORT, (long)999);	//ri
		inic_eepr(false, 8, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)999, _SHORT, (long)999);	//ri
		inic_eepr(false, 8, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)999, _SHORT, (long)999);	//ri
		
		inic_eepr(false, 7, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)7500);	//rv
		inic_eepr(false, 7, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)7500);	//rv
		inic_eepr(false, 7, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)7500);	//rv
		
		inic_eepr(false, 6, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0);	//rz
		inic_eepr(false, 6, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0);	//rz
		inic_eepr(false, 6, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0, _SHORT, (long)0);	//rz
		
#endif
		load();
	}
	else{
		obscht_r = (unsigned long int)readData(1);
		tek_r = (unsigned long int)readData(2);
		jaskravist = (char)readData(3);
		rezhym_tmr = (char)readData(4);

		test_time = (unsigned long int)readData(5);
		sprac_r_1 = (unsigned long int)readData(6);
		sprac_r_2 = (unsigned long int)readData(7);
		sprac_r_3 = (unsigned long int)readData(8);
		
		adminpass = (unsigned short int)readData(PASS);
#ifdef NO_ADMINPASS		
		adminpass = 0;
#endif
		for(byte i = 0; i < 16; i++) {
			arr_golovne_menu[i] = (byte)readData(PASS + 1 + i);
#ifdef VALID_MENU			
			if((arr_golovne_menu[i] & (byte)0xf0) != 0) {
				EEPROM.update(MAX_MEM_EPROM - 1, (char)(VER_MEM + 1));
				load();
				return;
			}
#endif			
		}
		
		short int pp = PASS + 1 + 16;
		pp = ri[0].load(pp);
		pp = ri[1].load(pp);
		pp = ri[2].load(pp);
		pp = rv[0].load(pp);
		pp = rv[1].load(pp);
		pp = rv[2].load(pp);
		pp = rz[0].load(pp);
		pp = rz[1].load(pp);
		pp = rz[2].load(pp);
	}
}

void save()
{
//	cli();
	
	writeData(1, (long int)obscht_r);
	writeData(2, (long int)tek_r);
	writeData(3, (long int)jaskravist);
	writeData(4, (long int)rezhym_tmr);
	
	writeData(5, (long int)test_time);
	writeData(6, (long int)sprac_r_1);
	writeData(7, (long int)sprac_r_2);
	writeData(8, (long int)sprac_r_3);
	
	writeData(PASS, (long int)adminpass);
	
	for(byte i = 0; i < 16; i++) {
		writeData(PASS + 1 + i, (long int)arr_golovne_menu[i]);
	}
	
	short int pp = 16 + PASS + 1;
	pp = ri[0].save(pp);
	pp = ri[1].save(pp);
	pp = ri[2].save(pp);
	pp = rv[0].save(pp);
	pp = rv[1].save(pp);
	pp = rv[2].save(pp);
	pp = rz[0].save(pp);
	pp = rz[1].save(pp);
	pp = rz[2].save(pp);
	
}


#else
	

bool load()
{
	test_time = 0;
	sprac_r_1 = 0;
	sprac_r_2 = 0;
	sprac_r_3 = 0;
	short int point = 0;
	if(!valid_eeprom()) {
#ifdef WIFI8266
		writeEEPROM(ADR_PORT, _LONG, 0x00000000);
#endif
		writeEEPROM(MAX_MEM_EPROM - 1, _BYTE, (long int)VER_MEM);
		writeEEPROM(point, _LONG, (long int)0xe4c);		//obscht_r
		point += _LONG;
		writeEEPROM(point, _LONG, (long int)0x78);		//tek_r
		point += _LONG;
#ifdef _5A283		
		writeEEPROM(point, _BYTE, (long int)2);		//jaskravist
		point += _BYTE;
		writeEEPROM(point, _BYTE, (long int)8);		//rezhym_tmr
		point += _BYTE;
		writeEEPROM(point, _SHORT, (long int)0x1954);		//adminpass
		point += _SHORT;
		writeEEPROM(point, _BYTE, (long int)0b1100);		//arr_golovne_menu[]
		point += _BYTE;
		writeEEPROM(point, _BYTE, (long int)0b1101);		//arr_golovne_menu[]
		point += _BYTE;
		writeEEPROM(point, _BYTE, (long int)0b0100);		//arr_golovne_menu[]
		point += _BYTE;
		writeEEPROM(point, _BYTE, (long int)0b0000);		//arr_golovne_menu[]
		point += _BYTE;
		writeEEPROM(point, _BYTE, (long int)0b1110);		//arr_golovne_menu[]
		point += _BYTE;
		writeEEPROM(point, _BYTE, (long int)0b0001);		//arr_golovne_menu[]
		point += _BYTE;
		writeEEPROM(point, _BYTE, (long int)0b1000);		//arr_golovne_menu[]
		point += _BYTE;
		for(byte i = 7; i < 16; i++) {
			writeEEPROM(point, _BYTE, (long int)0x0f);		//arr_golovne_menu[]
			point += _BYTE;
		}
		writeEEPROM(point, _SHORT, (long int)0);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0x4000);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0x6000);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0x6400);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)15);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)15);		//ri_1
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)999);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)999);		//ri_2
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)999);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)999);		//ri_3
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0x8000);		//rv_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0x0400);		//rv_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0x0400);		//rv_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0x6000);		//rv_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0x6200);		//rv_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)7500);		//rv_1
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0x8000);		//rv_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0x6000);		//rv_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0x6200);		//rv_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0x9194);		//rv_2
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//rv_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)7500);		//rv_3
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0x0200);		//rz_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0x0800);		//rz_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_1
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//rz_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_2
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//rz_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_3
		point += _SHORT;
		
#elif defined(ONLY_TIME)		
		
		writeEEPROM(point, _BYTE, (long int)2);		//jaskravist
		point += _BYTE;
		writeEEPROM(point, _BYTE, (long int)8);		//rezhym_tmr
		point += _BYTE;
		writeEEPROM(point, _SHORT, (long int)0);		//adminpass
		point += _SHORT;
		writeEEPROM(point, _BYTE, (long int)0b1101);		//arr_golovne_menu[]
		point += _BYTE;
		writeEEPROM(point, _BYTE, (long int)0b1100);		//arr_golovne_menu[]
		point += _BYTE;
		for(byte i = 2; i < 16; i++) {
			writeEEPROM(point, _BYTE, (long int)0x0f);		//arr_golovne_menu[]
			point += _BYTE;
		}
		writeEEPROM(point, _SHORT, (long int)0);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)999);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)999);		//ri_1
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)999);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)999);		//ri_2
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)999);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)999);		//ri_3
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//rv_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)7500);		//rv_1
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//rv_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)7500);		//rv_2
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//rv_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)7500);		//rv_3
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//rz_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_1
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//rz_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_2
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//rz_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_3
		point += _SHORT;
		
#else
		writeEEPROM(point, _BYTE, (long int)10);		//jaskravist
		point += _BYTE;
		writeEEPROM(point, _BYTE, (long int)8);		//rezhym_tmr
		point += _BYTE;
		writeEEPROM(point, _SHORT, (long int)0);		//adminpass
		point += _SHORT;
		for(byte i = 0; i < 16; i++) {
			writeEEPROM(point, _BYTE, (long int)0x0f);		//arr_golovne_menu[]
			point += _BYTE;
		}
		writeEEPROM(point, _SHORT, (long int)0);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)999);		//ri_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)999);		//ri_1
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)999);		//ri_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)999);		//ri_2
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)999);		//ri_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)999);		//ri_3
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//rv_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)7500);		//rv_1
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//rv_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)7500);		//rv_2
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//rv_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rv_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)7500);		//rv_3
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//rz_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_1
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_1
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//rz_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_2
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_2
		point += _SHORT;
		
		writeEEPROM(point, _SHORT, (long int)0);		//rz_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_3
		point += _SHORT;
		writeEEPROM(point, _SHORT, (long int)0);		//rz_3
		point += _SHORT;
		
#endif		
	}
	point = 0;
	
	obscht_r = (unsigned long int)readEEPROM(point, _LONG);
	point += _LONG;
	tek_r = (unsigned long int)readEEPROM(point, _LONG);
	point += _LONG;
	jaskravist = (char)readEEPROM(point, _BYTE);
	point += _BYTE;
	rezhym_tmr = (char)readEEPROM(point, _BYTE);
	point += _BYTE;
	adminpass = (unsigned short int)readEEPROM(point, _SHORT);
	point += _SHORT;
	
	for(byte i = 0; i < 16; i++) {
		arr_golovne_menu[i] = (byte)readEEPROM(point, _BYTE);
		point += _BYTE;
	}
	
	point = ri[0].load(point);
	point = ri[1].load(point);
	point = ri[2].load(point);
	point = rv[0].load(point);
	point = rv[1].load(point);
	point = rv[2].load(point);
	point = rz[0].load(point);
	point = rz[1].load(point);
	point = rz[2].load(point);
	
#ifdef WIFI8266
	sn_mega = (unsigned long int)readEEPROM(point, _LONG);
	sprac_r_3 = (long) point;
#endif
	return 0;
}

/*

save adminset:
	jaskravist
	adminpass
	rezhym_tmr
	arr_golovne_menu[]
	ri/rv/rz - b0 - b5(batjarelay::save)
	
	.ino(536)

save setup
	ri - br_imp		lichylnik_imp.cpp(82)	dija_vk_return();
	rv - time_s		rv.cpp(124, 130)		dija_vk_return();

save time
	obscht_r, tek_r			null_tek_r();
	
	tek_r
	if(tmr_ == 1) save();
	
save ri all imp
	attachInterrupt(0, save_, RISING);
	
*/


bool save_adminset()
{
#ifdef BACKLIGHT_CONTROL
	digitalWrite(BLC_PIN, 0);
#endif
	short int point = 0;
//	writeEEPROM(point, _LONG, (long int)2);		//obscht_r
	point += _LONG;
//	writeEEPROM(point, _LONG, (long int)8);		//tek_r
	point += _LONG;
	writeEEPROM(point, _BYTE, (long int)jaskravist);		//jaskravist
	point += _BYTE;
	writeEEPROM(point, _BYTE, (long int)rezhym_tmr);		//rezhym_tmr
	point += _BYTE;
	writeEEPROM(point, _SHORT, (long int)adminpass);		//adminpass
	point += _SHORT;
	for(byte i = 0; i < 16; i++) {
		writeEEPROM(point, _BYTE, (long int)arr_golovne_menu[i]);		//arr_golovne_menu[]
		point += _BYTE;
	}
	ri[0].save(0, (byte)101);
	ri[1].save(0, (byte)101);
	ri[2].save(0, (byte)101);
	rv[0].save(0, (byte)101);
	rv[1].save(0, (byte)101);
	rv[2].save(0, (byte)101);
	rz[0].save(0, (byte)101);
	rz[1].save(0, (byte)101);
	rz[2].save(0, (byte)101);
#ifdef BACKLIGHT_CONTROL
	digitalWrite(BLC_PIN, 1);
#endif
	return 0;
}



bool save_time()
{
#ifdef BACKLIGHT_CONTROL
	digitalWrite(BLC_PIN, 0);
#endif
	if(tek_r == 0) {
		writeEEPROM(0, _LONG, (long int)obscht_r);		//obscht_r
		writeEEPROM(_LONG, _LONG, (long int)tek_r);		//tek_r
	}
	else
		writeEEPROM(_LONG, _LONG, (long int)tek_r);		//tek_r
#ifdef BACKLIGHT_CONTROL
	digitalWrite(BLC_PIN, 1);
#endif
	return 0;
}


bool save_itr()
{
	for(byte ji = 0; ji < 3; ji++) {
		ri[ji].save(0, (byte)103);
	}
	return 0;
}


#endif
