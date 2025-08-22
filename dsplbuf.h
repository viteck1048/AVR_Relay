
class dspl_buff {
	private:
		char screen[8][16];
		byte view;
		bool blnk;
		char buff[17];
	public:
		dspl_buff();
		void onblnk() {
			blnk = 1;
		}
		void offblnk() {
			blnk = 0;
		}
		byte prnt(byte y, byte x, char* str, int param, bool withParam);
		void setView(byte vi) {
			view = vi;
		}
		byte getView() {
			return view;
		}
		void prnt_screen();
		friend void blnkCursor();
		void dsplbuf_clear();
		char* getlaststr() {
			return &(screen[7][0]);
		}
		friend byte reedSerial();
		friend byte writeSerial();
		/* char* getArr() {
			return &(screen[0][0]);
		} */
};