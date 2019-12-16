/**
 * Arkanoid game with ATmega8A and LCD5110
 * 
 * Created: 28. 12. 2015 15:43:04
 * Author: Andy F
 * Video: https://www.youtube.com/watch?v=fJ2O5IXK5po&t=14s
 * My YouTube channel: https://www.youtube.com/channel/UChNqbREySML4ZNs9sl-661Q
 */

#define F_CPU 8000000UL

#define BUZZER 3
#define BL 0
#define RST 6
#define DC 7

#define BW 7
#define BH 6
#define BLOKOV BW*BH
#define LEVELOV 28


#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <math.h>

const unsigned char ZNAK[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00,// (space)
	0x00, 0x00, 0x5F, 0x00, 0x00,// !
	0x00, 0x07, 0x00, 0x07, 0x00,// "
	0x14, 0x7F, 0x14, 0x7F, 0x14,// #
	0x24, 0x2A, 0x7F, 0x2A, 0x12,// $
	0x23, 0x13, 0x08, 0x64, 0x62,// %
	0x36, 0x49, 0x55, 0x22, 0x50,// &
	0x00, 0x05, 0x03, 0x00, 0x00,// '
	0x00, 0x1C, 0x22, 0x41, 0x00,// (
	0x00, 0x41, 0x22, 0x1C, 0x00,// )
	0x08, 0x2A, 0x1C, 0x2A, 0x08,// *
	0x08, 0x08, 0x3E, 0x08, 0x08,// +
	0x00, 0x50, 0x30, 0x00, 0x00,// ,
	0x08, 0x08, 0x08, 0x08, 0x08,// -
	0x00, 0x60, 0x60, 0x00, 0x00,// .
	0x20, 0x10, 0x08, 0x04, 0x02,// /
	0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
	0x00, 0x42, 0x7F, 0x40, 0x00,// 1
	0x42, 0x61, 0x51, 0x49, 0x46,// 2
	0x21, 0x41, 0x45, 0x4B, 0x31,// 3
	0x18, 0x14, 0x12, 0x7F, 0x10,// 4
	0x27, 0x45, 0x45, 0x45, 0x39,// 5
	0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
	0x01, 0x71, 0x09, 0x05, 0x03,// 7
	0x36, 0x49, 0x49, 0x49, 0x36,// 8
	0x06, 0x49, 0x49, 0x29, 0x1E,// 9
	0x00, 0x36, 0x36, 0x00, 0x00,// :
	0x00, 0x56, 0x36, 0x00, 0x00,// ;
	0x00, 0x08, 0x14, 0x22, 0x41,// <
	0x14, 0x14, 0x14, 0x14, 0x14,// =
	0x41, 0x22, 0x14, 0x08, 0x00,// >
	0x02, 0x01, 0x51, 0x09, 0x06,// ?
	0x32, 0x49, 0x79, 0x41, 0x3E,// @
	0x7E, 0x11, 0x11, 0x11, 0x7E,// A
	0x7F, 0x49, 0x49, 0x49, 0x36,// B
	0x3E, 0x41, 0x41, 0x41, 0x22,// C
	0x7F, 0x41, 0x41, 0x22, 0x1C,// D
	0x7F, 0x49, 0x49, 0x49, 0x41,// E
	0x7F, 0x09, 0x09, 0x01, 0x01,// F
	0x3E, 0x41, 0x41, 0x51, 0x32,// G
	0x7F, 0x08, 0x08, 0x08, 0x7F,// H
	0x00, 0x41, 0x7F, 0x41, 0x00,// I
	0x20, 0x40, 0x41, 0x3F, 0x01,// J
	0x7F, 0x08, 0x14, 0x22, 0x41,// K
	0x7F, 0x40, 0x40, 0x40, 0x40,// L
	0x7F, 0x02, 0x04, 0x02, 0x7F,// M
	0x7F, 0x04, 0x08, 0x10, 0x7F,// N
	0x3E, 0x41, 0x41, 0x41, 0x3E,// O
	0x7F, 0x09, 0x09, 0x09, 0x06,// P
	0x3E, 0x41, 0x51, 0x21, 0x5E,// Q
	0x7F, 0x09, 0x19, 0x29, 0x46,// R
	0x46, 0x49, 0x49, 0x49, 0x31,// S
	0x01, 0x01, 0x7F, 0x01, 0x01,// T
	0x3F, 0x40, 0x40, 0x40, 0x3F,// U
	0x1F, 0x20, 0x40, 0x20, 0x1F,// V
	0x7F, 0x20, 0x18, 0x20, 0x7F,// W
	0x63, 0x14, 0x08, 0x14, 0x63,// X
	0x03, 0x04, 0x78, 0x04, 0x03,// Y
	0x61, 0x51, 0x49, 0x45, 0x43,// Z
	0x00, 0x00, 0x7F, 0x41, 0x41,// [
	0x02, 0x04, 0x08, 0x10, 0x20,// "\"
	0x41, 0x41, 0x7F, 0x00, 0x00,// ]
	0x04, 0x02, 0x01, 0x02, 0x04,// ^
	0x40, 0x40, 0x40, 0x40, 0x40,// _
	0x00, 0x01, 0x02, 0x04, 0x00,// `
	0x20, 0x54, 0x54, 0x54, 0x78,// a
	0x7F, 0x48, 0x44, 0x44, 0x38,// b
	0x38, 0x44, 0x44, 0x44, 0x20,// c
	0x38, 0x44, 0x44, 0x48, 0x7F,// d
	0x38, 0x54, 0x54, 0x54, 0x18,// e
	0x08, 0x7E, 0x09, 0x01, 0x02,// f
	0x08, 0x14, 0x54, 0x54, 0x3C,// g
	0x7F, 0x08, 0x04, 0x04, 0x78,// h
	0x00, 0x44, 0x7D, 0x40, 0x00,// i
	0x20, 0x40, 0x44, 0x3D, 0x00,// j
	0x00, 0x7F, 0x10, 0x28, 0x44,// k
	0x00, 0x41, 0x7F, 0x40, 0x00,// l
	0x7C, 0x04, 0x18, 0x04, 0x78,// m
	0x7C, 0x08, 0x04, 0x04, 0x78,// n
	0x38, 0x44, 0x44, 0x44, 0x38,// o
	0x7C, 0x14, 0x14, 0x14, 0x08,// p
	0x08, 0x14, 0x14, 0x18, 0x7C,// q
	0x7C, 0x08, 0x04, 0x04, 0x08,// r
	0x48, 0x54, 0x54, 0x54, 0x20,// s
	0x04, 0x3F, 0x44, 0x40, 0x20,// t
	0x3C, 0x40, 0x40, 0x20, 0x7C,// u
	0x1C, 0x20, 0x40, 0x20, 0x1C,// v
	0x3C, 0x40, 0x30, 0x40, 0x3C,// w
	0x44, 0x28, 0x10, 0x28, 0x44,// x
	0x0C, 0x50, 0x50, 0x50, 0x3C,// y
	0x44, 0x64, 0x54, 0x4C, 0x44,// z
};

