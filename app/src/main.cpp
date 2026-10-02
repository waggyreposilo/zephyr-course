#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>

#include "include/custom_driver.h"

//#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "led0" alias. */
//#define LED_NODE DT_ALIAS(led0)
//#define LED_NODE DT_ALIAS(customled)

//static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
//LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define CUSTOM_SENSOR_NODE  DT_NODELABEL(custom_sensor0)

bool invert = false;
int cycles = 0;

int main(void)
{
    //bool led_state = true;
    //if (!gpio_is_ready_dt(&led)) return 0;
    //if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    const struct device *dev = DEVICE_DT_GET(CUSTOM_SENSOR_NODE);

    struct sensor_value val;
    if(!device_is_ready(dev)){
        return 0;
    }

    while (1) {
        /*if (gpio_pin_toggle_dt(&led) < 0) return 0;

        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        //k_msleep(SLEEP_TIME_MS);
        //k_msleep(CONFIG_LED_BLINK_SLEEPTIME);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);*/

        int ret = sensor_sample_fetch(dev);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
        
        ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);

        if (++cycles >= 5) {
            cycles = 0;
            invert = !invert;
            custom_driver_set_invert(dev, invert);
        }
    }
    return 0;
}
