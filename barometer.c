#include <time.h>
#include "mpl3115a2.h"


int main() {
    stdio_init_all();
    #if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
#warning i2c / mpl3115a2_i2c example requires a board with I2C pins
    puts("Default I2C pins were not defined");
    return 0;
#else
   
    sensor_init();
    printf("Running!");
    while (true) {
        if (has_new_data) {
            read_data();

            time_t rawtime;
            struct tm * timeinfo;

            time ( &rawtime );
            timeinfo = localtime ( &rawtime );
            printf ( "Current local time and date: %s", asctime (timeinfo) );

            printf("%d sample average -> Farenheit: %.4f F, Celsius: %.4f, h: %.4f m\n", MPL3115A2_FIFO_SIZE, get_farenheit_temp(), get_celcius_temp(),
                   get_altitude());
            reset_reading();
        }
        sleep_ms(10);
    }
#endif
}
