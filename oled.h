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


void OLED_Command(u8 c) {
    i2c_start();
    i2c_master_address(SSD1306_I2C_ADDRESS, I2C_MODE_WRITE); //Send address
    i2c_write((u8)0x00);          //Control byte, next is command
    i2c_write(c);                            //Command
    i2c_stop();
}

void OLED_Commands(u8 *c, u8 n) {
    i2c_start();
    i2c_master_address(SSD1306_I2C_ADDRESS, I2C_MODE_WRITE); //Send address
    i2c_write((u8)0x00);          //Control byte, next are commands
    while(n--) {                                    //Loop in commands...
        i2c_write(*c);
        c++;
    }
    i2c_stop();
}

inline void OLED_SetPageAndColumnAddress(u8 startPage, u8 endPage, u8 startColumn, u8 endColumn) {
  
    u8 commands[6];
    commands[0] = SSD1306_PAGEADDR;
    commands[1] = startPage;
    commands[2] = endPage;
    commands[3] = SSD1306_COLUMNADDR;
    commands[4] = startColumn;
    commands[5] = endColumn;
    OLED_Commands(commands, sizeof(commands));
}

void init_OLED(void) {
    u8 commands[] = {
        SSD1306_DISPLAYOFF,             //Switch off display (0xAE)
        SSD1306_SETDISPLAYCLOCKDIV,     //Set Display Clock Divide (0xD5)
            0x80,                       //Clock divide is 0x80
        SSD1306_SETMULTIPLEX,           //Set Multiplex Ratio (0xA8)
            0x3F,                       //Fixed to 0x1F (32 lines)
        SSD1306_SETDISPLAYOFFSET,       //Set Display Offset (0xD3)
            0x00,                       //No offset (0x00)
        SSD1306_SETSTARTLINE,           //Set Display Start Line (0x40) as line 0 (0x00)
        SSD1306_CHARGEPUMP,             //Set Charge Pump (0x8D)
            0x14,                       //Enabled (0x14, internal VCC)
        SSD1306_MEMORYMODE,             //Memory Addressing Mode (0x20)
        SSD1306_MEMORYMODE_HORZ,        //Vertical/Horizontal addressing mode (0x01/0x02)
        SSD1306_SEGREMAP_FLIP,          //Segment Re-map (0xA0) as COL0=SEG0 (0x00)
        SSD1306_COMSCANDEC,             //Scan from N -> 0        
#if defined SSD1306_128_32
        SSD1306_SETCOMPINS,                    // 0xDA
        0x02,
        SSD1306_SETCONTRAST,                   // 0x81
        0x8F,
#elif defined SSD1306_128_64
        SSD1306_SETCOMPINS,                    // 0xDA
        0x12,
        SSD1306_SETCONTRAST,                   // 0x81
        #if defined SELECTED_SSD1306_EXTERNALVCC
            0x9F,
        #else
            0xCF,
        #endif
#elif defined SSD1306_96_16
        SSD1306_SETCOMPINS,                    // 0xDA
        0x2,   //ada x12
        SSD1306_SETCONTRAST,                   // 0x81
        #if defined SELECTED_SSD1306_EXTERNALVCC
            0x10, 
        #else
            0xAF,
        #endif
#endif
        // Default tested setting ::
        // SSD1306_SETCOMPINS,             //COM Pins (0xDA)
        //     0x12,                       //Sequential, disabled remap
        // SSD1306_SETCONTRAST,            //Contrast Control (0x81)
        //     0x8F,                       //Contrast to 0x8F
        SSD1306_SETPRECHARGE,           //Pre-charge Period (0xD9)
#if defined SELECTED_SSD1306_EXTERNALVCC
        0xF1,
#else
        0x22,
#endif
//            0xF1,                       //Period to 0xF1
        SSD1306_SETVCOMDETECT,          //VCOMH Deselect Level (0xDB)
            0x20,                       //Level to 0x20~0x40
        SSD1306_DISPLAYALLON_RESUME,    //Display based on RAM (0xA4)
        SSD1306_NORMALDISPLAY,          //Normal display (0xA6)
        SSD1306_DEACTIVATE_SCROLL,      //Deactivate scroll (0x2E)
        SSD1306_DISPLAYON               //Turn on display (0xAF)
    };
    OLED_Commands(commands, sizeof(commands));
    OLED_ClearDisplay();
}

void OLED_ClearDisplay(void) {
    OLED_SetPageAndColumnAddress(0x00, 0x07, 0x00, 0x7F);

    i2c_start();
    i2c_master_address(SSD1306_I2C_ADDRESS, I2C_MODE_WRITE); //Send address
    i2c_write((u8)0x40);          //Control byte, next are data
    for(uint16_t byte=0; byte<FRAMEBUFFER; byte++) {       //Send a blank image (all zeroes)
        i2c_write(0X00);
    }
    i2c_stop();
}

