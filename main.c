
#include "stdio.h"
#include <stdlib.h>
#include "string.h"
#include <avr/io.h>
#include "avr/interrupt.h"
#define F_CPU 1000000
#include "util/delay.h"
#include <time.h>

volatile int scroll_down[]  = {0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
0x80,0x90,0xa0,0xb0,0xc0,0xd0,0xe0,0xf0};
volatile int curX = 1, curY = 4, oldX = 1, oldY = 4;
volatile int type;
volatile int version = 0;
volatile int board[18][9];
/* for start */
unsigned char text_array[][8] = {
	{0x7e,0x7e,0x5a,0x18,0x18,0x18,0x3c,0x00}, // T
	{0x7e,0x46,0x16,0x1e,0x16,0x46,0x7e,0x00}, // E
	{0x7e,0x7e,0x5a,0x18,0x18,0x18,0x3c,0x00}, // T
	{0x7f,0x66,0x66,0x3e,0x36,0x66,0xc6,0x00}, // R
	{0x3c,0x18,0x18,0x18,0x18,0x18,0x3c,0x00}, // I
	{0x3e,0x63,0x46,0x1c,0x30,0x63,0x3e,0x00}, // S 
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}  // _
};

int isStarted = 0;
int isOngoing = 0;
unsigned int score = 0;
unsigned char tmp_LED[1][16];
void display(unsigned char LED[][16]){
	cli();

	for(int i = 0; i <= 16; i++){
		// change loop count to change speed, lower will be faster
		PORTA = scroll_down[i];
		PORTC = LED[0][i];
		_delay_ms(1);
	}
	sei();
}
void scrollingChar(unsigned int speed, int n){
	for(int h = 0; h < 8; h++){
		if(PINB == 0x01){
			return;
		}
		for(int z = 0; z < 15; z++){
			tmp_LED[0][z] = tmp_LED[0][z+1];
		}
		tmp_LED[0][15] = text_array[n][h];
		for(int w = 0; w < speed; w++)
		{
			display(tmp_LED);
		}
	}
}
void scrollingText(int speed){
	while(1){
		for(int i = 0; i <= 6; i++){
			if(PINB == 0x01){
				return;
			}
			scrollingChar(speed, i);
			sei();
		}
	}

}
/*-----*/
int getRow(int x) {
	int row = 0;
	for (int i = 1; i <= 8; i++)
	row = row | (board[x][i] << (8-i));
	return row;
}

void lightLED(int r, int c){
	board[r][c] = 1;
}

