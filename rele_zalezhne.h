

class rele_zalezhnosti : public batjarelay{
	public:
		int operator --(int);
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