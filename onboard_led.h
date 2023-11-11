#include "pico/stdlib.h"

#ifndef ON_BOARD_LED
#define ON_BOARD_LED 25
#endif

static uint led_pin = ON_BOARD_LED;
static bool state = false;

void on_board_led_init(){
    on_board_led_init(false);
}

void on_board_led_init_configured_state(bool on){
    state = on;
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    gpio_put(led_pin, state);
}

void on_board_led_toggle(){
    state = !state;
    gpio_put(led_pin, state);
}