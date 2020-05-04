#include "ui.h"
#include "img.h"
#include "colors.h"

ui::ui(PinName *lcd_data, PinName lcd_rst, PinName lcd_cs, PinName lcd_rs, PinName lcd_wr, PinName lcd_rd, unsigned short ScreenSize_X, unsigned short ScreenSize_Y)
    :s(lcd_data, lcd_rst, lcd_cs, lcd_rs, lcd_wr, lcd_rd, ScreenSize_X, ScreenSize_Y), battery_state{-1}, motor_state{false}, kswitch_state(true)
{
    s.setFontBgColor(0x738E);   //Grayish
    s.wrcolorbuf(0, 0, ScreenSize_X, ScreenSize_Y, ui_bg);

    draw_kswitch(false); 

    for(int i = 1; i <= MOTOR_COUNT; i+= 1) draw_motor(i,true);

    for(int i = 0; i < BATTERY_COUNT; i += 1){
        draw_battery(i, BATTERY_MAX_VOLTAGE_INT);
    }
}

void ui::draw_battery(unsigned int battery_nb, int voltage_int){
   
    if(voltage_int < BATTERY_MIN_VOLTAGE_INT || voltage_int > BATTERY_MAX_VOLTAGE_INT) return;

    const unsigned int battery_x_pos = 28;
    const unsigned int battery_y_pos[4] = {112, 166, 227, 278};

    const unsigned int voltage_x_pos = 140;
    const unsigned int voltage_y_pos[4] = {118, 172, 231, 284};

    const color_t *battery_img[6] = { bat_0_5, bat_1_5, bat_2_5, bat_3_5, bat_4_5, bat_5_5 };

    int b_state;

    // Get the state of the battery
    // First value is the highest threshold
    const int threshold[5] = { 15, 12, 9, 6, 3 };

    if     (voltage_int > threshold[0]) b_state = 5;
    else if(voltage_int > threshold[1]) b_state = 4;
    else if(voltage_int > threshold[2]) b_state = 3;
    else if(voltage_int > threshold[3]) b_state = 2;
    else if(voltage_int > threshold[4]) b_state = 1;
    else                                b_state = 0;

    // Only redraw when needed
    if(battery_state[battery_nb] != b_state){
        battery_state[battery_nb] = b_state;
        s.wrcolorbuf(battery_x_pos, battery_y_pos[battery_nb], battery_x_size, battery_y_size, battery_img[b_state], true);
    }

    float voltage = BATTERY_MIN_VOLTAGE + (float)voltage_int * BATTERY_RESOLUTION;
    char str[5];    // Attention au null terminator: réserver n+1 pour '\0' sinon le programme va planter
    sprintf(str,"%2.1f", voltage);
    s.string(voltage_x_pos, voltage_y_pos[battery_nb], str, BLACK);
}


void ui::draw_motor(unsigned int motor_nb, bool motor_status){

    const color_t *motor_img[2] = {m_off, m_on};
    
    const unsigned int motor_x_pos[2] = { 355, 215 };           // [0] -> moteurs paires, [1] -> moteurs impaires
    const unsigned int motor_y_pos[4] = { 91, 148, 210, 265 };  // 1 et 2 on le meme, 3 et 4 ont le meme...

    unsigned int xindex = motor_nb % 2;                         // 1,3,5,7 ont le meme index, 2,4,6,8 ont le meme index
    unsigned int yindex = (motor_nb - 1) - (motor_nb / 2);      // grouper: 1,2 ; 3,4 ; 5,6 ; 7,8; 

    if(motor_state[motor_nb - 1] != motor_status){
        motor_state[motor_nb - 1] = motor_status;
        s.wrcolorbuf(motor_x_pos[xindex], motor_y_pos[yindex], motor_x_size, motor_y_size, motor_img[motor_status], true);
    }
}

void ui::draw_kswitch(bool kswitch_status){

    const color_t *kswitch_img[2] = {kswitch_off, kswitch_on};
    
    const unsigned int kswitch_x_pos = 320;
    const unsigned int kswitch_y_pos = 30;

    if(kswitch_state != kswitch_status){
        kswitch_state = kswitch_status;
        s.wrcolorbuf(kswitch_x_pos, kswitch_y_pos, kswitch_x_size, kswitch_y_size, kswitch_img[kswitch_status], true);
    }

}

void ui::demo(void){

    wait_us(1000000);

    draw_kswitch(true);
    
    wait_us(1000000);
    
    draw_kswitch(false);

    wait_us(1000000);
    
    for(int i = 1; i <= MOTOR_COUNT; i+= 1) draw_motor(i,false);
    
    wait_us(1000000);
    
    for(int i = 1; i <= MOTOR_COUNT; i+= 1) draw_motor(i,true);

    wait_us(1000000);

    for(int i = BATTERY_MAX_VOLTAGE_INT; i >= BATTERY_MIN_VOLTAGE_INT; i -= 1){
        for(int j = 0; j < BATTERY_COUNT; j += 1){
            draw_battery(j, i);
            wait_us(10000);
        }
    }

    wait_us(1000000);

    for(int i = BATTERY_MIN_VOLTAGE_INT; i <= BATTERY_MAX_VOLTAGE_INT; i += 1){
        for(int j = 0; j < BATTERY_COUNT; j += 1){
            draw_battery(j, i);
            wait_us(10000);
        }
    }
}