void make_heart_1F(){
	for(int i = 11; i <=16; i++){
		for(int j = 1; j <=8; j++){
			if(i == 11 && (j == 4 || j ==5 || j == 1 || j == 8)) j = j;
			else if(i == 14 && (j == 1 || j == 8)) j = j;
			else if(i == 15 && (j == 1 || j == 8 || j == 2 || j == 7)) j = j;
			else if(i == 16 && (j == 1 || j == 8 || j == 2 || j == 7 || j == 3 || j == 6)) j = j;
			else lightLED(i,j);
		}
	}
}
void make_heart_1(){
	for(int i = 11; i <=16; i++){
		for(int j = 1; j <=8; j++){
			if(i == 11 && (j == 2 || j ==3 || j == 6 || j == 7)) 
				lightLED(i,j);
			else if(i == 12 && (j == 4 || j ==5 || j == 1 || j == 8)) 
				lightLED(i,j);
			else if(i == 13 && (j == 1 || j == 8)) 
				lightLED(i,j);
			else if(i == 14 && (j == 2 || j == 7)) 
				lightLED(i,j);
			else if(i == 15 && (j == 3 || j == 6)) 
				lightLED(i,j);
			else if(i == 16 && (j == 4 || j == 5)) 
				lightLED(i,j);
			else j = j;
		}
	}
}
void make_heart_2(){
	for(int i = 11; i <=17; i++){
		for(int j = 1; j <=8; j++){
			if(i == 11 && (j == 2 || j ==3 || j == 6 || j == 7)) 
				lightLED(i,j);
			else if(i == 12 && (j == 4 || j ==5 || j == 1 || j == 8)) 
				lightLED(i,j);
			else if(i == 13 && (j == 1 || j == 8)) 
				lightLED(i,j);
			else if(i == 14 && (j == 1 || j == 8)) 
				lightLED(i,j);
			else if(i == 15 && (j == 2 || j == 7)) 
				lightLED(i,j);
			else if(i == 16 && (j == 3 || j == 6)) 
				lightLED(i,j);
			else if(i == 17 && (j == 4 || j == 5)) 
				lightLED(i,j);
			else j = j;
		}
	}
}
void make_heart_2F(){
	for(int i = 11; i <=17; i++){
		for(int j = 1; j <=8; j++){
			if(i == 11 && (j == 4 || j ==5 || j == 1 || j == 8)) j = j;
			else if(i == 15 && (j == 1 || j == 8)) j = j;
			else if(i == 16 && (j == 1 || j == 8 || j == 2 || j == 7)) j = j;
			else if(i == 17 && (j == 1 || j == 8 || j == 2 || j == 7 || j == 3 || j == 6)) j = j;
			else lightLED(i,j);
		}
	}
}
void make_0(int r, int c){
	lightLED(r-1, c-1);
	lightLED(r, c+1);
	lightLED(r, c-1);
	lightLED(r-1, c+1);
	lightLED(r-2, c+1);
	lightLED(r-2, c);
	lightLED(r-2, c-1);
	lightLED(r+1, c-1);
	lightLED(r+2, c-1);
	lightLED(r+2, c);
	lightLED(r+2, c+1);
	lightLED(r+1, c+1);
}
void make_1(int r, int c){
	lightLED(r, c);
	lightLED(r+1, c);
	lightLED(r+2, c);
	lightLED(r+2, c-1);
	lightLED(r+2, c+1);
	lightLED(r-1, c);
	lightLED(r-2, c);
	lightLED(r-1, c+1);
}
void make_2(int r, int c){
	lightLED(r, c);
	lightLED(r, c+1);
	lightLED(r, c-1);
	lightLED(r-1, c-1);
	lightLED(r-2, c+1);
	lightLED(r-2, c);
	lightLED(r-2, c-1);
	lightLED(r+1, c+1);
	lightLED(r+2, c-1);
	lightLED(r+2, c);
	lightLED(r+2, c+1);
}
void make_3(int r, int c){
	lightLED(r, c);
	lightLED(r, c+1);
	lightLED(r, c-1);
	lightLED(r-1, c+1);
	lightLED(r-2, c+1);
	lightLED(r-2, c);
	lightLED(r-2, c-1);
	lightLED(r+1, c+1);
	lightLED(r+2, c-1);
	lightLED(r+2, c);
	lightLED(r+2, c+1);
}
void make_4(int r, int c){
	lightLED(r, c);
	lightLED(r, c+1);
	lightLED(r, c-1);
	lightLED(r-1, c+1);
	lightLED(r-2, c+1);
	lightLED(r-1, c-1);
	lightLED(r-2, c-1);
	lightLED(r+1, c+1);
	lightLED(r+2, c+1);
}
void make_5(int r, int c){
	lightLED(r, c);
	lightLED(r, c+1);
	lightLED(r, c-1);
	lightLED(r-1, c+1);
	lightLED(r-2, c+1);
	lightLED(r-2, c);
	lightLED(r-2, c-1);
	lightLED(r+1, c-1);
	lightLED(r+2, c-1);
	lightLED(r+2, c);
	lightLED(r+2, c+1);
}
void make_6(int r, int c){
	lightLED(r, c);
	lightLED(r, c+1);
	lightLED(r, c-1);
	lightLED(r-1, c-1);
	lightLED(r-2, c+1);
	lightLED(r-2, c);
	lightLED(r-2, c-1);
	lightLED(r+1, c+1);
	lightLED(r+2, c-1);
	lightLED(r+2, c);
	lightLED(r+2, c+1);
	lightLED(r+1, c-1);
}
void make_7(int r, int c){
	lightLED(r, c-1);
	lightLED(r+1, c-1);
	lightLED(r+2, c-1);
	lightLED(r-1, c-1);
	lightLED(r-2, c+1);
	lightLED(r-2, c);
	lightLED(r-2, c-1);
}
void make_8(int r, int c){
	lightLED(r, c);
	lightLED(r-1, c-1);
	lightLED(r, c+1);
	lightLED(r, c-1);
	lightLED(r-1, c+1);
	lightLED(r-2, c+1);
	lightLED(r-2, c);
	lightLED(r-2, c-1);
	lightLED(r+1, c-1);
	lightLED(r+2, c-1);
	lightLED(r+2, c);
	lightLED(r+2, c+1);
	lightLED(r+1, c+1);
}
void make_9(int r, int c){
	lightLED(r, c);
	lightLED(r-1, c-1);
	lightLED(r, c+1);
	lightLED(r, c-1);
	lightLED(r-1, c+1);
	lightLED(r-2, c+1);
	lightLED(r-2, c);
	lightLED(r-2, c-1);
	lightLED(r+2, c-1);
	lightLED(r+2, c);
	lightLED(r+2, c+1);
	lightLED(r+1, c-1);
}

