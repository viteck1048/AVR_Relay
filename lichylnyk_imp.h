
/*
class batjarelay{
	private:
		short b0, b1; // !res(enable)
		short b2, b3; // clock(start)
		short b4, b5; // [12 - 0] - 
		static byte set_st;
		bool spracjuvalo;
		byte tmr_ignor, tmr_autores;
		bool _reakcija_imp();
	public:
		batjarelay() {
			b0 = b1 = b2 = b3 = b4 = b5 = 0;
			set_st = 0;
			spracjuvalo = false;
			tmr_ignor = 0;
			tmr_autores = 0;
		}
		bool reakcija_res();
		void tmr_count();
		bool reakcija_imp();
		void set_relay_dija_1();
		void set_relay_dija_2();
		void reset_relay();
		byte admset(char key, byte navihacija_menu);
		void nullset() {
			b0 = b1 = b2 = b3 = b4 = b5 = 0;
			set_st = 0;
		}
		void autoreset();
}

*/


class lichylnyk_imp : public batjarelay{
	private:
		short int br_imp, imp;
	public:
		lichylnyk_imp() {
			br_imp = imp = 0;
		}
		int operator --(int);
		short int get_br_imp() {return br_imp;}
		short int get_imp() {return imp;}
		void prnt(byte y, byte x);
		byte setup(char key);
		short int load(int x);
		friend byte reedSerial();
		friend byte writeSerial();
#ifdef NEW_MEM		
		short int save(int x, byte save_fl);
#else		
		short int save(int x);
#endif
		void dija_vk_return();
};
