#include "mbed.h"
#include "pins.h"
#include "ui.h"
#include "RS485_DEF_DEMO.h"

#define DEMO

#define BAUD_RATE 115200

RawSerial pc(USBTX, USBRX, BAUD_RATE); // tx, rx, baud

PinName LCD_DATA[8] = {LCD_D0, LCD_D1, LCD_D2, LCD_D3, LCD_D4, LCD_D5, LCD_D6, LCD_D7};
ui u(LCD_DATA, LCD_RST, LCD_CS, LCD_RS, LCD_WR, LCD_RD);

// main() runs in its own thread in the OS
int main()
{
#ifdef DEMO
    u.demo();
#else

    while(true){

        /* Replace these values with your own library */
        char id = pc.getc();
        pc.putc(id);

        char data = pc.getc();
        pc.putc(data);

        pc.putc('\n');
        pc.putc('\r');
        
        /* Modify the data variable accordingly to your own values 
           You should also set these values in the ui.h and ui.cpp */
        switch(id){
        // Power supply  
        case SLAVE_powersupply0:
            data -= PS_DATA_OFFSET;     //Delete this offset value in the real configuration
            u.draw_battery(0, data);
            break;
        case SLAVE_powersupply1:
            data -= PS_DATA_OFFSET;
            u.draw_battery(1, data);
            break;
        case SLAVE_powersupply2:
            data -= PS_DATA_OFFSET;
            u.draw_battery(2, data);
            break;
        case SLAVE_powersupply3:
            data -= PS_DATA_OFFSET;
            u.draw_battery(3, data);
            break;
        
        // Motors
        case SLAVE_ESC_1:
            data -= MOTOR_DATA_OFFSET;
            u.draw_motor(1, data);
            break;
        case SLAVE_ESC_2:
            data -= MOTOR_DATA_OFFSET;
            u.draw_motor(2, data);
            break;
        case SLAVE_ESC_3:
            data -= MOTOR_DATA_OFFSET;
            u.draw_motor(3, data);
            break;
        case SLAVE_ESC_4:
            data -= MOTOR_DATA_OFFSET;
            u.draw_motor(4, data);
            break;
        case SLAVE_ESC_5:
            data -= MOTOR_DATA_OFFSET;
            u.draw_motor(5, data);
            break;
        case SLAVE_ESC_6:
            data -= MOTOR_DATA_OFFSET;
            u.draw_motor(6, data);
            break;
        case SLAVE_ESC_7:
            data -= MOTOR_DATA_OFFSET;
            u.draw_motor(7, data);
            break;
        case SLAVE_ESC_8:
            data -= MOTOR_DATA_OFFSET;
            u.draw_motor(8, data);
            break;

        // KillSwitch
        case SLAVE_killMission:
            data -= KS_DATA_OFFSET;
            u.draw_kswitch(data);
            break;
        }
    }
#endif
}