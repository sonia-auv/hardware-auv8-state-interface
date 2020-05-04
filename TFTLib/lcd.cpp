#include "lcd.h"
#include "ui_bg.h"

lcd::lcd(PinName *lcd_data, PinName lcd_rst, PinName lcd_cs, PinName lcd_rs, PinName lcd_wr, PinName lcd_rd, unsigned short ScreenSize_X, unsigned short ScreenSize_Y)
    : hw(lcd_data, lcd_rst, lcd_cs, lcd_rs, lcd_wr, lcd_rd), xSize(ScreenSize_X), ySize(ScreenSize_Y), fontBgColor(WHITE), bgImg(ui_bg), dfont(Terminal12x16, 12, 16)
{
    // Init Sequence
    /*  Les commandes pour le gamma on été prises de la librairie UNIGRAPHICS,
        elles résoud un problème d'écran qui "flicker" sais raison apparentes.
        A documenter peut etre...
     */
    wr_cmd8(0xE0);        // Positive gamma control
    wr_data8(0x0F);
    wr_data8(0x18);
    wr_data8(0x15);
    wr_data8(0x09);
    wr_data8(0x0B);
    wr_data8(0x04);
    wr_data8(0x49);
    wr_data8(0x64);
    wr_data8(0x3D);
    wr_data8(0x08);
    wr_data8(0x15);
    wr_data8(0x06);
    wr_data8(0x12);
    wr_data8(0x07);
    wr_data8(0x00);

    wr_cmd8(0xE1);        // Negative gamma control
    wr_data8(0x0F);
    wr_data8(0x38);
    wr_data8(0x35);
    wr_data8(0x0a);
    wr_data8(0x0c);
    wr_data8(0x03);
    wr_data8(0x4A);
    wr_data8(0x42);
    wr_data8(0x36);
    wr_data8(0x04);
    wr_data8(0x0F);
    wr_data8(0x03);
    wr_data8(0x1F);
    wr_data8(0x1B);
    wr_data8(0x00);

    wr_cmd8(0x36);        // Memory Access Control
    wr_data8(0xE8);       // BGR screen
    
    wr_cmd8(0x3A);        // Interface Pixel Format
    wr_data8(0x55);       // 16 bit/pixel for rgb and cpu interface
    
    wr_cmd8(0xB1);        // Frame Rate Control
    wr_data8(0xB0);       // 70 Hz
    wr_data8(0x10);       // 16 clocks/line
    
    wr_cmd8(0x20);        // Display inversion OFF

    wr_cmd8(0x11);        // Sleep Out
    wait_us(160000);

    wr_cmd8(0x53);        // Write ctrl display value
    wr_data8(0x24);       // BCTRL=1, BL=1
    
    wr_cmd8(0x51);        // Write display brightness value
    wr_data8(0xFF);       // Highest brightness

    wr_cmd8(0x29);        // Display on
}

void lcd::set_window(unsigned short x1, unsigned short x2, unsigned short y1, unsigned short y2){
    wr_cmd8(0x2A);  
    wr_data16(x1);
    wr_data16(x2);

    wr_cmd8(0x2B);
    wr_data16(y1);
    wr_data16(y2);
}


void lcd::pixel(unsigned short x, unsigned short y, color_t color){

    set_window(x,x,y,y);

    wr_gram(color);
}

void lcd::hline(unsigned short x1, unsigned short x2, unsigned short y, color_t color){

    set_window((x1 < x2 ? x1 : x2), (x1 < x2 ? x2 : x1), y, y);

    short count = abs((short)(x2 - x1)) + 1;
    wr_gram(color, (unsigned short)count);
}

void lcd::vline(unsigned short x, unsigned short y1, unsigned short y2, color_t color){

    set_window(x,x,(y1 < y2 ? y1 : y2),(y1 < y2 ? y2 : y1));

    short count = abs((short)(y2 - y1)) + 1;
    wr_gram(color, (unsigned short)count);
}

void lcd::segment(unsigned short x1, unsigned short x2, unsigned short y1, unsigned short y2, color_t color){

    /* Algorithme: Brensnham line algorithme
       https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
     */

    short sx = x1 < x2 ? 1 : -1;
    short dx = (x2 - x1) * sx;
    
    short sy = y1 < y2 ? -1 : 1;
    short dy = (y2 - y1) * sy;
    
    short e = dx + dy;
    short e2;
    
    while((x1 != x2) && (y1 != y2)){
        pixel(x1, y1, color);

        e2 = e + e;
        if(e2 >= dy){
            e += dy;
            x1 += sx;
        }

        if(e2 <= dx){
            e += dx;
            y1 -= sy;
        }
    }

    pixel(x2,y2,color);
}

void lcd::rect(unsigned short x1, unsigned short x2, unsigned short y1, unsigned short y2, color_t color){

    hline(x1, x2, y1, color);
    hline(x1, x2, y2, color);
    
    vline(x1, y1, y2, color);
    vline(x2, y1, y2, color);
}
    
void lcd::fillrect(unsigned short x1, unsigned short x2, unsigned short y1, unsigned short y2, color_t color){
    set_window((x1 < x2 ? x1 : x2),(x1 < x2 ? x2 : x1),(y1 < y2 ? y1 : y2),(y1 < y2 ? y2 : y1));
  
    short count = (abs((short)(x2 - x1)) + 1) * (abs((short)(y2 - y1)) + 1);
    wr_gram(color, (unsigned short)count);
}
    