int makeT(int r, int c){
	if (version % 4 == 0){
		if (r > 17 || c < 2 || c > 7) return 0;
		if (board[r][c] == 0 && board[r][c-1] == 0 &&
		board[r][c+1] == 0 && board[r-1][c] == 0){
			lightLED(r,c);
			lightLED(r,c-1);
			lightLED(r,c+1);
			lightLED(r-1,c);
			return 1;
		}
	}
	else if (version % 4 == 1){
		if (r > 16 || c < 2 || c > 8) return 0;
		if (board[r][c] == 0 && board[r-1][c] == 0 &&
		board[r][c-1] == 0 && board[r+1][c] == 0){
			lightLED(r,c);
			lightLED(r-1,c);
			lightLED(r,c-1);
			lightLED(r+1,c);
			return 1;
		}
	}
	
	else if (version % 4 == 2){
		if (r > 16 || c < 2 || c > 7) return 0;
		if (board[r][c] == 0 && board[r][c-1] == 0 &&
		board[r][c+1] == 0 && board[r+1][c] == 0){
			lightLED(r,c);
			lightLED(r,c-1);
			lightLED(r,c+1);
			lightLED(r+1,c);
			return 1;
		}
	}
	else if (version % 4 == 3){
		if (r > 16 || c < 1 || c > 7) return 0;
		if (board[r][c] == 0 && board[r-1][c] == 0 &&
		board[r][c+1] == 0 && board[r+1][c] == 0){
			lightLED(r,c);
			lightLED(r-1,c);
			lightLED(r,c+1);
			lightLED(r+1,c);
			return 1;
		}
	}
	return 0;
}

int makeBAR(int r, int c){
	if (version % 2 == 0){
		if (r > 17 || c < 2 || c > 7) return 0;
		if (board[r][c] == 0 && board[r][c-1] == 0 &&
		board[r][c+1] == 0){
			lightLED(r,c);
			lightLED(r,c-1);
			lightLED(r,c+1);
			return 1;
		}
	}
	else if (version % 2 == 1){
		if (r > 16 || c < 1 || c > 8) return 0;
		if (board[r][c] == 0 && board[r-1][c] == 0 &&
		board[r+1][c] == 0){
			lightLED(r,c);
			lightLED(r-1,c);
			lightLED(r+1,c);
			return 1;
		}
	}
	return 0;
}

int makeSQUARE(int r, int c){
	if (r > 17 || c < 1 || c > 7) return 0;
	if (board[r][c] == 0 && board[r][c+1] == 0 &&
	board[r-1][c] == 0 && board[r-1][c+1] == 0){
		lightLED(r,c);
		lightLED(r,c+1);
		lightLED(r-1,c);
		lightLED(r-1,c+1);
		return 1;
	}
	return 0;
}

