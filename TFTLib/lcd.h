#ifndef __LCD_H__
#define __LCD_H__

#include "hw.h"
#include "font.h"
#include "colors.h"

/*  Class: lcd
    inherits functions of hw

    Desc: Instantiate a lcd object with high level draw function
          to draw on the 3.5 inch lcd screen with the ILI9486 lcd driver.

    Spec: Color format is 16 bit RGB565

    The screen is oriented with the origin in the top left corner: (x,y)
    
  (0,0)     (480,0)
    O-------- x
    |
    |
    y
  (0,320)


    Procedure to draw an image:
    1. Set the screen window with lcd::set_window, this will set the start column, the end column, the start page and the end page
       variable in the LCD controller.
    2. Send a memory write command to the lcd with your data. After sending the memory write,
       you can send all the pixels which will fill up the previously set window. The function wr_gram() does this job for you.

 */
class lcd : protected hw
{

public:
    lcd(PinName *lcd_data, PinName lcd_rst, PinName lcd_cs, PinName lcd_rs, PinName lcd_wr, PinName lcd_rd, unsigned short ScreenSize_X, unsigned short ScreenSize_Y);

    /* =============================================================== */
    /*  Function: lcd::pixel
        Desc: Draws a pixel at (x,y) of color color
     */
    void pixel(unsigned short x, unsigned short y, color_t color);

    /* =============================================================== */
     /* Function: lcd::hline
        Desc: Draws a horizontal line from (x1,y) to (x2,y) of color color
        Spec: Make sure x1 < x2
     */
    void hline(unsigned short x1, unsigned short x2,  unsigned short y, color_t color);

    /* Function: lcd::vline
        Desc: Draws a vertical line from (x,y1) to (x,y2) of color color
        Spec: Make sure y1 < y2
     */
    void vline(unsigned short x, unsigned short y1, unsigned short y2, color_t color);

    /* Function: lcd::segment
        Desc: Draws a segment from (x1,y1) to (x2,y2) of color color
     */
    void segment(unsigned short x1, unsigned short x2, unsigned short y1, unsigned short y2, color_t color);

    /* =============================================================== */
    /* Function: lcd::rect
        Desc: Draws a rectangle with (x1,y1), (x1,y2), (x2,y1), (x2,y2) as edge points
        Spec: Make sure x1 < x2 and y1 < y2
     */
    void rect(unsigned short x1, unsigned short x2, unsigned short y1, unsigned short y2, color_t color);

    /* Function: lcd::fillrect
        Desc: fills a rectangle with (x1,y1), (x1,y2), (x2,y1), (x2,y2) as edge points
        Spec: Make sure x1 < x2 and y1 < y2
     */
    void fillrect(unsigned short x1, unsigned short x2, unsigned short y1, unsigned short y2, color_t color);
    
    /* =============================================================== */
    /* Function: lcd::circle
        Desc: Draws a circle with (x0,y0) as center point and r as radius
     */
    void circle(unsigned short x0, unsigned short y0, unsigned short r, color_t color);

    /* Function: lcd::fillcircle
        Desc: fills a circle with (x0,y0) as center point and r as radius
     */
    void fillcircle(unsigned short x0, unsigned short y0, unsigned short r, color_t color);

    /* =============================================================== */

    /*  Function: setfont
        Desc: Set the character font.
        Create a dfont class and pass it as an argument
     */
    void setfont(font dfont) { this->dfont = dfont; }

    /* Function: character
       Desc: Writes the character 'c' at (x,y)
       Spec: The character will be coloured while the background will be of color fontBgColor
     */
    void character(unsigned short x, unsigned short y, char c, color_t color);

    /* Function string
       Desc: Prints an array of characters starting from (x,y)
       Spec: Make sure str is null terminated or program will be stuck
     */
    void string(unsigned short x, unsigned short y, const char *str, color_t color);

    /* Function stringcbuf
       Desc: Prints an array of characters starting from (x,y)
       Spec: Make sure str is null terminated or program will be stuck
             Make sure colorbuf is the same size as the string
     */
    void stringcbuf(unsigned short x, unsigned short y, const char *str, const color_t *colorbuf);

    /* =============================================================== */

    /*  Function: wrcolorbuf
        Desc: Writes an array of color starting at (x0,y0) of xsize and ysize.
              
              The parameter skip_black will replace black pixels in color buf with the 
              content of bgImg at this pixel location 
    
     */
    void wrcolorbuf(unsigned short x0, unsigned short y0, unsigned short xsize, unsigned short ysize, const color_t *colorbuf, bool skip_black = false);

    /* =============================================================== */
    
    unsigned short getXSize(void) { return xSize; };

    unsigned short getYSize(void) { return ySize; };

    /* Functin: Clear screen
       Prints a white screen
     */
    void cls(void) { fillrect(0, getXSize(), 0, getYSize(), WHITE); }

    /* Function setFontBgColor
       Desc: Set the font background. When the actual character is not printed it prints this instead.
     */
    void setFontBgColor(color_t fontBgColor) { this->fontBgColor = fontBgColor; }

    /* Function: setBgImg
       Desc: the background image is used with function wrcolorbuf with the skip_black flag on
             This bg img should be your UI
     */
    void setBgImg(const color_t *bgImg) { this->bgImg = bgImg; }

private:

    /*  Function: Set_window
        Desc: Set the window size, all pixels that will be drawn using memwrite will be written in the
              lcd memory at the location specified by the input parameters: (x1,y1) ------> (x1, y2)
                                                                            (x2,y1)         (x2, y2)
        Spec: these conditions MUST be met: X1 < X2 and y1 < y2
     */
    void set_window(unsigned short x1, unsigned short x2, unsigned short y1, unsigned short y2);
    
    unsigned short xSize;
    unsigned short ySize;

    color_t fontBgColor;
    const color_t *bgImg;

    font dfont;
};

#endif