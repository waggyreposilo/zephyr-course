#include <zephyr/init.h>
#include <zephyr/kernel.h>
static int board_l5_task2_init(void) {
    
    printf("Board Initialized\n");
    return 0;
    
}

SYS_INIT(board_l5_task2_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);