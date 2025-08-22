#include "configboard.h"


byte coord::getPoz() {
	return (y - minY) * (maxX - minX + 1) + x - minX;
}




void coord::view()
{
	if(y < dsplbuf.getView()) {
		dsplbuf.setView(y);
	}
	else if(y > dsplbuf.getView() + 1) {
		dsplbuf.setView(y - 1);
	}
}


byte coord::get() {
	return x + (y << 4);
}


void coord::setDiap(byte minY, byte maxY, byte minX, byte maxX) {
	if(this->minX != minX || this->maxX != maxX || this->minY != minY || this->maxY != maxY) {
		set(minY, minX);
	}
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
}


coord::coord() {
	x = 7;
	y = 0;
	minX = 0;
	maxX = 15;
	minY = 0;
	maxY = 7;
}


void coord::set(byte y, byte x) {
	this->x = x;
	this->y = y;
	view();
}


void coord::operator --(int) {
	if(x == minX) {
		this->operator--();
		x = maxX;
	}
	else
		x--;
	view();
}


void coord::operator ++(int) {
	if(x == maxX) {
		this->operator++();
		x = minX;
	}
	else
		x++;
	view();
}


void coord::operator --() {
	if(y == minY)
		y = maxY;
	else
		y--;
	view();
}



void coord::operator ++() {
	if(y == maxY)
		y = minY;
	else
		y++;
	view();
}




/* 

void coord::view()
{
	if(y.y < dsplbuf.getView()) {
		dsplbuf.setView(y.y);
	}
	else if(y.y > dsplbuf.getView() + 1) {
		dsplbuf.setView(y.y - 1);
	}
}


byte coord::get() {
	return x.x + (y.y << 4);
}


void coord::setDiap(byte minX, byte maxX, byte minY, byte maxY) {
	if(x.minX != minX || x.maxX != maxX || y.minY != minY || y.maxY != maxY) {
		set(minX, minY);
	}
	x.setMax(minX, maxX);
	y.setMax(minY, maxY);
}


coord::coord() {
	x = 7;
	y = 0;
}


void coord::set(byte xx, byte yy) {
	x = xx;
	y = yy;
	view();
}


void coord::operator --(int) {
	if(x.x == x.minX)
		y--;
	x--;
	view();
}


void coord::operator ++(int) {
	if(x.x == x.maxX)
		y++;
	x++;
	view();
}


void coord::operator --() {
	y--;
	view();
}



void coord::operator ++() {
	y++;
	view();
}


 */