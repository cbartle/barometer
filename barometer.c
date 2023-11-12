#include <time.h>
#include <math.h>
#include "mpl3115a2.h"
#include "onboard_led.h"

int LED_1 = 0;
int LED_2 = 1;
int LED_3 = 2;
int PRESSURE_LED_1 = 3;
int PRESSURE_LED_2 = 6;
int PRESSURE_LED_3 = 7;
int PRESSURE_LED_4 = 8;
int PRESSURE_LED_5 = 9;
int PRESSURE_LED_6 = 10;
int PRESSURE_LED_7 = 11;
int PRESSURE_LED_8 = 12;
int PRESSURE_LED_9 = 13;
int PRESSURE_LED_10 = 14;

void pressure_leds_init(){
    gpio_init(PRESSURE_LED_1);
    gpio_set_dir(PRESSURE_LED_1, GPIO_OUT);
    gpio_init(PRESSURE_LED_2);
    gpio_set_dir(PRESSURE_LED_2, GPIO_OUT);
    gpio_init(PRESSURE_LED_3);
    gpio_set_dir(PRESSURE_LED_3, GPIO_OUT);
    gpio_init(PRESSURE_LED_4);
    gpio_set_dir(PRESSURE_LED_4, GPIO_OUT);
    gpio_init(PRESSURE_LED_5);
    gpio_set_dir(PRESSURE_LED_5, GPIO_OUT);
    gpio_init(PRESSURE_LED_6);
    gpio_set_dir(PRESSURE_LED_6, GPIO_OUT);
    gpio_init(PRESSURE_LED_7);
    gpio_set_dir(PRESSURE_LED_7, GPIO_OUT);
    gpio_init(PRESSURE_LED_8);
    gpio_set_dir(PRESSURE_LED_8, GPIO_OUT);
    gpio_init(PRESSURE_LED_9);
    gpio_set_dir(PRESSURE_LED_9, GPIO_OUT);
    gpio_init(PRESSURE_LED_10);
    gpio_set_dir(PRESSURE_LED_10, GPIO_OUT);
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

void reset_leds(){
    gpio_put(LED_1, false);
    gpio_put(LED_2, false);
    gpio_put(LED_3, false);
}

void process_farenheit(float temp_f){
    reset_leds();
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

    //always keep the lowest light on. 
    gpio_put(PRESSURE_LED_1, true);

    

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
            printf("%d sample average -> Farenheit: %.4f F, Celsius: %.4f, h: %.4f m, Pressure: %.4f kPa\n", MPL3115A2_FIFO_SIZE, farenheit, celcius, altitude, pressure);           
            
        }
        
        sleep_ms(10);
        if(i == 100){
            i = -1;
            on_board_led_toggle();
        }
        i++;
    }
#endif
}
