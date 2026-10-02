#ifndef CUSTOM_DRIVER_H
#define CUSTOM_DRIVER_H

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdbool.h>
#include <errno.h>

#ifdef __cplusplus
extern "C"{
#endif

struct custom_driver_api{
    struct sensor_driver_api sensor_api;
    int (*set_invert)(const struct device *dev, bool invert);
};

static inline int custom_driver_set_invert(const struct device *dev, bool invert){
    const struct custom_driver_api *api = (const struct custom_driver_api *)dev->api;

    if(api->set_invert == NULL){
        return -ENOSYS;
    }

    return api->set_invert(dev,invert);
}

#ifdef __cplusplus
}
#endif

#endif