int makeL(int r, int c){
	if (version % 4 == 0) {
		if (r > 16 || c < 2 || c > 8) return 0;
		if (board[r][c] == 0 && board[r+1][c-1] == 0 &&
		board[r-1][c] == 0 && board[r+1][c] == 0){
			lightLED(r,c);
			lightLED(r+1,c);
			lightLED(r+1,c-1);
			lightLED(r-1,c);
			return 1;
		}
	}
	else if (version % 4 == 1) {
		if (r > 16 || c < 2 || c > 7) return 0;
		if (board[r][c] == 0 && board[r][c+1] == 0 &&
		board[r][c-1] == 0 && board[r+1][c+1] == 0){
			lightLED(r,c);
			lightLED(r,c+1);
			lightLED(r,c-1);
			lightLED(r+1,c+1);
			return 1;
		}
	}
	else if (version % 4 == 2) {
		if (r > 17 || c < 1 || c > 7) return 0;
		if (board[r][c] == 0 && board[r-1][c+1] == 0 &&
		board[r+1][c] == 0 && board[r-1][c] == 0){
			lightLED(r,c);
			lightLED(r-1,c+1);
			lightLED(r+1,c);
			lightLED(r-1,c);
			return 1;
		}
	}
	else if (version % 4 == 3) {
		if (r > 17 || c < 2 || c > 7) return 0;
		if (board[r][c] == 0 && board[r][c-1] == 0 &&
		board[r][c+1] == 0 && board[r-1][c-1] == 0){
			lightLED(r,c);
			lightLED(r,c-1);
			lightLED(r,c+1);
			lightLED(r-1,c-1);
			return 1;
		}
	}
	return 0;
}

int makeS(int r, int c){
	if (version % 2 == 0){
		if (r > 17 || c < 2 || c > 7) return 0;
		if (board[r][c] == 0 && board[r][c-1] == 0
		&& board[r-1][c+1] == 0 && board[r-1][c]==0){
			lightLED(r,c);
			lightLED(r,c-1);
			lightLED(r-1,c);
			lightLED(r-1,c+1);
			return 1;
		}
	}
	else if (version % 2 == 1){
		if (r > 16 || c < 1 || c > 7) return 0;
		if (board[r][c] == 0 && board[r][c+1] == 0
		&& board[r-1][c] == 0 && board[r+1][c+1]==0){
			lightLED(r,c);
			lightLED(r,c+1);
			lightLED(r+1,c+1);
			lightLED(r-1,c);
			return 1;
		}
	}
	return 0;
}

int makeZ(int r, int c){
	if (version % 2 == 0){
		if (r > 17 || c < 2 || c > 7) return 0;
		if (board[r][c] == 0 && board[r][c+1] == 0
		&& board[r-1][c] == 0 && board[r-1][c-1]==0){
			lightLED(r,c);
			lightLED(r,c+1);
			lightLED(r-1,c);
			lightLED(r-1,c-1);
			return 1;
		}
	}
	else if (version % 2 == 1){
		if (r > 16 || c < 1 || c > 7) return 0;
		if (board[r][c] == 0 && board[r][c+1] == 0
		&& board[r-1][c+1] == 0 && board[r+1][c]==0){
			lightLED(r,c);
			lightLED(r,c+1);
			lightLED(r-1,c+1);
			lightLED(r+1,c);
			return 1;
		}
	}
	return 0;
}

