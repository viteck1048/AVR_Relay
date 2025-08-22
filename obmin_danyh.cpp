#include "configboard.h"
#ifdef WIFI8266


byte reedSerial()
{
	if(Serial.available() == 18) {
		Serial.readBytes(resv_buf, 18);
		byte prov = (byte)0b10101010;
		for(byte ii = 0; ii < 17; ii++)
			prov ^= resv_buf[ii];
		if(prov != resv_buf[17]) {
			for(byte ii = 0; ii < 18; ii++)
				resv_buf[ii] = 0;
			return 0;
		}
#ifdef TIME_CONTROL
		digitalWrite(CONTROL_OUT_5, 1);
#endif
		time_slot = resv_buf[16];
		return 1;
	}
	else if(Serial.available() > 18) {
		while (Serial.available() > 0) {
			char junk = Serial.read();
		}
	}
	time_slot = 0;
	return 0;
}



byte writeSerial()
{
	for(bi = 0; bi < 32; bi++)
		send_buf[bi] = 0;
	switch(time_slot) {
		case 0x1:;
		case 0x2:;
		case 0x3:;
		case 0x4: {
			for(bi = 0; bi < 32; bi++) {
				send_buf[bi] = *(&(dsplbuf.screen[0][0]) + bi + (time_slot - 1) * 32);
			}
			break;
		}
		case 0x5: {
			*((long*)(&(send_buf[0]))) = obscht_r;
			*((long*)(&(send_buf[4]))) = tek_r;
			*((short*)(&(send_buf[8]))) = in;
			*((short*)(&(send_buf[10]))) = in_mem;
			send_buf[12] = cory.get();
			send_buf[13] = navihacija_menu;
			send_buf[14] = (byte)rezhym_tmr;
			*((short*)(&(send_buf[15]))) = rv[0].time;
			*((short*)(&(send_buf[17]))) = rv[1].time;
			*((short*)(&(send_buf[19]))) = rv[2].time;
			*((short*)(&(send_buf[21]))) = ri[0].imp;
			*((short*)(&(send_buf[23]))) = ri[1].imp;
			*((short*)(&(send_buf[25]))) = ri[2].imp;
			send_buf[27] |= rz[0].spracjuvalo == false ? 0 : 0b00100000;
			send_buf[28] |= rv[0].spracjuvalo == false ? 0 : 0b00100000;
			send_buf[27] |= rz[1].spracjuvalo == false ? 0 : 0b00010000;
			send_buf[28] |= rv[1].spracjuvalo == false ? 0 : 0b00010000;
			send_buf[27] |= rz[2].spracjuvalo == false ? 0 : 0b00001000;
			send_buf[28] |= rv[2].spracjuvalo == false ? 0 : 0b00001000;
			send_buf[27] |= ri[0].spracjuvalo == false ? 0 : 0b00000100;
			send_buf[28] |= rv[0].min == false ? 0 : 0b00000100;
			send_buf[27] |= ri[1].spracjuvalo == false ? 0 : 0b00000010;
			send_buf[28] |= rv[1].min == false ? 0 : 0b00000010;
			send_buf[27] |= ri[2].spracjuvalo == false ? 0 : 0b00000001;
			send_buf[28] |= rv[2].min == false ? 0 : 0b00000001;
			send_buf[27] |= rv[0].vidlik == false ? 0 : 0b10000000;
			send_buf[27] |= rv[1].vidlik == false ? 0 : 0b01000000;
			send_buf[28] |= rv[2].vidlik == false ? 0 : 0b01000000;
			*((short*)(&(send_buf[29]))) = adminpass;
			if(updWiFi) {
				updWiFi = 0;
				send_buf[31] = 0xff;
			}
			if(updt_setup) {
				updt_setup = 0;
				send_buf[31] |= 0b00000010;
			}
			if(updt_admset) {
				updt_admset = 0;
				send_buf[31] |= 0b00000001;
			}
			break;
		}
		case 0x10: {
			*((short*)(&(send_buf[0]))) = rz[0].b0;
			*((short*)(&(send_buf[2]))) = rz[0].b1;
			*((short*)(&(send_buf[4]))) = rz[0].b2;
			*((short*)(&(send_buf[6]))) = rz[0].b3;
			*((short*)(&(send_buf[8]))) = rz[0].b4;
			*((short*)(&(send_buf[10]))) = rz[0].b5;
			*((short*)(&(send_buf[12]))) = rz[1].b0;
			*((short*)(&(send_buf[14]))) = rz[1].b1;
			*((short*)(&(send_buf[16]))) = rz[1].b2;
			*((short*)(&(send_buf[18]))) = rz[1].b3;
			*((short*)(&(send_buf[20]))) = rz[1].b4;
			*((short*)(&(send_buf[22]))) = rz[1].b5;
			*((short*)(&(send_buf[24]))) = rz[2].b0;
			*((short*)(&(send_buf[26]))) = rz[2].b1;
			*((short*)(&(send_buf[28]))) = rz[2].b2;
			*((short*)(&(send_buf[30]))) = rz[2].b3;
			break;
		}
		case 0x11: {
			*((short*)(&(send_buf[0]))) = rz[2].b4;
			*((short*)(&(send_buf[2]))) = rz[2].b5;
			*((short*)(&(send_buf[4]))) = ri[0].b0;
			*((short*)(&(send_buf[6]))) = ri[0].b1;
			*((short*)(&(send_buf[8]))) = ri[0].b2;
			*((short*)(&(send_buf[10]))) = ri[0].b3;
			*((short*)(&(send_buf[12]))) = ri[0].b4;
			*((short*)(&(send_buf[14]))) = ri[0].b5;
			*((short*)(&(send_buf[16]))) = ri[0].br_imp;
			*((short*)(&(send_buf[18]))) = ri[1].b0;
			*((short*)(&(send_buf[20]))) = ri[1].b1;
			*((short*)(&(send_buf[22]))) = ri[1].b2;
			*((short*)(&(send_buf[24]))) = ri[1].b3;
			*((short*)(&(send_buf[26]))) = ri[1].b4;
			*((short*)(&(send_buf[28]))) = ri[1].b5;
			*((short*)(&(send_buf[30]))) = ri[1].br_imp;
			break;
		}
		case 0x12: {
			*((short*)(&(send_buf[0]))) = ri[2].b0;
			*((short*)(&(send_buf[2]))) = ri[2].b1;
			*((short*)(&(send_buf[4]))) = ri[2].b2;
			*((short*)(&(send_buf[6]))) = ri[2].b3;
			*((short*)(&(send_buf[8]))) = ri[2].b4;
			*((short*)(&(send_buf[10]))) = ri[2].b5;
			*((short*)(&(send_buf[12]))) = ri[2].br_imp;
			*((short*)(&(send_buf[14]))) = rv[0].b0;
			*((short*)(&(send_buf[16]))) = rv[0].b1;
			*((short*)(&(send_buf[18]))) = rv[0].b2;
			*((short*)(&(send_buf[20]))) = rv[0].b3;
			*((short*)(&(send_buf[22]))) = rv[0].b4;
			*((short*)(&(send_buf[24]))) = rv[0].b5;
			*((short*)(&(send_buf[26]))) = rv[0].time_s;
			break;
		}
		case 0x13: {
			*((short*)(&(send_buf[0]))) = rv[1].b0;
			*((short*)(&(send_buf[2]))) = rv[1].b1;
			*((short*)(&(send_buf[4]))) = rv[1].b2;
			*((short*)(&(send_buf[6]))) = rv[1].b3;
			*((short*)(&(send_buf[8]))) = rv[1].b4;
			*((short*)(&(send_buf[10]))) = rv[1].b5;
			*((short*)(&(send_buf[12]))) = rv[1].time_s;
			*((short*)(&(send_buf[14]))) = rv[2].b0;
			*((short*)(&(send_buf[16]))) = rv[2].b1;
			*((short*)(&(send_buf[18]))) = rv[2].b2;
			*((short*)(&(send_buf[20]))) = rv[2].b3;
			*((short*)(&(send_buf[22]))) = rv[2].b4;
			*((short*)(&(send_buf[24]))) = rv[2].b5;
			*((short*)(&(send_buf[26]))) = rv[2].time_s;
			break;
		}
		case 0x14: {
			for(bi = 0; bi < 16; bi++)
				send_buf[bi] = arr_golovne_menu[bi];
			*((short*)(&(send_buf[16]))) = rv[0].time_s;
			*((short*)(&(send_buf[18]))) = rv[1].time_s;
			*((short*)(&(send_buf[20]))) = rv[2].time_s;
			*((short*)(&(send_buf[22]))) = ri[0].br_imp;
			*((short*)(&(send_buf[24]))) = ri[1].br_imp;
			*((short*)(&(send_buf[26]))) = ri[2].br_imp;
			break;
		}
		case 0x31: {
			readStringEEPROM(ADR_SSID, 32, (char*)send_buf);
			break;
		}
		case 0x32: {
			readStringEEPROM(ADR_PASS, 32, (char*)send_buf);
			break;
		}
		case 0x33: {
			readStringEEPROM(ADR_HOST, 32, (char*)send_buf);
			break;
		}
		case 0x34: {
			readStringEEPROM(ADR_IP, 4, (char*)send_buf);
			readStringEEPROM(ADR_SUBNET, 4, (char*)(send_buf + 4));
			readStringEEPROM(ADR_GATE, 4, (char*)(send_buf + 8));
			readStringEEPROM(ADR_DNS1, 4, (char*)(send_buf + 12));
			readStringEEPROM(ADR_DNS2, 4, (char*)(send_buf + 16));
			*((long*)(&(send_buf[20]))) = (long)readEEPROM(ADR_PORT, _LONG);
			send_buf[24] = (byte)readEEPROM(ADR_DHCP, _BYTE);
			*((long*)(&(send_buf[25]))) = sn_mega;
			break;
		}
		case 0x35: {
			readStringEEPROM(ADR_NAME, 32, (char*)send_buf);
			break;
		}
		case 0xe1: {
			rz[0].b0 = *((short*)(&(resv_buf[2])));
			rz[0].b1 = *((short*)(&(resv_buf[4])));
			rz[0].b2 = *((short*)(&(resv_buf[6])));
			rz[0].b3 = *((short*)(&(resv_buf[8])));
			rz[0].b4 = *((short*)(&(resv_buf[10])));
			rz[0].b5 = *((short*)(&(resv_buf[12])));
			break;
		}
		case 0xe2: {
			rz[1].b0 = *((short*)(&(resv_buf[2])));
			rz[1].b1 = *((short*)(&(resv_buf[4])));
			rz[1].b2 = *((short*)(&(resv_buf[6])));
			rz[1].b3 = *((short*)(&(resv_buf[8])));
			rz[1].b4 = *((short*)(&(resv_buf[10])));
			rz[1].b5 = *((short*)(&(resv_buf[12])));
			break;
		}
		case 0xe3: {
			rz[2].b0 = *((short*)(&(resv_buf[2])));
			rz[2].b1 = *((short*)(&(resv_buf[4])));
			rz[2].b2 = *((short*)(&(resv_buf[6])));
			rz[2].b3 = *((short*)(&(resv_buf[8])));
			rz[2].b4 = *((short*)(&(resv_buf[10])));
			rz[2].b5 = *((short*)(&(resv_buf[12])));
			break;
		}
		case 0xe4: {
			ri[0].b0 = *((short*)(&(resv_buf[2])));
			ri[0].b1 = *((short*)(&(resv_buf[4])));
			ri[0].b2 = *((short*)(&(resv_buf[6])));
			ri[0].b3 = *((short*)(&(resv_buf[8])));
			ri[0].b4 = *((short*)(&(resv_buf[10])));
			ri[0].b5 = *((short*)(&(resv_buf[12])));
			break;
		}
		case 0xe5: {
			ri[1].b0 = *((short*)(&(resv_buf[2])));
			ri[1].b1 = *((short*)(&(resv_buf[4])));
			ri[1].b2 = *((short*)(&(resv_buf[6])));
			ri[1].b3 = *((short*)(&(resv_buf[8])));
			ri[1].b4 = *((short*)(&(resv_buf[10])));
			ri[1].b5 = *((short*)(&(resv_buf[12])));
			break;
		}
		case 0xe6: {
			ri[2].b0 = *((short*)(&(resv_buf[2])));
			ri[2].b1 = *((short*)(&(resv_buf[4])));
			ri[2].b2 = *((short*)(&(resv_buf[6])));
			ri[2].b3 = *((short*)(&(resv_buf[8])));
			ri[2].b4 = *((short*)(&(resv_buf[10])));
			ri[2].b5 = *((short*)(&(resv_buf[12])));
			break;
		}
		case 0xe7: {
			rv[0].b0 = *((short*)(&(resv_buf[2])));
			rv[0].b1 = *((short*)(&(resv_buf[4])));
			rv[0].b2 = *((short*)(&(resv_buf[6])));
			rv[0].b3 = *((short*)(&(resv_buf[8])));
			rv[0].b4 = *((short*)(&(resv_buf[10])));
			rv[0].b5 = *((short*)(&(resv_buf[12])));
			break;
		}
		case 0xe8: {
			rv[1].b0 = *((short*)(&(resv_buf[2])));
			rv[1].b1 = *((short*)(&(resv_buf[4])));
			rv[1].b2 = *((short*)(&(resv_buf[6])));
			rv[1].b3 = *((short*)(&(resv_buf[8])));
			rv[1].b4 = *((short*)(&(resv_buf[10])));
			rv[1].b5 = *((short*)(&(resv_buf[12])));
			break;
		}
		case 0xe9: {
			rv[2].b0 = *((short*)(&(resv_buf[2])));
			rv[2].b1 = *((short*)(&(resv_buf[4])));
			rv[2].b2 = *((short*)(&(resv_buf[6])));
			rv[2].b3 = *((short*)(&(resv_buf[8])));
			rv[2].b4 = *((short*)(&(resv_buf[10])));
			rv[2].b5 = *((short*)(&(resv_buf[12])));
			break;
		}
		case 0xea: {
			for(bi = 0; bi < 16; bi++)
				arr_golovne_menu[bi] = resv_buf[bi];
			break;
		}
		case 0xeb: {
			rezhym_tmr = resv_buf[0];
			break;
		}
		case 0xec: {
			sn_mega = *((long*)(&(resv_buf[0])));
			writeEEPROM(sprac_r_3, _LONG, (long int)sn_mega);
			break;
		}
		case 0xe0: {
			adminpass = *((short*)(&(resv_buf[0])));
			break;
		}
		case 0xf1: {
			rz[0].dija_vk_return();
			rz[0].save(0, (byte)102);
			break;
		}
		case 0xf2: {
			rz[1].dija_vk_return();
			rz[1].save(0, (byte)102);
			break;
		}
		case 0xf3: {
			rz[2].dija_vk_return();
			rz[2].save(0, (byte)102);
			break;
		}
		case 0xf4: {
			if(*((short*)(&(resv_buf[0]))) != 0) {
				ri[0].br_imp = *((short*)(&(resv_buf[0])));
				ri[0].dija_vk_return();
				ri[0].save(0, (byte)102);
				updt_setup = 1;
			}
			else
				ri[0].dija_vk_return();
			break;
		}
		case 0xf5: {
			if(*((short*)(&(resv_buf[0]))) != 0) {
				ri[1].br_imp = *((short*)(&(resv_buf[0])));
				ri[1].dija_vk_return();
				ri[1].save(0, (byte)102);
				updt_setup = 1;
			}
			else
				ri[1].dija_vk_return();
			break;
		}
		case 0xf6: {
			if(*((short*)(&(resv_buf[0]))) != 0) {
				ri[2].br_imp = *((short*)(&(resv_buf[0])));
				ri[2].dija_vk_return();
				ri[2].save(0, (byte)102);
				updt_setup = 1;
			}
			else
				ri[2].dija_vk_return();
			break;
		}
		case 0xf7: {
			if(*((short*)(&(resv_buf[0]))) != 0) {
				if(bit(*((short*)(&(resv_buf[0]))), 15) != 0)
					rv[0].min = true;
				else
					rv[0].min = false;
				rv[0].time_s = *((short*)(&(resv_buf[0])));
				rv[0].dija_vk_return();
				rv[0].save(0, (byte)102);
				updt_setup = 1;
			}
			else
				rv[0].dija_vk_return();
			break;
		}
		case 0xf8: {
			if(*((short*)(&(resv_buf[0]))) != 0) {
				if(bit(*((short*)(&(resv_buf[0]))), 15) != 0)
					rv[1].min = true;
				else
					rv[1].min = false;
				rv[1].time_s = *((short*)(&(resv_buf[0])));
				rv[1].dija_vk_return();
				rv[1].save(0, (byte)102);
				updt_setup = 1;
			}
			else
				rv[1].dija_vk_return();
			break;
		}
		case 0xf9: {
			if(*((short*)(&(resv_buf[0]))) != 0) {
				if(bit(*((short*)(&(resv_buf[0]))), 15) != 0)
					rv[2].min = true;
				else
					rv[2].min = false;
				rv[2].time_s = *((short*)(&(resv_buf[0])));
				rv[2].dija_vk_return();
				rv[2].save(0, (byte)102);
				updt_setup = 1;
			}
			else
				rv[2].dija_vk_return();
			break;
		}
		case 0xfa: {
			null_tek_r();
			break;
		}
		case 0xfb: {
			save_adminset();
			updt_admset = 1;
			break;
		}
		case 0xfe: {
			wifi_enable_fl = 250;
			break;
		}
		case 0xff: {
			break;
		}
		case 0x20: {
			char test[] = "abcd+efgj+hlkm+";
			char a = resv_buf[6];
			for(bi = 0; bi < 15; bi++) {
				send_buf[bi] = test[bi];
				send_buf[30 - bi] = test[bi];
			}
			send_buf[15] = a;
			send_buf[31] = 0;
			break;
		}
		default: {
			break;
		}
	}
	if((time_slot & 0xf0) != 0xf0 && (time_slot & 0xf0) != 0xe0) {
		send_buf[32] = (byte)0b10101010;
		for(byte ii = 0; ii < 32; ii++)
			send_buf[32] ^= send_buf[ii];
		Serial.write(send_buf, 33);
	}
	else {
		send_buf[0] = 'O';
		send_buf[1] = 'K';
		if(updWiFi == 1) {
			send_buf[0] = 'W';
			send_buf[1] = 'F';
			send_buf[2] = 0xff;
			updWiFi = 0;
		}
		Serial.write(send_buf, 3);
	}
	time_slot = 0;
	
#ifdef TIME_CONTROL
	digitalWrite(CONTROL_OUT_5, 0);
#endif
	return 0;
}
#endif