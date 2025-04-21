/*
 * 20190312.013
 * OLED 128x32
 *
 * File: OLED.h
 * Processor: PIC16F1825
 * Author: wizlab.it
 */

#ifndef OLED_H
#define	OLED_H

#include "commons.h"
#include "i2c_master.h"

#ifndef SSD1306_I2C_ADDRESS
  #define SSD1306_I2C_ADDRESS   0x3C
#endif

#if !defined SSD1306_128_32 && !defined SSD1306_96_16
#define SSD1306_128_64
#endif
#if defined SSD1306_128_32 && defined SSD1306_96_16
  #error "Only one SSD1306 display can be specified at once"
#endif

#if defined SSD1306_128_64
  #define SSD1306_LCDWIDTH            128
  #define SSD1306_LCDHEIGHT            64
#endif
#if defined SSD1306_128_32
  #define SSD1306_LCDWIDTH            128
  #define SSD1306_LCDHEIGHT            32
#endif
#if defined SSD1306_96_16
  #define SSD1306_LCDWIDTH             96
  #define SSD1306_LCDHEIGHT            16
#endif

#define BLACK           0
#define WHITE           1
#define INVERSE         2

#define SSD1306_MEMORYMODE          0x20
#define SSD1306_MEMORYMODE_VERT     0x01
#define SSD1306_MEMORYMODE_HORZ     0x00
#define SSD1306_COLUMNADDR          0x21
#define SSD1306_PAGEADDR            0x22
#define SSD1306_SETCONTRAST         0x81
#define SSD1306_CHARGEPUMP          0x8D
#define SSD1306_SEGREMAP            0xA0
#define SSD1306_SEGREMAP_FLIP       0xA1
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON        0xA5
#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7
#define SSD1306_SETMULTIPLEX        0xA8
#define SSD1306_DISPLAYOFF          0xAE
#define SSD1306_DISPLAYON           0xAF
#define SSD1306_COMSCANINC          0xC0
#define SSD1306_COMSCANDEC          0xC8
#define SSD1306_SETDISPLAYOFFSET    0xD3
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5
#define SSD1306_SETPRECHARGE        0xD9
#define SSD1306_SETCOMPINS          0xDA
#define SSD1306_SETVCOMDETECT       0xDB

#define SSD1306_SETLOWCOLUMN        0x00
#define SSD1306_SETHIGHCOLUMN       0x10
#define SSD1306_SETSTARTLINE        0x40

#define SSD1306_EXTERNALVCC         0x01
#define SSD1306_SWITCHCAPVCC        0x02

#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A
#define SSD1306_DEACTIVATE_SCROLL                    0x2E
#define SSD1306_ACTIVATE_SCROLL                      0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3

#if !defined SELECTED_SSD1306_EXTERNALVCC || SELECTED_SSD1306_SWITCHCAP_VCC
    #define  SELECTED_SSD1306_EXTERNALVCC // default 
#endif 
#define FRAMEBUFFER 1024 // for 128x64
#define CHAR_SIZE 5 // Character : 5x8 (1-page)


void OLED_Command(u8 c);
void OLED_Commands(u8 *c, u8 n);
void OLED_SetPageAndColumnAddress(u8 startPage, u8 endPage, u8 startColumn, u8 endColumn);
void init_OLED(void);
void OLED_ClearDisplay(void);
void OLED_InvertDisplay(u8 i);
void OLED_StartScrollRight(u8 start, u8 stop);
void OLED_StartScrollLeft(u8 start, u8 stop);
void OLED_StartScrollDiagRight(u8 start, u8 stop);
void OLED_StartScrollDiagLeft(u8 start, u8 stop);
void OLED_StopScroll(void);
void OLED_Dim(bool dim);
void OLED_DATA_WRITE(
    u8 startPage, 
    u8 endPage, 
    u8 startColumn, 
    u8 endColumn);
void OLED_DrawBitmap(u8 startPage, u8 endPage, u8 startColumn, u8 endColumn, u8 *bitmap, uint16_t bitmapSize);
void _OLED_Draw_H_Line(u8 x1,u8 x2, u8 y,bool invert);
#define OLED_Draw_H_Line(x1, x2, y) _OLED_Draw_H_Line(x1, x2, y, false)
#define OLED_Erase_H_Line(x1, x2, y) _OLED_Draw_H_Line(x1, x2, y, true)
#define draw_progressbar(x1, x2, y, value) \
    OLED_Draw_H_Line(x1, value, y);           \
    OLED_Erase_H_Line(value, x2, y);       \
    
void OLED_Draw_V_Line(u8 x,u8 y1, u8 y2);

void OLED_DrawRectangle(
    u8 x, // 0..127
    u8 y, // 0..63
    u8 width, 
    u8 height);
void OLED_PutChar(u8 c, u8 x, u8 y, bool invert);
void OLED_PrintString(char* c, u8 x, u8 y, bool invert);
#define OLED_Printf(c, x, y) OLED_PrintString(c, x, y, false)
#define OLED_Printfi(c, x, y) OLED_PrintString(c, x, y, true)

#endif