int check_rotate_collision(){
	int i = curX;
	int j = curY;
	switch(type){
		case 1: // makeT
			switch(version) {
				case 0:
					return (i == 17 || board[i+1][j]) ;
				case 1:
					return board[i][j+1] ;
				case 2:
					return 0 ;
				case 3:
					return board[i][j-1];
			}
			break;
		case 2: // makeBAR
			if(version == 0){
				return (i == 17 || board[i+1][j]) ;
			}
			else return (board[i][j+1] || board[i][j-1]);
			break;
		case 3: // makeSquare
			return 1;
			break;
		case 4: // makeL
			switch(version) {
				case 0:
				return (board[i][j-1] || board[i][j+1] || board[i+1][j+1]);
				case 1:
				return (board[i+1][j] || board[i-1][j] || board[i-1][j+1]);
				case 2:
				return (board[i][j+1] || board[i-1][j-1] || board[i][j-1]);
				case 3:
				return (i == 17 || board[i+1][j-1] || board[i+1][j] || board[i-1][j]) ;
			}
			break;
		case 6: // makeZ
			if(version == 0)
			return (i == 16 || board[i+1][j] || board[i-1][j+1]);
			else return (board[i-1][j] || board[i-1][j-1]);
			break;
		case 5: // makeS
			if(version == 0)
			return (i == 16 || board[i][j+1] || board[i+1][j+1]) ;
			else return (board[i][j-1] || board[i-1][j+1]) ;
			break;
	}
}
int checkbottom_collision(){
	int i = curX;
	int j = curY;
	switch(type){
		case 1: // makeT
			switch(version) {
				case 0:
				return (i == 17 || board[i+1][j+1] || board[i+1][j-1] || board[i+1][j]) ;
				case 1:
				return (i == 16 || board[i+1][j-1] || board[i+2][j]) ;
				case 2:
				return(i == 16 || board[i+1][j+1] || board[i+1][j-1] || board[i+2][j]) ;
				case 3:
				return (i == 16|| board[i+2][j] ||board[i+1][j+1]) ;
			}
			break;
		case 2: // makeBAR
			if(version == 0){
				return (i == 17 || board[i+1][j+1] || board[i+1][j-1] || board[i+1][j]) ;
			}
			else return (i == 16 || board[i+2][j]) ;
			break;
		case 3: // makeSquare
			return (i == 17 || board[i+1][j] || board[i+1][j+1]) ;
			break;
		case 4: // makeL
			switch(version) {
				case 0:
				return (i == 16 || board[i+2][j-1] || board[i+2][j]) ;
				case 1:
				return (i == 16 || board[i+2][j+1] || board[i+1][j-1] || board[i+1][j]) ;
				case 2:
				return (i == 16 || board[i+2][j] || board[i][j+1]) ;
				case 3:
				return (i == 17 || board[i+1][j+1] || board[i+1][j-1] || board[i+1][j]) ;
			}
			break;
		case 5: // makeS
			if(version == 0)
			return (i == 17 || board[i][j+1] || board[i+1][j-1] || board[i+1][j]) ;
			else return (i == 16 || board[i+1][j] || board[i+2][j+1]) ;
			break;

		case 6: // makeZ
			if(version == 0)
			return (i == 17 || board[i+1][j+1] || board[i][j-1] || board[i+1][j]) ;
			else return (i == 16 || board[i+1][j+1] || board[i+2][j]) ;
			break;
	return 0;
	}
}
int makeBLOCK(int r, int c, int type){
	if (type == 1) return makeT(r,c);
	else if (type == 2) return makeBAR(r,c);
	else if (type == 3) return makeSQUARE(r,c);
	else if (type == 4) return makeL(r,c);
	else if (type == 5) return makeS(r,c);
	else if (type == 6) return makeZ(r,c);
	return 0;
}

