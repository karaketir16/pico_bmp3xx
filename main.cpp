#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "bmp3.h"

#include <iostream>

#include "hardware/uart.h"

#include "tusb.h"

#include <Adafruit_BMP3XX.h>

#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 0
#define UART_RX_PIN 1

void core1_entry() {
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        gpio_put(LED_PIN, 1);
        sleep_ms(1000);
        gpio_put(LED_PIN, 0);
        sleep_ms(1000);
    }
}

int main_2();
int main_scan();

int main() {
    stdio_init_all();
    multicore_launch_core1(core1_entry);

    uart_init(UART_ID, BAUD_RATE);

    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    uint32_t t0,t1;

    t0 = time_us_32();
    while (!tud_cdc_connected()) {}
    t1 = time_us_32();
    printf("\nusb host detected! (%dus)\n", t1-t0);


    // main_2();
    // main_scan();

    Adafruit_BMP3XX bmp3xx;
    bmp3xx.begin_I2C();
    while(true){
        sleep_ms(250);
        auto pressure = bmp3xx.readPressure();
        printf("pressure: %f\r\n", pressure);
    }

    return 0;
}
