byte null_tek_r();
byte admset(char key, byte navihacija_menu);
#ifdef WIFI8266
byte wifiset(char key, byte navihacija_menu);
#endif

/*

0000			20 20 20 31 3A 30 33 04 05 20 20 20 30 3A 30 32 		prnt_scrn
0010			20 32 35 20 20 32 35 04 05 33 30 30 2E 30 73 DB 
0020			DB DB DB 7E DB 0A DB 04 05 31 37 39 2E 38 6D 08 
0030			52 31 0A 20 52 32 0A 04 05 20 20 20 20 20 20 20 
0040			20 20 20 20 20 20 20 04 05 20 20 20 20 20 20 20 
0050			20 20 20 20 20 20 20 04 05 20 20 20 20 20 20 20 
0060			20 20 20 20 20 20 20 04 05 20 20 20 20 20 20 20 
0070			20 20 20 20 20 20 20 04 05 20 20 20 20 20 20 20 

0080			4C 0E 00 00 											obscht_r
0084						78 00 00 00 								tek_r
0088									00 08 							in
008A										  00 00 					in_mem
008C												07 					cory (y, x)
008D												   00 				navihacija_menu
008E													  08 			rezhym_tmr
008F														 00 		rz_1:	b0...b5, spracjuvalo
0090			02 00 00 00 00 00 00 00 08 00 00 01 
009C												00 00 00 00 		rz_2
				00 00 00 00 00 00 00 00 00 
00A9									   00 00 00 00 00 00 00 		rz_3
				00 00 00 00 00 00 
00B6							  00 00 00 00 00 40 00 00 00 60 		ri_1:	b0...b5, br_imp, imp, spracjuvalo
				00 64 19 00 00 02 00 
00C7								 00 00 00 00 00 00 00 00 00 		ri_2
				00 00 00 E7 03 E7 03 00 
00D8									00 00 00 00 00 00 00 00 		ri_3
				00 00 00 00 E7 03 E7 03 00 
00E9									   00 80 00 00 00 04 00 		rv_1: 	b0...b5, time_s, time, spracjuvalo, vidlik, min
				04 00 60 00 62 4C 1D 4C 1D 00 00 00 
00FC												00 80 00 00 		rv_2
				00 00 00 00 00 60 00 62 94 11 80 11 00 01 01 
010F														 00 		rv_3
				00 00 00 00 00 00 00 00 00 00 00 4C 1D 4C 1D 00 
				00 00 
0122				  0C 0D 04 00 0E 01 08 0F 0F 0F 0F 0F 0F 0F 		intrfs
				0F 0F

0132				  AA BB CC DD EE									contr slovo
0137								 FF									contr sum
0138									AA BB CC DD						sn_esp
013C												11 22 33 44			sn_mega
*/