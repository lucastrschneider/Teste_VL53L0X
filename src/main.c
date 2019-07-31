/**
 * @file main.c
 *
 * @brief Main function
 */

#include "usart.h"
#include <errno.h>
#include <sys/unistd.h>
#include <stdio.h>

#include "mcu.h"
#include "gpio.h"

#include "thunder_vl53l0x.h"

/*****************************************
 * Private Constant Definitions
 *****************************************/

#define ERROR_NONE 0

int _write(int file, char* data, int len) {
    if ((file != STDOUT_FILENO) && (file != STDERR_FILENO)) {
        errno = EBADF;
        return -1;
    }

    // arbitrary timeout 1000
    HAL_StatusTypeDef status =
        HAL_UART_Transmit(&huart2, (uint8_t*) data, len, 1000);

    // return # of bytes written - as best we can tell
    return status == HAL_OK ? len : 0;
}

/*****************************************
 * Main Function
 *****************************************/
__attribute__((used)) static uint16_t distance[] = {0, 0, 0, 0, 0};

int main(void) {
    uint8_t Status = ERROR_NONE;

    mcu_init();
    MX_USART2_UART_Init();

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

    Status = vl53l0x_init();

    if (Status != ERROR_NONE) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

        for (;;)
            ;
    }

    for (;;) {
        if (Status == ERROR_NONE) {
            Status |= just_update();

            if (Status == ERROR_NONE) {
                for (distance_sensor_position_t i = DS_SIDE_RIGHT; i < DS_MAX_POSITION; i++) {
                    distance[i] = vl53l0x_get(i);
                }
            }

            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        } else {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
            Status = vl53l0x_reinit();

            // OBS: reinit nao implementado
        }

        mcu_sleep(10);
    }
}

// FAZER CODIGO DE CALIBRAÇÃO DOS SENSORES