void removeBLOCKS(int r, int c, int type){
	
	if (type == 1) {
		if (version % 4 == 0){
			board[r][c]   = 0;
			board[r][c-1] = 0;
			board[r][c+1] = 0;
			board[r-1][c] = 0;
		}
		else if (version % 4 == 1){
			board[r][c]   = 0;
			board[r-1][c] = 0;
			board[r][c-1] = 0;
			board[r+1][c] = 0;
		}
		else if (version % 4 == 2){
			board[r][c]   = 0;
			board[r][c-1] = 0;
			board[r][c+1] = 0;
			board[r+1][c] = 0;
		}
		else if (version % 4 == 3){
			board[r][c]   = 0;
			board[r-1][c] = 0;
			board[r][c+1] = 0;
			board[r+1][c] = 0;
		}
	}
	else if (type == 2) {
		if (version % 2 == 0){
			board[r][c]   = 0;
			board[r][c-1] = 0;
			board[r][c+1] = 0;
		}
		else if (version % 2 == 1){
			board[r][c]   = 0;
			board[r-1][c] = 0;
			board[r+1][c] = 0;
		}
	}
	else if (type == 3) {
		board[r][c]     = 0;
		board[r][c+1]   = 0;
		board[r-1][c]   = 0;
		board[r-1][c+1] = 0;
	}
	else if (type == 4) {
		if (version % 4 == 0){
			board[r][c]     = 0;
			board[r+1][c-1] = 0;
			board[r-1][c]   = 0;
			board[r+1][c]   = 0;
		}
		else if (version % 4 == 1){
			board[r][c]   = 0;
			board[r][c+1] = 0;
			board[r][c-1] = 0;
			board[r+1][c+1] = 0;
		}
		else if (version % 4 == 2){
			board[r][c]   = 0;
			board[r-1][c+1] = 0;
			board[r+1][c] = 0;
			board[r-1][c] = 0;
		}
		else if (version % 4 == 3){
			board[r][c]   = 0;
			board[r][c-1] = 0;
			board[r][c+1] = 0;
			board[r-1][c-1] = 0;
		}
	}
	else if (type ==5){
		if (version % 4 == 0){
			board[r][c]= 0;
			board[r][c-1] = 0;
			board[r-1][c+1] =0;
			board[r-1][c] = 0;
		}
		else if (version % 4 == 1){
			board[r][c] = 0;
			board[r][c+1] = 0;
			board[r-1][c] = 0;
			board[r+1][c+1] = 0;
		}
	}
	else if (type ==6){
		if (version % 4 == 0){
			board[r][c]= 0;
			board[r][c+1] = 0;
			board[r-1][c-1] =0;
			board[r-1][c] = 0;
		}
		else if (version % 4 == 1){
			board[r][c] = 0;
			board[r][c+1] = 0;
			board[r-1][c+1] = 0;
			board[r+1][c] = 0;
		}
	}
	
}

void makeBOARD(int speed){
	cli();
	int i,j,k,p;
	int valArr[16];

	for (p = 0; p < 16; p++)
	valArr[p] = getRow(p+2);
	
	for(k = 0; k < speed; k++){
		for(i = 1; i <= 16; i++){
			int val = valArr[i-1];
			for (j = 1; j <= 10; j++){
				// change loop count to change speed, lower will be faster
				PORTA = scroll_down[i-1];
				PORTC = val;
			}
		}
	}
	sei();
}
void rotate(){
	switch(type){
		case 1:
		version = (version + 1)%4;
		break;
		case 2:
		version = (version + 1)%2;
		break;
		case 3:
		break;
		case 4:
		version = (version + 1)%4;
		break;
		case 5:
		version = (version + 1)%2;
		break;
		case 6:
		version = (version + 1)%2;
		break;
	}
	if(curY==8)
	curY--;
	if(curY==1)
	curY++;
}
ISR(INT2_vect)
{
	//rotate
	if(check_rotate_collision()) return;
	removeBLOCKS(curX,curY,type);
	rotate();

	makeBLOCK(curX,curY,type);
}
void move_player_left(){
	if(curX == 0) return;
	removeBLOCKS(curX, curY, type);
	oldY = curY;
	curY++;
	if (makeBLOCK(curX,curY,type) == 0)
	{
		curY = oldY;
		makeBLOCK(curX,curY,type);
	}
}
void move_player_right(){
	if(curX == 1) return;
	removeBLOCKS(curX, curY, type);
	oldY = curY;
	curY--;
	if (makeBLOCK(curX,curY,type) == 0)
	{
		curY = oldY;
		makeBLOCK(curX,curY,type);
	}
}

ISR(INT1_vect)
{
	//left shift
	move_player_right();
}


ISR(INT0_vect)
{
	//right shift
	move_player_left();
}

int isPressDown(){
	return PINB == 0x40;
}

void move_player_down(){
	if(checkbottom_collision()){
		return;
	}
	while(isPressDown()){
		if(checkbottom_collision() ){
			curX = 1;
			curY = 4;
			type = rand() % 6 +1;

			return;
		} 
		removeBLOCKS(curX, curY, type);
		curX++;
		makeBLOCK(curX, curY, type);
		makeBOARD(15);
	}

}

