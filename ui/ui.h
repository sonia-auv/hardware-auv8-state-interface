#ifndef __UI_H__
#define __UI_H__

#include "lcd.h"

#define BATTERY_COUNT               4
#define BATTERY_MAX_VOLTAGE_INT     18
#define BATTERY_MIN_VOLTAGE_INT     0
#define BATTERY_MAX_VOLTAGE         16.8f
#define BATTERY_MIN_VOLTAGE         15.0f
#define BATTERY_RESOLUTION          0.1f    //Formula: (((BATTERY_MAX_VOLTAGE) - (BATTERY_MIN_VOLTAGE)) / ((BATTERY_MAX_VOLTAGE_INT) - (BATTERY_MIN_VOLTAGE_INT)))

#define MOTOR_COUNT                 8

class ui {

public:
    ui(PinName *lcd_data, PinName lcd_rst, PinName lcd_cs, PinName lcd_rs, PinName lcd_wr, PinName lcd_rd, unsigned short ScreenSize_X = 480, unsigned short ScreenSize_Y = 320);

    /*  Function: draw_battery
        Desc: Will print the voltage of the specified battery, if the voltage goes down under
              defined threshold the battery icon will change.
        battery_nb = [0:3]
        voltage_int = int value: [0:18], that represents [15V;16.8V] (0 -> 15V, 18 -> 16.8V)
     */
    void draw_battery(unsigned int battery_nb, int voltage_int);

    /* Function draw_motor
       Desc: will print the status of the specified motor on the screen
       motor_nb = [1:8]
       motor_status = on (1) or off(0)
     */
    void draw_motor(unsigned int motor_nb, bool motor_status);

    /* Function: draw_kswitch
       Desc: will print the status of the kill switch
       kswitch_status = on (1) or off(0)
     */
    void draw_kswitch(bool kswitch_status);

    /* This function will demo the ui */
    void demo(void);
    
private:

    lcd s;
    
    int battery_state[BATTERY_COUNT];
    const unsigned int battery_x_size = 75;
    const unsigned int battery_y_size = 28;

    bool motor_state[MOTOR_COUNT];
    const unsigned int motor_x_size = 52;
    const unsigned int motor_y_size = 28;
    
    bool kswitch_state;
    const unsigned int kswitch_x_size = 101;
    const unsigned int kswitch_y_size = 31;
};

#endif