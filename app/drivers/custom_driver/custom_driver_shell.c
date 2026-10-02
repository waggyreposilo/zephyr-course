#include <zephyr/shell/shell.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

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

SHELL_STATIC_SUBCMD_SET_CREATE(sub_custom_driver,
    SHELL_CMD(fetch, NULL, "Calls sensor_sample_fetch", shell_custom_driver_sample_fetch),
    SHELL_CMD(get, NULL, "Calls sensor_channel_get and prints the result", shell_custom_driver_channel_get),
    SHELL_CMD(info, NULL, "Prints Device Name and ready state", shell_custom_driver_info),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_custom_driver, "Custom Driver Shell Commands", NULL);