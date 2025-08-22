/* 

struct corX{
	byte x, maxX, minX;
	byte operator --(int) {
		if(x == minX) {
			x = maxX;
		}
		else {
			x--;
		}
		return x;
	}
	byte operator ++(int) {
		if(x == maxX) {
			x = minX;
		}
		else {
			x++;
		}
		return x;
	}
	byte operator =(byte xx) {
		if(xx >= minX && xx <= maxX) {
			return x = xx;
		}
		else {
			return x;
		}
	}
	corX() {
		x = 0;
		maxX = 15;
		minX = 0;
	}
	void setMax(byte min, byte max) {
		maxX = max;
		minX = min;
	}
};


struct corY{
	byte y, maxY, minY;
	byte operator --(int) {
		if(y == minY) {
			y = maxY;
		}
		else {
			y--;
		}
		return y;
	}
	byte operator ++(int) {
		if(y == maxY) {
			y = minY;
		}
		else {
			y++;
		}
		return y;
	}
	byte operator =(byte yy) {
		if(yy >= minY && yy <= maxY) {
			return y = yy;
		}
		else {
			return y;
		}
	}
	corY() {
		y = 0;
		maxY = 1;
		minY = 0;
	}
	void setMax(byte min, byte max) {
		maxY = max;
		minY = min;
	}
};
 */

class coord{
	private:
		byte x, y;
		byte maxX, minX;
		byte maxY, minY;
//		corY y;
//		corX x;
	public:
		void operator --(int);
		void operator ++(int);
		void operator --();
		void operator ++();
		void view();
		void set(byte y, byte x);
		coord();
		void setDiap(byte minY, byte maxY, byte minX, byte maxX);
		friend void blnkCursor();
		byte get();
		byte getPoz();
		friend byte reedSerial();
		friend byte writeSerial();
};