const unsigned char LEVELS [] PROGMEM = {
	//LEVEL 1
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0100010, 0b0011100, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
	//LEVEL 2
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b1101011, 0b0010100, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0000000, 0b0000000, 0b1000001, 0b0000000, 0b0000000, 0b0000000,
	//LEVEL 3
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b1111111, 0b1010101, 0b0111110, 0b0001000, 0b0000000, 0b0000000,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
	//LEVEL 4
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b1100011, 0b1100011, 0b1111111, 0b1111111, 0b0000000, 0b0000000,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
	//LEVEL 5
0b0000000, 0b1010101, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0000000, 0b0101010, 0b1010101, 0b0000000, 0b1111111, 0b0000000,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
	//LEVEL 6
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b1010000, 0b0101000, 0b0010100, 0b0001010, 0b0000101, 0b0000000,
0b0100000, 0b0010000, 0b0001000, 0b0000100, 0b0000010, 0b0000000,
	//LEVEL 7
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0000000, 0b0001000, 0b0010100, 0b0001000, 0b0001000, 0b0001000,
0b0000000, 0b0000000, 0b0001000, 0b0000000, 0b0000000, 0b0000000,
	//LEVEL 8
0b0000000, 0b0000000, 0b0011100, 0b0000000, 0b0000000, 0b0000000,
0b1111111, 0b1000001, 0b1000001, 0b1000001, 0b1111111, 0b0000000,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
	//LEVEL 9
0b0000000, 0b0000000, 0b0001000, 0b0011100, 0b0000000, 0b0000000,
0b0001000, 0b0011100, 0b0110110, 0b1100011, 0b1111111, 0b0011100,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
	//LEVEL 10
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b1100011, 0b0000000,
0b0101010, 0b0011100, 0b0110110, 0b1100011, 0b0011100, 0b0000000,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
	//LEVEL 11
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0010011, 0b0100110, 0b1001100, 0b0011001, 0b0110010, 0b1100100,
0b0100000, 0b1000000, 0b0000000, 0b0000000, 0b0000001, 0b0000010,
	//LEVEL 12
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0111110, 0b0001100, 0b0011000, 0b0111110, 0b0000000, 0b1111111,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
	//LEVEL 13
0b0000000, 0b0000000, 0b0000000, 0b0011100, 0b0000000, 0b0000000,
0b0111110, 0b0001000, 0b0011100, 0b0000000, 0b0000000, 0b0000000,
0b0000000, 0b0010100, 0b0000000, 0b0000000, 0b1111111, 0b0000000,
	//LEVEL 14
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b1111111, 0b1010101, 0b1001001, 0b1010101, 0b1000001, 0b1111111,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0100010, 0b0000000,
	//LEVEL 15
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b1010101, 0b0101010, 0b1010101, 0b0101010, 0b1010101, 0b0101010,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
	//LEVEL 16
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0101010, 0b1111111, 0b0000001, 0b1111111, 0b1000000, 0b1111111,
0b1010101, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
	//LEVEL 17
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0101010, 0b1010101, 0b0000000, 0b0101010, 0b0000000, 0b0000000,
0b1010101, 0b0101010, 0b1010101, 0b0000000, 0b0000000, 0b0000000,
	//LEVEL 18
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0011100, 0b0000000,
0b0011100, 0b0101010, 0b0111110, 0b0110110, 0b0100010, 0b0000000,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0011100,
	//LEVEL 19
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0000001, 0b0000011, 0b0000111, 0b0001111, 0b0011111, 0b0111111,
0b0000010, 0b0000100, 0b0001000, 0b0010000, 0b0100000, 0b1000000,
	//LEVEL 20
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0000000, 0b1000001, 0b0000000, 0b1000001, 0b0000000, 0b1100011,
0b1111111, 0b0000000, 0b1111111, 0b0000000, 0b1111111, 0b0000000,
	//LEVEL 21
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0000011, 0b0001111, 0b0001111, 0b0001111, 0b0001111, 0b0001111,
0b0000000, 0b0010000, 0b0010000, 0b0010000, 0b0010000, 0b0010000,
	//LEVEL 22
0b0000000, 0b0000000, 0b0011100, 0b0011100, 0b0000000, 0b0000000,
0b1111111, 0b1100011, 0b1100011, 0b1100011, 0b1100011, 0b1111111,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
	//LEVEL 23
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b1111111, 0b1111111, 0b1111111, 0b1111111, 0b1111111, 0b1111111,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
	//LEVEL 24
0b1111111, 0b1000000, 0b1001111, 0b1000000, 0b1000000, 0b1111110,
0b0000000, 0b0001000, 0b0010000, 0b0101010, 0b0010101, 0b0000001,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
	//LEVEL 25
0b1111111, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
0b0000000, 0b0000000, 0b0011100, 0b0011100, 0b0000000, 0b0001000,
0b0000000, 0b1111111, 0b1100011, 0b1100011, 0b1111111, 0b1000001,
	//LEVEL 26
0b0100010, 0b0100010, 0b0000000, 0b0000000, 0b0100010, 0b0011100,
0b0001000, 0b0011100, 0b0111110, 0b0010100, 0b0011100, 0b0000000,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
	//LEVEL 27
0b0000000, 0b0010100, 0b0001000, 0b0001000, 0b0010100, 0b0000000,
0b1111111, 0b1101011, 0b1110111, 0b1110111, 0b1101011, 0b1111111,
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b0000000,
	//LEVEL 28
0b0000000, 0b0000000, 0b0000000, 0b0000000, 0b1100011, 0b0000000,
0b1010101, 0b0101010, 0b1010101, 0b0101010, 0b0000000, 0b1111111,
0b0101010, 0b1010101, 0b0101010, 0b1010101, 0b0011100, 0b0000000
};

