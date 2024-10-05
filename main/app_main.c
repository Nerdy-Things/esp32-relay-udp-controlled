// socat - UDP-DATAGRAM:255.255.255.255:9341,broadcast

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include <inttypes.h>
#include <string.h>
#include <nerdy_wifi.h>
#include <nerdy_udp_server.h>
#include <nerdy_udp_server_event.h>

static const char *TAG = "MAIN";

#define RELAY_PIN (gpio_num_t) GPIO_NUM_4
#define MESSAGE_ON "BOO!"

int port = 9341;

/**
 * Receives UDP messages from network
*/
static void nerdy_udp_server_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) 
{
    if (event_base != NERDY_UDP_SERVER_EVENT) return;
    if (event_id == NERDY_UDP_SERVER_EVENT_MESSAGE) {
        char *message = (char*)event_data;
        ESP_LOGI(TAG, "Received in main: %s", message);
        if (strncmp(message, MESSAGE_ON, strlen(MESSAGE_ON)) == 0) {
            gpio_set_level(RELAY_PIN, 1);
            ESP_LOGI(TAG, "Turning ON");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            gpio_set_level(RELAY_PIN, 0);
            ESP_LOGI(TAG, "Turning OFF");
        }
    }
}

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

    gpio_set_direction(RELAY_PIN, GPIO_MODE_OUTPUT);
    nerdy_wifi_connect();

    nerdy_udp_server_start(port);
    esp_event_handler_instance_t instance;
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(
            NERDY_UDP_SERVER_EVENT, 
            NERDY_UDP_SERVER_EVENT_MESSAGE, 
            &nerdy_udp_server_event_handler,
            NULL, 
            &instance
        )
    );
}