void OLED_InvertDisplay(u8 i) {
  OLED_Command((i == 0x01) ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);
}


void OLED_StartScrollRight(u8 start, u8 stop)
{
  OLED_Command(SSD1306_RIGHT_HORIZONTAL_SCROLL);
  OLED_Command(0X00);
  OLED_Command(start);
  OLED_Command(0X00);
  OLED_Command(stop);
  OLED_Command(0X00);
  OLED_Command(0XFF);
  OLED_Command(SSD1306_ACTIVATE_SCROLL);
}

void OLED_StartScrollLeft(u8 start, u8 stop)
{
  OLED_Command(SSD1306_LEFT_HORIZONTAL_SCROLL);
  OLED_Command(0X00);
  OLED_Command(start);
  OLED_Command(0X00);
  OLED_Command(stop);
  OLED_Command(0X00);
  OLED_Command(0XFF);
  OLED_Command(SSD1306_ACTIVATE_SCROLL);
}

void OLED_StartScrollDiagRight(u8 start, u8 stop)
{
  OLED_Command(SSD1306_SET_VERTICAL_SCROLL_AREA);
  OLED_Command(0X00);
  OLED_Command(SSD1306_LCDHEIGHT);
  OLED_Command(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
  OLED_Command(0X00);
  OLED_Command(start);
  OLED_Command(0X00);
  OLED_Command(stop);
  OLED_Command(0X01);
  OLED_Command(SSD1306_ACTIVATE_SCROLL);
}

void OLED_StartScrollDiagLeft(u8 start, u8 stop)
{
  OLED_Command(SSD1306_SET_VERTICAL_SCROLL_AREA);
  OLED_Command(0X00);
  OLED_Command(SSD1306_LCDHEIGHT);
  OLED_Command(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
  OLED_Command(0X00);
  OLED_Command(start);
  OLED_Command(0X00);
  OLED_Command(stop);
  OLED_Command(0X01);
  OLED_Command(SSD1306_ACTIVATE_SCROLL);
}

void OLED_StopScroll(void)
{
  OLED_Command(SSD1306_DEACTIVATE_SCROLL);
}

void OLED_Dim(bool dim)
{
  u8 contrast;
  if (dim)
    contrast = 0; // Dimmed display
  else contrast = 
    #if defined SELECTED_SSD1306_EXTERNALVCC
      0x9F;
    #else
      0xCF;
    #endif
  // the range of contrast to too small to be really useful
  // it is useful to dim the display
  OLED_Command(SSD1306_SETCONTRAST);
  OLED_Command(contrast);
}

void OLED_DATA_WRITE(
    u8 startPage, 
    u8 endPage, 
    u8 startColumn, 
    u8 endColumn){
    OLED_SetPageAndColumnAddress(startPage, endPage, startColumn, endColumn);
    i2c_start();
    i2c_master_address(SSD1306_I2C_ADDRESS, I2C_MODE_WRITE);     //Send address
    i2c_write((u8)0x40);                    //Control byte, next are data
}

/*
 * The nature of frame-buffer display is overwritten continuously with values.
 */
void OLED_DrawBitmap(u8 startPage, u8 endPage, u8 startColumn, u8 endColumn, u8 *bitmap, uint16_t bitmapSize) {
    
    OLED_DATA_WRITE(startPage, endPage, startColumn, endColumn);
    
    for(uint16_t byte=0; byte<bitmapSize; byte++) {     //Loop into bitmap data...
        i2c_write(*bitmap);
        bitmap++;
    }
    i2c_stop();
}

void _OLED_Draw_H_Line(
    u8 x1,
    u8 x2, 
    u8 y,
    bool invert){
    // swap order :
    u8 x_start = min(x1, x2);
    u8 x_end = max(x1, x2);
    // y >> page + offset :
    u8 single_page = y / 8;
    u8 reminder =  y % 8;
    // set page + column :
    OLED_DATA_WRITE(single_page, single_page, x_start, x_end);
    // Draw->I2C :
    for(u8 _x = x_start; _x < x_end; _x++){
        u8 page = (u8)(1 << (reminder));
        i2c_write((u8)((invert ? 0 : 1) << page));
    }
    i2c_stop();
}

/// Reserve 8 bytes for a single column.
u8 page_buffer[8] = {};    

void OLED_Draw_V_Line(
    u8 x,
    u8 y1, 
    u8 y2){
    // swap order :
    u8 y_start = min(y1, y2);
    u8 y_end = max(y1, y2);
    // page start..end :
    u8 page_end = y_end/8;
    u8 page_start = y_start/8;
    OLED_DATA_WRITE(page_start, page_end, x, x);
    // in case both stay in the same page:
    if(page_start == page_end){
        // Same page !
        
    } else {
        // fill through pages :
        // | 0 | .. | 7 | 
        u8 body = 0b11111111;
        // Head >> Body >> Tail
        // 1111-1111 - (1 << 0)=0000-0001 + 1 
        // = 1111-1110 + 1
        // = 1111-1111
        u8 head = body - (u8)(1 << y_start) + 1; 
        i2c_write(head);
        // Body << only fill with 3+ pages
        // 1111-1111
        if(page_end - page_start > 1)
        for(u8 i = page_start+1; i < page_end; i++){
            i2c_write(body);
        }
        // Tail
        // (1 << 7+1)=0000-0000 - 1
        // 1111-1111
        u8 tail = (u8)(1 << (y_end + 1))-1; 
        i2c_write(tail);
    }
    
    i2c_stop();
}

/**
 * Draw a Rectangle directly to display :
 * - x ---> x + width 
 * - y ---> (y + height) % 8
 * @param 8-bit x 
 * @param 8-bit y
 * @param width
 * @param height
 */
void OLED_DrawRectangle(
    u8 x, // 0..127
    u8 y, // 0..63
    u8 width, 
    u8 height){
    u8 y1 = y > 7 ? y % 8 : 0;
    u8 y2 = y + height > 7 ? (y + height) % 8 : 0; 
    
    OLED_DATA_WRITE(y1, y2, x, x + width);
    
    u8 page0 = 0b00001111; // pretend that we only fill 128x1
    i2c_write(page0);
    
    for(u8 _x = 0; _x < x + width-2; _x++){
        u8 page = 0b00001001; // pretend that we only fill 128x1
        i2c_write(page);
    }
    
//    i8 page = 0b11111111; // pretend that we only fill 128x1
    i2c_write(page0);
    
    i2c_stop();
}


const char Font[255] = {
0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x5F, 0x00, 0x00,
0x00, 0x07, 0x00, 0x07, 0x00,
0x14, 0x7F, 0x14, 0x7F, 0x14,
0x24, 0x2A, 0x7F, 0x2A, 0x12,
0x23, 0x13, 0x08, 0x64, 0x62,
0x36, 0x49, 0x56, 0x20, 0x50,
0x00, 0x08, 0x07, 0x03, 0x00,
0x00, 0x1C, 0x22, 0x41, 0x00,
0x00, 0x41, 0x22, 0x1C, 0x00,
0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
0x08, 0x08, 0x3E, 0x08, 0x08,
0x00, 0x80, 0x70, 0x30, 0x00,
0x08, 0x08, 0x08, 0x08, 0x08,
0x00, 0x00, 0x60, 0x60, 0x00,
0x20, 0x10, 0x08, 0x04, 0x02,
0x3E, 0x51, 0x49, 0x45, 0x3E,
0x00, 0x42, 0x7F, 0x40, 0x00,
0x72, 0x49, 0x49, 0x49, 0x46,
0x21, 0x41, 0x49, 0x4D, 0x33,
0x18, 0x14, 0x12, 0x7F, 0x10,
0x27, 0x45, 0x45, 0x45, 0x39,
0x3C, 0x4A, 0x49, 0x49, 0x31,
0x41, 0x21, 0x11, 0x09, 0x07,
0x36, 0x49, 0x49, 0x49, 0x36,
0x46, 0x49, 0x49, 0x29, 0x1E,
0x00, 0x00, 0x14, 0x00, 0x00,
0x00, 0x40, 0x34, 0x00, 0x00,
0x00, 0x08, 0x14, 0x22, 0x41,
0x14, 0x14, 0x14, 0x14, 0x14,
0x00, 0x41, 0x22, 0x14, 0x08,
0x02, 0x01, 0x59, 0x09, 0x06,
0x3E, 0x41, 0x5D, 0x59, 0x4E,
0x7C, 0x12, 0x11, 0x12, 0x7C,
0x7F, 0x49, 0x49, 0x49, 0x36,
0x3E, 0x41, 0x41, 0x41, 0x22,
0x7F, 0x41, 0x41, 0x41, 0x3E,
0x7F, 0x49, 0x49, 0x49, 0x41,
0x7F, 0x09, 0x09, 0x09, 0x01,
0x3E, 0x41, 0x41, 0x51, 0x73,
0x7F, 0x08, 0x08, 0x08, 0x7F,
0x00, 0x41, 0x7F, 0x41, 0x00,
0x20, 0x40, 0x41, 0x3F, 0x01,
0x7F, 0x08, 0x14, 0x22, 0x41,
0x7F, 0x40, 0x40, 0x40, 0x40,
0x7F, 0x02, 0x1C, 0x02, 0x7F,
0x7F, 0x04, 0x08, 0x10, 0x7F,
0x3E, 0x41, 0x41, 0x41, 0x3E,
0x7F, 0x09, 0x09, 0x09, 0x06,
0x3E, 0x41, 0x51, 0x21, 0x5E,
0x7F, 0x09, 0x19, 0x29, 0x46
};
const char Font2[220] = {
0x26, 0x49, 0x49, 0x49, 0x32,
0x03, 0x01, 0x7F, 0x01, 0x03,
0x3F, 0x40, 0x40, 0x40, 0x3F,
0x1F, 0x20, 0x40, 0x20, 0x1F,
0x3F, 0x40, 0x38, 0x40, 0x3F,
0x63, 0x14, 0x08, 0x14, 0x63,
0x03, 0x04, 0x78, 0x04, 0x03,
0x61, 0x59, 0x49, 0x4D, 0x43,
0x00, 0x7F, 0x41, 0x41, 0x41,
0x02, 0x04, 0x08, 0x10, 0x20,
0x00, 0x41, 0x41, 0x41, 0x7F,
0x04, 0x02, 0x01, 0x02, 0x04,
0x40, 0x40, 0x40, 0x40, 0x40,
0x00, 0x03, 0x07, 0x08, 0x00,
0x20, 0x54, 0x54, 0x78, 0x40,
0x7F, 0x28, 0x44, 0x44, 0x38,
0x38, 0x44, 0x44, 0x44, 0x28,
0x38, 0x44, 0x44, 0x28, 0x7F,
0x38, 0x54, 0x54, 0x54, 0x18,
0x00, 0x08, 0x7E, 0x09, 0x02,
0x18, 0xA4, 0xA4, 0x9C, 0x78,
0x7F, 0x08, 0x04, 0x04, 0x78,
0x00, 0x44, 0x7D, 0x40, 0x00,
0x20, 0x40, 0x40, 0x3D, 0x00,
0x7F, 0x10, 0x28, 0x44, 0x00,
0x00, 0x41, 0x7F, 0x40, 0x00,
0x7C, 0x04, 0x78, 0x04, 0x78,
0x7C, 0x08, 0x04, 0x04, 0x78,
0x38, 0x44, 0x44, 0x44, 0x38,
0xFC, 0x18, 0x24, 0x24, 0x18,
0x18, 0x24, 0x24, 0x18, 0xFC,
0x7C, 0x08, 0x04, 0x04, 0x08,
0x48, 0x54, 0x54, 0x54, 0x24,
0x04, 0x04, 0x3F, 0x44, 0x24,
0x3C, 0x40, 0x40, 0x20, 0x7C,
0x1C, 0x20, 0x40, 0x20, 0x1C,
0x3C, 0x40, 0x30, 0x40, 0x3C,
0x44, 0x28, 0x10, 0x28, 0x44,
0x4C, 0x90, 0x90, 0x90, 0x7C,
0x44, 0x64, 0x54, 0x4C, 0x44,
0x00, 0x08, 0x36, 0x41, 0x00,
0x00, 0x00, 0x77, 0x00, 0x00,
0x00, 0x41, 0x36, 0x08, 0x00,
0x02, 0x01, 0x02, 0x04, 0x02
};

void OLED_PutChar(u8 c, u8 x, u8 y, bool invert) {
  u8 font_c;
  
  
  if((c < ' ') || (c > '~'))
    c = '?';
  u8 start_page = y % 8;

  OLED_DATA_WRITE(start_page, start_page, x, x + CHAR_SIZE);
  
  for(u8 i = 0; i < 5; i++ ) 
  {
    u8 offs;
    if(c < 'S') {
        offs = (c - 32)*5;
        font_c = Font[offs + i];
    } else {
        offs = (c - 'S')*5;
        font_c = Font2[offs + i];
    }    
    i2c_write(invert ? ~font_c : font_c);
  }
  i2c_stop();
}
void OLED_PrintString(char* c, u8 x, u8 y, bool invert){
    OLED_Command(SSD1306_SEGREMAP_FLIP);
    u8 counter = 0;
    while(*c!=0x00){
        OLED_PutChar(*c, x + counter * CHAR_SIZE, y, invert);
        counter++;
        c++;
   }
}

#endif