unsigned char canvas [504];
char repaint;
int level;
char bloky [BLOKOV];

void pin(int* port, int no, int val) {
	if(val)
		*port |= (1<<no);
	else
		*port &= ~(1<<no);
}

void lcdCmd(int cmd) {
	pin(&PORTD, DC, 0);		//DC pin is low for commands
	SPDR = cmd;				//transmit serial data
	while(!(SPSR & (1<<SPIF)))
		;
}

void lcdData(int data) {
	pin(&PORTD, DC, 1);		//DC pin is high for data
	SPDR = data;				//transmit serial data
	while(!(SPSR & (1<<SPIF)))
		;
}

void gotoXY(int x, int y) {
	if(x >= 0 && x < 84)
		lcdCmd(0x80 | x);
	if(y >= 0 && y < 6)
		lcdCmd(0x40 | y);
}

void drawPoint(char x, char y) {
	if(x > 84 || y > 48)
		return;
	
	canvas[(y/8)*84 + x] |= (1<<(y%8));
	repaint = 1;
}

void drawRect(char l, char t, char r, char b, char full) {
	int i, j;
	for(i = l; i < r; i++) {
		for(j = t; j < b; j++) {
			if(full == 2 || i==l || j==t || i==r-1 || j==b-1 || (full>2 && (i%(full>3? 2 : 3))^(j%2)==1))
				drawPoint(i, j);
		}			
	}
}

