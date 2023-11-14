#include <time.h>
#include <math.h>
#include "mpl3115a2.h"
#include "onboard_led.h"

int LED_1 = 0;
int LED_2 = 1;
int LED_3 = 2;
int pressure_leds_length = 10;
const int PRESSURE_LED_1 = 3;
const int PRESSURE_LED_2 = 6;
const int PRESSURE_LED_3 = 7;
const int PRESSURE_LED_4 = 8;
const int PRESSURE_LED_5 = 9;
const int PRESSURE_LED_6 = 10;
const int PRESSURE_LED_7 = 11;
const int PRESSURE_LED_8 = 12;
const int PRESSURE_LED_9 = 13;
const int PRESSURE_LED_10 = 14;

int pressure_leds[] = {
    PRESSURE_LED_1,
    PRESSURE_LED_2,
    PRESSURE_LED_3,
    PRESSURE_LED_4,
    PRESSURE_LED_5,
    PRESSURE_LED_6,
    PRESSURE_LED_7,
    PRESSURE_LED_8,
    PRESSURE_LED_9,
    PRESSURE_LED_10
    };

void pressure_leds_init(){
    for(int i = 0; i < pressure_leds_length; i++){
        gpio_init(pressure_leds[i]);
        gpio_set_dir(pressure_leds[i], GPIO_OUT);
    }
}

void init_leds(){
    on_board_led_init_configured_state(true);

    gpio_init(LED_1);
    gpio_set_dir(LED_1, GPIO_OUT);
    gpio_init(LED_2);
    gpio_set_dir(LED_2, GPIO_OUT);
    gpio_init(LED_3);
    gpio_set_dir(LED_3, GPIO_OUT);
    pressure_leds_init();
}

void reset_temp_leds(){
    gpio_put(LED_1, false);
    gpio_put(LED_2, false);
    gpio_put(LED_3, false);
}

void reset_pressure_lights(){
    for (int i = 0; i < pressure_leds_length; i++){
        gpio_put(pressure_leds[i], false);
    }
}

void process_farenheit(float temp_f){
    reset_temp_leds();
    double rounded = round(temp_f);
    if(rounded <= 69){
        gpio_put(LED_1, true);
        return;
    }
    if(rounded >= 70 && rounded < 80){
        gpio_put(LED_2, true);
        return;
    }
    if(rounded >= 80){
        gpio_put(LED_3, true);
    }
}

void process_pressure(float pressure){
    //pressure is in pascals.

    float lowest_recorded = 87000.00;
    float highest_recorded = 108380.00;

    reset_pressure_lights();

    int pressure_led = -1;
    for(float i = lowest_recorded; i < highest_recorded; i = i+2120){
        pressure_led++;
        if(pressure < i){
            gpio_put(pressure_leds[pressure_led], true);
        }
        
    }  
}

int main() {
    stdio_init_all();
    #if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
#warning i2c / mpl3115a2_i2c example requires a board with I2C pins
    puts("Default I2C pins were not defined");
    return 0;
#else
    init_leds();

    printf("initializing sensor");
    sensor_init(true);
    printf("Running!");
    int i = 0;
    while (true) {
        if (has_new_data) {
            read_data();

            time_t rawtime;
            struct tm * timeinfo;

            time ( &rawtime );
            timeinfo = localtime ( &rawtime );
            printf ( "Current local time and date: %s", asctime (timeinfo) );
            float farenheit = get_farenheit_temp()/* i*/;
            float celcius = get_celcius_temp();
            float altitude = get_altitude();
            float pressure = get_pressure();

            process_farenheit(farenheit);
            process_pressure(pressure);
            printf("%d sample average -> Farenheit: %.4f F, Celsius: %.4f, h: %.4f m, Pressure: %.4f kPa\n", MPL3115A2_FIFO_SIZE, farenheit, celcius, altitude, pressure);           
            
        }
    }
#endif
}