int clearMatchedRow(){
	int i,j,k,p;
	int matched = 0;

	for (i = 17; i >= 2; i--)
	{
		if (getRow(i) == 255){
			for (j = i-1; j >= 1; j--)
				for (k = 1; k <= 8; k++)
					board[j+1][k] = board[j][k];
			for (p = 1; p <= 8; p++)
				board[2][p] = 0;
			i++;
			matched=1;
			score++;
		}
	}
	return matched;
}
int check_GameOver()
{
	for(int i = 1; i <= 8; i++){
		if(board[1][i] && checkbottom_collision()) return 1;
	}
	return 0;
}
void get_num_to_display(int num, int col){
	switch(num){
		case 0:
			make_0(6, col);
			break;
		case 1:	
			make_1(6, col);
			break;
		case 2:	
			make_2(6, col);
			break;
		case 3:	
			make_3(6, col);
			break;
		case 4:	
			make_4(6, col);
			break;
		case 5:	
			make_5(6, col);
			break;
		case 6:	
			make_6(6, col);
			break;
		case 7:	
			make_7(6, col);
			break;
		case 8:	
			make_8(6, col);
			break;
		case 9:	
			make_9(6, col);
			break;
	}
}

void score_display(int num, int col){
	int x = num % 10;
	int y = num / 10;
	while(1){
		if(PINB == 0x01) return;
		make_heart_1F();
		get_num_to_display(y, col);
		get_num_to_display(x, col-4);
		makeBOARD(60);
		memset(board, 0, sizeof(board));
		
		if(PINB == 0x01) return;
		make_heart_1();
		get_num_to_display(y, col);
		get_num_to_display(x, col-4);
		makeBOARD(60);
		memset(board, 0, sizeof(board));

		if(PINB == 0x01) return;
		make_heart_2();
		get_num_to_display(y, col);
		get_num_to_display(x, col-4);
		makeBOARD(60);
		memset(board, 0, sizeof(board));

		if(PINB == 0x01) return;
		make_heart_2F();
		get_num_to_display(y, col);
		get_num_to_display(x, col-4);
		makeBOARD(60);
		memset(board, 0, sizeof(board));
	}
}
void Play()
{
	if(isStarted ==0) return;
	if(isPressDown()){
			move_player_down();
	}
	if(check_GameOver())
	{
		memset(board,0,sizeof(board));
		makeBOARD(60);
		isOngoing=1;
		curX = 1;
		curY = 4;
		score_display(score, 6);
	}
	
	if (curX == 1)
	{
		
		type = rand() % 6 +1;
		version=0;
		makeBLOCK(curX, curY, type);
	}

	if(checkbottom_collision()==0) {
		removeBLOCKS(curX, curY, type);
		curX++;
		makeBLOCK(curX, curY, type);
		makeBOARD(60);
		return;
	}
	else{
		if(clearMatchedRow()){
			makeBOARD(60);
			_delay_ms(15);
			curX = 1;
			curY = 4;
		}
		else{
			_delay_ms(15);
			curX = 1;
			curY = 4;
			makeBOARD(60);
			return;
		}
	}
}

void initInterrupt()
{
	GICR =  1<<INT1 | 1<<INT0 | 1<<INT2;
	MCUCR = 0x0F;
	MCUCSR = 0x40;
	sei();
}

int main(void)
{
	memset(board, 0, sizeof(board));
	memset(tmp_LED,0,sizeof(tmp_LED));
	DDRB = 0x00;
	DDRA = 0xFF;
	DDRD = 0b00010001;
	
	initInterrupt();
	srand(time(NULL));

	while(1){
		if(PINB == 0x80){
			isStarted =0;
			isOngoing=0;
			curX = 1;
			curY = 4;
			memset(board,0,sizeof(board));
			memset(tmp_LED,0,sizeof(tmp_LED));
			makeBOARD(60);
		}
		if(isStarted == 0 && isOngoing == 0){
			scrollingText(10);
		}
		if (isStarted==1 && isOngoing ==1){
			score = 0;
			Play();
		}
		else if(isStarted == 1 && isOngoing==0)
		{
			// game going normally, board filled
			uint8_t temp = PINB & 0x01;
			if(temp==0x01)
			{
				isStarted =1;
				isOngoing=1;
			}
		}
		else
		// initial
		if(PINB==0b00000001)
		{
			isStarted =1;
			isOngoing=1;
			_delay_ms(400);
		}
	}
}
