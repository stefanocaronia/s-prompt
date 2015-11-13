#pragma once
#include <windows.h>
#include <iostream>
#include "globals.h"

inline void setcolor(concol textcol, concol backcol);
inline void setcolor(int textcol, int backcol);
int textcolor();
int backcolor();

#define std_con_out GetStdHandle(STD_OUTPUT_HANDLE)

using namespace std;

int textcolor() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(std_con_out,&csbi);
	int a=csbi.wAttributes;
	return a%16;
}

int backcolor() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(std_con_out,&csbi);
	int a=csbi.wAttributes;
	return (a/16)%16;
}

inline void setcolor(concol textcol, concol backcol) {
	setcolor(int(textcol),int(backcol));
}

inline void setcolor(int textcol, int backcol) {
	textcol%=16;
	backcol%=16;
	unsigned short wAttributes=((unsigned)backcol<<4)|(unsigned)textcol;
	SetConsoleTextAttribute(std_con_out, wAttributes);
}

ostream& operator<<(ostream& os,concol c) {
	os.flush();
	setcolor(c,backcolor());
	return os;
}

istream& operator>>(istream& is,concol c) {
	cout.flush();
	setcolor(c,backcolor());
	return is;
}