void lcd::circle(unsigned short x0, unsigned short y0, unsigned short r, color_t color){
    
    /*
    Algorithme: Cercle d'andres
    https://fr.wikipedia.org/wiki/Algorithme_de_trac%C3%A9_de_cercle_d%27Andres
        Plus lent mais résoud le problème de bresenham
    */
    /*
    short x = 0;
    short y = r;
    short d = r - 1;

    short x_2;
    short y_2;

    while(x <= y){
        pixel(x0 + x, y0 + y, color);
        pixel(x0 + y, y0 + x, color);
        
        pixel(x0 - x, y0 + y, color);
        pixel(x0 - y, y0 + x, color);
        
        pixel(x0 + x, y0 - y, color);
        pixel(x0 + y, y0 - x, color);
        
        pixel(x0 - x, y0 - y, color);
        pixel(x0 - y, y0 - x, color);
        
        x_2 = (x + x);
        y_2 = (y + y);

        if(d >= x_2){
            d = d - x_2 - 1;
            x = x + 1;
        }
        else if(d < ((r + r) - y_2)){
            d = d + y_2 - 1;
            y = y - 1;
        }   
        else{
            d = d + y_2 - x_2 - 2;
            y = y - 1;
            x = x + 1;
        }   
    }
    */

    /* 
    Algorithme: cercle de bresenham
    https://fr.wikipedia.org/wiki/Algorithme_de_trac%C3%A9_d%27arc_de_cercle_de_Bresenham
    Rapide, mais si on dessine plusieurs cercles de rayon 1,2,..., n il va y avoir des pixel vides
    entre les cercles
    */
    short x = 0;
    short y = r;
    short m = 5 - 4*y;

    while(x <= y){

        pixel(x0 + x, y0 + y, color);
        pixel(x0 + y, y0 + x, color);
        
        pixel(x0 - x, y0 + y, color);
        pixel(x0 - y, y0 + x, color);
        
        pixel(x0 + x, y0 - y, color);
        pixel(x0 + y, y0 - x, color);
        
        pixel(x0 - x, y0 - y, color);
        pixel(x0 - y, y0 - x, color);

        if(m > 0){
            y = y - 1;
            m = m - 8*y;
        }
        
        x = x + 1;
        m = m + 8*x + 4;
    }
}

void lcd::fillcircle(unsigned short x0, unsigned short y0, unsigned short r, color_t color){

    short x = 0;
    short y = r;
    short m = 5 - 4*y;

    while(x <= y){

        vline((x0 + x), (y0 - y), (y0 + y), color);
        vline((x0 - x), (y0 - y), (y0 + y), color);
       // vline((x0 + y), (y0 - x), (y0 + x), color);
      //  vline((x0 - y), (y0 - x), (y0 + x), color);

       // hline((x0 - x), (x0 + x), (y0 + y), color);
       // hline((x0 - x), (x0 + x), (y0 - y), color);
        hline((x0 - y), (x0 + y), (y0 + x), color);
        hline((x0 - y), (x0 + y), (y0 - x), color);        

        if(m > 0){
            y = y - 1;
            m = m - 8*y;
        }

        x = x + 1;
        m = m + 8*x + 4;
    }
}

void lcd::character(unsigned short x, unsigned short y, char c, color_t color){

    if(c < 32 || c > 127)
        return;
  
    set_window(x,x + dfont.x, y, y + dfont.y);
    
    unsigned int index = (c - 32) * dfont.x * dfont.ycount;

    for(int i = 0; i < dfont.x; i += 1){
        
        for(int j = 0; j < dfont.ycount; j += 1){
            
            int w = 0;
            int yoffset = y + j * 8;
            
            switch(dfont.buf[index + j]){
            case 0x00:
                vline(x + i, yoffset,  yoffset + 8, fontBgColor);
                break;

            case 0xFF:
                vline(x + i, yoffset,  yoffset + 8, color);
                break;
            
            default:
                for(int k = 0x01; k <= 0x80; k = k << 1){
                    char bit = (dfont.buf[index + j] & k);
                    pixel((x + i), yoffset + w, (bit ? color : fontBgColor));
                    w += 1;
                }
                break;
            }      
        }
        index += dfont.ycount;
    }
}

void lcd::string(unsigned short x, unsigned short y, const char *str, color_t color){

    short xOffset = 0;
    while(*str){

        character(x + xOffset, y, *str, color);
        
        xOffset += dfont.x;
        ++str;
    }
}

void lcd::stringcbuf(unsigned short x, unsigned short y, const char *str, const color_t *colorbuf){

    short xOffset = 0;
    while(str){

        character(x + xOffset, y, *str, *colorbuf);
        
        xOffset += dfont.x;
        ++colorbuf;
        ++str;
    }
}

void lcd::wrcolorbuf(unsigned short x0, unsigned short y0, unsigned short xsize, unsigned short ysize, const color_t *colorbuf, bool skip_black){
    
    unsigned short x_off = x0 + xsize - 1;
    unsigned short y_off = y0 + ysize - 1;
    unsigned int count = xsize * ysize;

    set_window(x0, x_off, y0, y_off);

    if(!skip_black){
        wr_grambuf((const unsigned short*)colorbuf, count);   
    }
    else{
        
        wr_cmd8(0x2C);
        
        for(unsigned int y = y0; y < y0 + ysize; y += 1){
            for(unsigned int x = x0; x < x0 + xsize; x += 1){
                
                if(*colorbuf == 0){
                    unsigned int index = y * this->xSize + x;
                    wr_data16((unsigned short)(bgImg[index]));
                }
                else{
                    wr_data16((unsigned short)((*colorbuf)));
                }

                colorbuf += 1;
            }
        }
    }
}