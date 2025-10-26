#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include <string.h>
    const uint LED_PIN = 25;
    const uint ADC_PIN = 26;
    bool led_is_on = false;
    #define DISK_SIZE 256
    uint8_t disk_memory[DISK_SIZE];
    char cmd_buffer[100];
    int cmd_index = 0;
 void process_command(char *command) {
      unsigned int lba, data;
      gpio_put(LED_PIN, 1);
      sleep_ms(50);
      printf("Command recieved: %s\n", command);
      if(strcmp(command, "PING") == 0){
      printf("PONG\n");
      }else if (strcmp(command, "LED_ON") == 0) {
        gpio_put(LED_PIN, 1);
        led_is_on = true;
        printf("ACK\n");
      }else if(strcmp(command, "LED_OFF") == 0){
        gpio_put(LED_PIN, 0);
        led_is_on = false;
        printf("ACK\n");
      }else if(strcmp(command, "LED_STATUS") == 0){
        if (led_is_on) {
        printf("STATUS: ON\n");
    } else {
        printf("STATUS: OFF\n");
    }
      }else if(strcmp(command, "READ_SMART") == 0){
        uint16_t adc_raw = adc_read();
        float temp = (adc_raw / 4095.0) * 40.0 + 10.0;
        printf("SMART_TEMP: %.1f\n", temp);;
      }else if(sscanf(command, "WRITE %u %u", &lba, &data) == 2){
        if (lba >= DISK_SIZE) {
            printf("ERROR: LBA_OUT_OF_BOUNDS\n");
        } else {
            disk_memory[lba] = (uint8_t)data;
            printf("WRITE_OK %u\n", lba);
        }
      }else if(sscanf(command, "READ %u ", &lba) == 1){
        if (lba >= DISK_SIZE) {
            printf("ERROR: LBA_OUT_OF_BOUNDS\n");
        } else {
          uint8_t data = disk_memory[lba];
            printf("READ_OK %u %u\n", lba, data);
        }
      }else{
        printf("Unknown command\n");
      }
      if (strcmp(command, "LED_ON") != 0) {
         gpio_put(LED_PIN, 0);
      }
    }
int main() {

    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(0);

    printf("System has started\n");
    while (true) {
      int c = getchar_timeout_us(100);
      if (c != PICO_ERROR_TIMEOUT) {
        if (c == '\n') {
          cmd_buffer[cmd_index] = '\0';
          process_command(cmd_buffer);
          cmd_index = 0;
        }else{
          if(cmd_index < 99){
            cmd_buffer[cmd_index] = c;
            cmd_index++;
          }
        }
      }
    }
}