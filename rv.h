
class rele_chasu : public batjarelay{
	private:
		short int time_s, time;
		bool vidlik;
		bool min;
		byte minsek;
	public:
		rele_chasu() {
			time_s = time = 0;
			vidlik = false;
		}
		int operator --(int);
		short int get_time_s() {return (time_s * 10) / 25;}
		short int get_time() {return (time * 10) / 25;}
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
