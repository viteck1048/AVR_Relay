

class batjarelay{
	protected:
		short b0, b1; // !res(enable)
		short b2, b3; // clock(start)
		short b4, b5; // [12 - 0] - 
		bool spracjuvalo;
		byte tmr_ignor, tmr_autoreset;
		bool _reakcija_imp();
		short point;
	public:
		static byte set_st;
		batjarelay();
		bool reakcija_res();
		void tmr_count();
		bool reakcija_imp();
		void set_relay_dija_1();
		void set_relay_dija_2();
		void reset_relay();
		byte admset(char key, byte navihacija_menu);
		void nullset() {
			b0 = b1 = b2 = b3 = b4 = b5 = 0;
		}
		virtual int operator --(int);
		virtual void prnt(byte y, byte x);
		virtual byte setup(char key);
		virtual short int load(int x);
		friend byte reedSerial();
		friend byte writeSerial();
#ifdef NEW_MEM		
		virtual short int save(int x, byte save_fl);
#else		
		virtual short int save(int x);
#endif		
		virtual void dija_vk_return();
};
