#include <zephyr/shell/shell.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#include "include/custom_driver.h"

static int shell_custom_driver_sample_fetch(const struct shell *shell, size_t argc, char **argv){
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(custom_sensor0));

    if(!device_is_ready(dev)){
        shell_print(shell, "Device not ready");
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(dev);
    if(ret < 0){
        shell_print(shell, "Failed to fetch sample: %d", ret);
        return ret;
    }

    shell_print(shell, "Sample fetched successfully");
    return 0;
}

static int shell_custom_driver_channel_get(const struct shell *shell, size_t argc, char **argv){
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(custom_sensor0));

    if(!device_is_ready(dev)){
        shell_print(shell, "Device not ready");
        return -ENODEV;
    }

    struct sensor_value val;
    int ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
    if(ret < 0){
        shell_print(shell, "Failed to get channel data: %d", ret);
        return ret;
    }

    shell_print(shell, "Channel data: val1=%d, val2=%d", val.val1, val.val2);
    return 0;
}

static int shell_custom_driver_info(const struct shell *shell, size_t argc, char **argv){
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(custom_sensor0));

    if(!device_is_ready(dev)){
        shell_print(shell, "Device not ready");
        return -ENODEV;
    }

    shell_print(shell, "Device Name: %s", dev->name);
    shell_print(shell, "Device Ready State: %s", device_is_ready(dev) ? "Ready" : "Not Ready");

    return 0;
}

static int shell_custom_driver_invert(const struct shell *shell, size_t argc, char **argv){
    if(argc != 2){
        shell_error(shell, "Invalid number of arguments. Usage: invert <0|1>");
        return -EINVAL;
    }

    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(custom_sensor0));

    if(!device_is_ready(dev)){
        shell_error(shell, "Device not ready");
        return -ENODEV;
    }

    bool invert = (strcmp(argv[1], "1") == 0);
    int ret = custom_driver_set_invert(dev, invert);
    if(ret < 0){
        shell_error(shell, "Failed to set invert: %d", ret);
        return ret;
    }

    shell_print(shell, "Invert set to: %s", invert ? "true" : "false");
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_custom_driver,
    SHELL_CMD(fetch, NULL, "Calls sensor_sample_fetch", shell_custom_driver_sample_fetch),
    SHELL_CMD(get, NULL, "Calls sensor_channel_get and prints the result", shell_custom_driver_channel_get),
    SHELL_CMD(info, NULL, "Prints Device Name and ready state", shell_custom_driver_info),
    SHELL_CMD_ARG(invert, NULL, "Sets the invert property of the sensor. Usage: invert <0|1>", shell_custom_driver_invert, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_custom_driver, "Custom Driver Shell Commands", NULL);