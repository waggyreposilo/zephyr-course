#define DT_DRV_COMPAT custom_driver

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

#include "include/custom_driver.h"

struct custom_config{
    struct gpio_dt_spec led;
};

struct custom_data{
    bool invert;
};

static int custom_driver_sample_fetch(const struct device *dev,enum sensor_channel type){
    const struct custom_config *config = dev->config;

    struct custom_data *data = dev->data;

    return gpio_pin_set_dt(&config->led, data->invert ? 0 : 1);
}

static int custom_driver_channel_get(const struct device *dev,enum sensor_channel chan,struct sensor_value *val){
    const struct custom_config *config = dev->config;
    struct custom_data *data = dev->data;
    val->val1 = 0;
    val->val2 = 0;

    return gpio_pin_set_dt(&config->led, data->invert ? 1 : 0);
}

static int custom_driver_invert(const struct device *dev, bool invert){
    struct custom_data *data = dev->data;
    data->invert = invert;

    return 0;
}

/*
static DEVICE_API(sensor,custom_driver_api) = {
    .sample_fetch = custom_driver_sample_fetch,
    .channel_get  = custom_driver_channel_get,
};
*/

static const struct custom_driver_api custom_driver_api = {
    .sensor_api = {
        .sample_fetch = custom_driver_sample_fetch,
        .channel_get  = custom_driver_channel_get,
    },
    .set_invert = custom_driver_invert,
};

static int custom_driver_init(const struct device *dev){
    const struct custom_config *config = dev->config;

    if(!gpio_is_ready_dt(&config->led)){
        return -ENODEV;
    }

    return gpio_pin_configure_dt(&config->led, GPIO_OUTPUT_INACTIVE);
}

#define CUSTOM_DRIVER_INIT(inst) \
    static struct custom_data custom_driver_data_##inst; \
    static const struct custom_config custom_driver_config_##inst = { \
        .led = GPIO_DT_SPEC_INST_GET(inst, led_gpios), \
    }; \
    SENSOR_DEVICE_DT_INST_DEFINE(inst, &custom_driver_init, NULL, &custom_driver_data_##inst, \
                          &custom_driver_config_##inst, POST_KERNEL, \
                          CONFIG_SENSOR_INIT_PRIORITY, &custom_driver_api);

DT_INST_FOREACH_STATUS_OKAY(CUSTOM_DRIVER_INIT)