#ifndef __HW_H__
#define __HW_H__

#include "mbed.h"

class hw {
    
protected:
    hw(PinName *lcd_data, PinName lcd_rst, PinName lcd_cs, PinName lcd_rs, PinName lcd_wr, PinName lcd_rd);
    
    void wr_cmd8(unsigned char cmd);    
    
    void wr_data8(unsigned char data);
    void wr_data16(unsigned short data);
    void wr_data32(unsigned int data);
    
    /* Write to graphic memory */
    void wr_gram(unsigned short color, unsigned int count);
    void wr_gram(unsigned short color);
    void wr_grambuf(const unsigned short *colorbuf, unsigned int count);
    
    unsigned char rd_data8(void);
    unsigned short rd_data16(void);
    unsigned int rd_data32(void);

    void rd_gram(unsigned short *colorbuf, unsigned int count);

private:
    
    BusInOut dbus;
    
    DigitalOut rst;
    DigitalOut cs;
    DigitalOut rs;
    DigitalOut wr;
    DigitalOut rd;
    
};
#endif