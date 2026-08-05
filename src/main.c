#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

#define ROW1 4
#define ROW2 32
#define COL1 19
#define COL2 23
#define DEBOUNCE_TIME 50000
const char* TAG = "MACROPAD";

/*
configure row and column gpio
*/

uint8_t row_pins[2] = {4,32};
uint8_t col_pins[2] = {19,23};
uint8_t key_state[2][2] = {
                        {0,0},
                        {0.0}
                        };
int64_t last_time[2][2] = {{0,0},{0,0}};       // last accepted change time

gpio_config_t config_row = {
    .mode = GPIO_MODE_OUTPUT,
    .pin_bit_mask = (1ULL << ROW1) | (1ULL << ROW2),
    .pull_up_en = GPIO_PULLUP_ENABLE,
    .intr_type = GPIO_INTR_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE
};

gpio_config_t config_column = {
    .mode = GPIO_MODE_INPUT,
    .pin_bit_mask = (1ULL << COL1) | (1ULL << COL2),
    .pull_up_en = GPIO_PULLUP_ENABLE,
    .intr_type = GPIO_INTR_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE
};

void app_main() {
    gpio_config(&config_row);
    gpio_config(&config_column);

    for (;;) {
        for (int i = 0; i < 2; i++) {
            gpio_set_level(row_pins[i], 0);

            for (int j = 0; j < 2; j++) {
                
                //debounce logic

                int64_t now = esp_timer_get_time();
                int level = gpio_get_level(col_pins[j]);
                uint8_t pressed = (level == 0) ? 1 : 0;
                //first part prevents holding of the key to be registered as a "key press" as this loop is fast
                //second part prevents actual debouncing of the key

                if (pressed != key_state[i][j] && (now - last_time[i][j]) > DEBOUNCE_TIME) {
                    key_state[i][j] = pressed;
                    last_time[i][j] = now;

                    if (pressed) {
                        ESP_LOGI(TAG, "%i %i", i, j);
                    }
                }
            }

            gpio_set_level(row_pins[i], 1); 
        }

        vTaskDelay(pdMS_TO_TICKS(30)); 
    }
}