void drawCircle(char cx, char cy, float r) {
	int i, j;
	int qr = r*r;
	for(i = -r; i < r; i++) {
		for(j = -r; j < r; j++) {
			if(i*i + j*j < qr)
				drawPoint(i+cx, j+cy);
		}			
	}
}

void drawText(char * text) {
	int i, j;
	for(i = 0; text[i] != 0; i++) {
		for(j = 0; j < 5; j++) {
			lcdData(pgm_read_byte(&ZNAK[(text[i]-0x20)*5+j]));
		}
		lcdData(0x00);
	}
}

void cleanUp() {
	int i;
	for(i = 0; i < 504; i++)
		canvas[i] = 0;
}

void bzzz(int dlho) {
	pin(&PORTC, BUZZER, 0);
	if(dlho)
		_delay_ms(400);
	else
		_delay_ms(50);
	pin(&PORTC, BUZZER, 1);
}

int main(void)
{
	int hra = 2;
	int leti;
	int i;
	float r = 2.7f;
	float ballx, bally;
	float ballv;
	float vx, vy;
	float padx, pady = 45;
	int pads2 = 8;
	int posun;
	int pgs;
	int zivoty = 3;
	//int hackEn = 0;
	int maxL = 0;
	level = 0;
	pin(&PORTC, BUZZER, 1);
	DDRD = 0b11000001;
	DDRC =   0b001000;
	DDRB = 0b00101100;
	PORTD = PORTD|(1<<1)|(1<<3)|(1<<4);
	SPCR = 0b01010001;
	pin(&PORTD, RST, 0);
	_delay_ms(5);
	pin(&PORTD, RST, 1);
	lcdCmd(0x21); // LCD extended commands
	lcdCmd(0xBC); // set LCD Vop (contrast)
	lcdCmd(0x04); // set temp coefficent
	lcdCmd(0x14); // LCD bias mode 1:40
	lcdCmd(0x20); // LCD basic commands
	lcdCmd(0x0C); // LCD normal video mode
	
	for(i = 0; i < 504; i++)
		lcdData(0x00);
	pin(&PORTD, BL, 1);
	gotoXY(9, 1);
	drawText("BALL BOUNCE");
	gotoXY(15, 4);
	drawText("by Andy F");
	_delay_ms(3500);
	while((PIND&(1<<1)) == 0) {
		level++;
		char tl[3];
		tl[0] = (level+1)/10 + '0';
		tl[1] = (level+1)%10 + '0';
		tl[2] = 0;
		gotoXY(36, 3);
		drawText(tl);
		_delay_ms(400);
	}
	
	
	repaint = 1;
	while(1) {
		if(hra == 3)	//prejdene vsetky levely
			continue;
		if(repaint) {
			gotoXY(0, 0);
			if(hra == 1 || (hra == 0 && zivoty > 0)) {
				for(i = 0; i < 504; i++)
					lcdData(canvas[i]|(i>498? pgm_read_byte(&ZNAK[(zivoty+0x10)*5+i-499]) : 0));
			}
			else if(hra == 0) {
				gotoXY(15, 2);
				drawText("GAME OVER");
			}		
			else {
				for(i = 0; i < 504; i++)
					lcdData((i >= 420)? ((i%84 == 0 || i%84 == 83)? 0xFF : (i%84 == 1 || i%84 == 82)? 0x81 : ((i%84)*LEVELOV/84 < level)?  0xBD : 0x81) : 0x00);
				
				gotoXY(20, 2);
				if(level < LEVELOV) {
					char tl[9] = "Level   ";
					if(level < 9)
						tl[6] = level+'1';
					else {
						tl[6] = (level+1)/10 + '0';
						tl[7] = (level+1)%10 + '0';
					}
					drawText(tl);
				}
				else {
					drawText("You win!");
				}
			}			
			repaint = 0;
			cleanUp();
		}
		if(hra == 0 || hra == 2) {
			if(hra == 0 && zivoty-- > 0)
				hra = 1;
			
			leti = 0;
			pgs = 0;
			ballx = 42;
			bally = 42;
			ballv = 0.8f;
			vx = 0.02f;
			vy = -bally;
			padx = 42;
			//hackEn = 0;
			if(posun > 8 && hra == 1)
				posun -= 8;
			else
				posun = 0;
			if(hra == 2) {
				bzzz(0);
				_delay_ms(70);
				bzzz(0);
				_delay_ms(70);
			}
			bzzz(hra==0);
			if(hra == 0) {
				level = maxL;
				zivoty = 3;
			}				
			if(hra != 1) {
				for(i = 0; i < BLOKOV; i++) {
					bloky[i] =   (pgm_read_byte(&LEVELS[level*BH*3+i/BW])&(1<<(BW-1-i%BW))) != 0? 1 :
							  (pgm_read_byte(&LEVELS[level*BH*3+BH+i/BW])&(1<<(BW-1-i%BW))) != 0? 2 :
							(pgm_read_byte(&LEVELS[level*BH*3+2*BH+i/BW])&(1<<(BW-1-i%BW))) != 0? 4 : 0;
				}
				_delay_ms(1500);
				if(level < LEVELOV)
					hra = 1;
				else {
					hra = 3;
					for(i = 3; i < 18; i++) {
						if(i%4)
							bzzz(0);
						else
							_delay_ms(50);
						_delay_ms(130);
					}
					pin(&PORTD, BL, 0);
				}
			}
		}
		_delay_ms(6);
		if(hra) {
			if(posun == 0)
				drawRect(0, 0, 84, 1, 2);
			else
				drawRect(0, posun-1, 84, posun+1, 2);
			for(i = 0; i < 42; i++) {
				drawPoint(0, i);
				drawPoint(83, i);
			}
			drawRect(padx-pads2, pady, padx+pads2, 48, 1);
			drawCircle(ballx, bally, r);
			
			for(i = 0; i < BLOKOV; i++) {
				if(bloky[i]) {
					drawRect((i%BW)*84/BW+1, i/BW*36/BH+1+posun, (i%BW+1)*84/BW, (i/BW+1)*36/BH+posun, bloky[i]);
					if((i/BW+1)*36/BH-1+posun > pady)
						hra = 0;
				}
			}
			if(leti) {
				ballx += vx;
				bally += vy;
			}
			else {
				ballx = padx;
				bally = 42;
			}
			if(leti)
				pgs++;
			//level 2, 5, 21 a 23 pojdu rychlejsie dole
			if(pgs > (level==1||level==4||level==20||level==22? 40 : 120)) {
				posun++;
				pgs = 0;
			}
			
			int prazdne = 1;
			int otacanie = 0;
			float cwh = 2.21f/BW*BH;
			for(i = 0; i < BLOKOV; i++) {
				int bx = (i%BW)*84/BW;
				int by = i/BW*36/BH+posun;
				float cx = bx+42/BW+0.5f;
				float cy = (by+19/BH)+0.5f;
				int dx = bx+84/BW;
				int dy = (by+36/BH);
				bx++;
				by++;
				if(bloky[i]) {
					if(ballx + r > bx && ballx - r < dx &&
					   bally + r > by && bally - r < dy) {
						if(bloky[i] == 2)
							bloky[i] = 0;
						else if(bloky[i] > 2)
							bloky[i]--;
						char vpravohore = (ballx - cx > (bally - cy)*cwh);
						char vpravodole = (ballx - cx > (cy - bally)*cwh);
						if(vpravohore&&vpravodole || !vpravohore&&!vpravodole) {
							if(!otacanie) {
								vx = -vx;
								ballv += 0.005f;
								pin(&PORTC, BUZZER, 0);
								otacanie = 1;
							}								
						}
						else if(!vpravohore&&vpravodole || vpravohore&&!vpravodole) {
							if(!otacanie) {
								vy = -vy;
								ballv += 0.005f;
								pin(&PORTC, BUZZER, 0);
								otacanie = 1;
							}								
						}
						if(vpravohore&vpravodole)			//vpravo
							ballx = 2*(dx+r)-ballx;
						else if(!vpravohore&&!vpravodole)	//vlavo
							ballx = 2*(bx-r)-ballx;
						else if(vpravohore&&!vpravodole)	//hore
							bally = 2*(by-r)-bally;
						else 								//dole
							bally = 2*(dy+r)-bally;
					}
					if(bloky[i] > 1)
						prazdne = 0;
				}
			}
			if(prazdne) {	//ak uz su vsetky kocky znicene, dalsi level
				level++;
				if(level > maxL)
					maxL = level;
				hra = 2;
			}
			if(bally > pady - r && bally < 48) {	//odraz od hracovej palicky
				if(ballx+r > padx-pads2 && ballx-r < padx+pads2) {
					ballv += 0.008f;
					pin(&PORTC, BUZZER, 0);
					vx = (ballx-padx)/pads2;
					vy = ballv*cos(vx);
					vx = ballv*sin(vx);
					bally = pady-r;
					vy = -vy;
				}
			}
			if(bally > 48 + 2*r) {
				hra = 0;
				repaint = 1;
			}
			if(ballx > 84 - r) {
				ballx = 2*(84-r)-ballx;
				vx = -vx;
				ballv += 0.005f;
				pin(&PORTC, BUZZER, 0);
			}
			if(ballx < r) {
				ballx = 2*r-ballx;
				vx = -vx;
				ballv += 0.005f;
				pin(&PORTC, BUZZER, 0);
			}
			if(bally < r+posun) {
				bally = 2*(r+posun)-bally;
				vy = -vy;
				ballv += 0.005f;
				pin(&PORTC, BUZZER, 0);
			}
			//tlacidla
			int val = PIND&(1<<1);
			if(val == 0) {
				if(leti == 0) {
					leti = 1;
					vy = -ballv;
				}				
				/*else if(hackEn == 0 || hackEn == 2) {
					hackEn++;
					if(hackEn == 3) {
						if(level<24)
							level = 24;
						else
							level++;
						hra = 2;
						hackEn = 0;
					}
				}					
			}
			else if(hackEn == 1) {
				hackEn++;*/
			}
			val = PIND&(1<<4);
			if(val == 0)
				padx += 0.6f+ballv;
			val = PIND&(1<<3);
			if(val == 0)
				padx -= 0.6f+ballv;
			if(padx > 84-pads2) {
				padx = 84-pads2;
				/*if(contr < 63) {
					contr++;
					lcdCmd(0x21); // LCD extended commands
					lcdCmd(0x80|contr); // set LCD Vop (contrast)
					lcdCmd(0x20); // LCD basic commands
				}*/
			}
			if(padx < pads2) {
				padx = pads2;
				/*if(contr > 0) {
					contr--;
					lcdCmd(0x21); // LCD extended commands
					lcdCmd(0x80|contr); // set LCD Vop (contrast)
					lcdCmd(0x20); // LCD basic commands
				}*/
			}
		}
		pin(&PORTC, BUZZER, 1);
	}
}
