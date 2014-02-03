#include "globals.h"
#include "weirdgraphics.h"
#include "math.h"
#include "graphics.h"

void plot_weird_heat(SDL_Surface *dest){
	float i,j;
	float colorSlide;
	float pixelScale = 2.0*3.14159265/400;
	//int r1=0xff, g1=0x3f, b1=0x3f, r2=0x3f, g2=0x3f, b2=0xff;
	int r1=0x00, g1=0xff, b1=0x00, r2=0x00, g2=0x00, b2=0x00;
	int rf, gf, bf;
	unsigned int colorFinal;
	
	for(j=0; j<SCREEN_HEIGHT; j+=1.0f){
		for(i=0; i<SCREEN_WIDTH; i+=1.0f){
			//colorSlide = j/SCREEN_HEIGHT;
			colorSlide = ( cosf(j*pixelScale)+sinf(i*pixelScale))/4 + 0.5;
			//colorSlide = rand()%1001 / 1000.0;
			//colorSlide = cosf((SCREEN_WIDTH/2-i)*(SCREEN_HEIGHT/2-j)*pixelScale*pixelScale)/2 + 0.5;
			/*
			colorFinal = 0;
			colorFinal |= (int)(( r1*colorSlide + r2*(1-colorSlide) )*0x10000);
			colorFinal |= (int)(( g1*colorSlide + g2*(1-colorSlide) )*0x100);
			colorFinal |= (int)(( b1*colorSlide + b2*(1-colorSlide) ));
			colorFinal |= (int)0xff000000;
			*/
			rf = (colorSlide*r1+(1-colorSlide)*r2);
			gf = (colorSlide*g1+(1-colorSlide)*g2);
			bf = (colorSlide*b1+(1-colorSlide)*b2);
			colorFinal = 0xff000000 + rf*0x10000 + gf*0x100 + bf;
			set_pixel(dest, i, j, colorFinal);
		}
	}
}
