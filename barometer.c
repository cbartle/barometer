#include <time.h>
#include <math.h>
#include "mpl3115a2.h"
#include "onboard_led.h"

int LED_1 = 0;
int LED_2 = 1;
int LED_3 = 2;

void init_leds(){
    on_board_led_init_configured_state(true);

    gpio_init(LED_1);
    gpio_set_dir(LED_1, GPIO_OUT);
    gpio_init(LED_2);
    gpio_set_dir(LED_2, GPIO_OUT);
    gpio_init(LED_3);
    gpio_set_dir(LED_3, GPIO_OUT);
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

int main() {
    stdio_init_all();
    #if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
#warning i2c / mpl3115a2_i2c example requires a board with I2C pins
    puts("Default I2C pins were not defined");
    return 0;
#else
    init_leds();

    printf("initializing sensor");
    sensor_init(false);
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

            process_farenheit(farenheit);
           printf("%d sample average -> Farenheit: %.4f F, Celsius: %.4f, h: %.4f m\n", MPL3115A2_FIFO_SIZE, farenheit, celcius, altitude);
            
            
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
