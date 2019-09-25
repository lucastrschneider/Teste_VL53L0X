/**
 * @file distance_sensors.c
 *
 * @brief Example with 5 ToF sensors using the VL53L0X lib
 *
 * @date 08/2019
 */

#include "distance_sensors.h"
#include "mcu.h"

/*****************************************
 * Private Constants
 *****************************************/

#define INIT_RESET_SLEEP_TIME_MS 10 // TODO: pode diminuir isso?
#define MAX_RANGE 600

/*****************************************
 * Private Variables
 *****************************************/

static VL53L0X_Dev_t sensors[] = {
    { // 0
        .I2cDevAddr = VL53L0X_DEFAULT_ADDRESS,
        .comms_type = 1, // I2C
        .comms_speed_khz = VL53L0X_DEFAULT_COMM_SPEED,
        .present = 0,
        .calibrated = 0,
        .hi2c = &hi2c1,
        .xshut_port = GPIOA,
        .xshut_pin = GPIO_PIN_7
    },
    { // 1
        .I2cDevAddr = VL53L0X_DEFAULT_ADDRESS,
        .comms_type = 1, // I2C
        .comms_speed_khz = VL53L0X_DEFAULT_COMM_SPEED,
        .present = 0,
        .calibrated = 0,
        .hi2c = &hi2c1,
        .xshut_port = GPIOB,
        .xshut_pin = GPIO_PIN_0
    },
    { // 2
        .I2cDevAddr = VL53L0X_DEFAULT_ADDRESS,
        .comms_type = 1, // I2C
        .comms_speed_khz = VL53L0X_DEFAULT_COMM_SPEED,
        .present = 0,
        .calibrated = 0,
        .hi2c = &hi2c1,
        .xshut_port = GPIOC,
        .xshut_pin = GPIO_PIN_7
    },
    { // 3
        .I2cDevAddr = VL53L0X_DEFAULT_ADDRESS,
        .comms_type = 1, // I2C
        .comms_speed_khz = VL53L0X_DEFAULT_COMM_SPEED,
        .present = 0,
        .calibrated = 0,
        .hi2c = &hi2c1,
        .xshut_port = GPIOA,
        .xshut_pin = GPIO_PIN_9
    },
    { // 4
        .I2cDevAddr = VL53L0X_DEFAULT_ADDRESS,
        .comms_type = 1, // I2C
        .comms_speed_khz = VL53L0X_DEFAULT_COMM_SPEED,
        .present = 0,
        .calibrated = 0,
        .hi2c = &hi2c1,
        .xshut_port = GPIOA,
        .xshut_pin = GPIO_PIN_8
    }
};
static VL53L0X_DeviceInfo_t sensors_info[DS_AMOUNT];
static VL53L0X_RangingMeasurementData_t sensors_measurement[DS_AMOUNT];
static vl53l0x_calibration_data_t sensors_calibration[DS_AMOUNT];

static uint16_t actual_range[] = {MAX_RANGE, MAX_RANGE, MAX_RANGE, MAX_RANGE, MAX_RANGE};
static const uint8_t i2c_addresses[] = {0x30, 0x34, 0x38, 0x3C, 0x40};
__attribute__((used)) static uint8_t sensors_status[] = {0, 0, 0, 0, 0};

/*****************************************
 * Public Functions Bodies Definitions
 *****************************************/

uint8_t distance_sensors_init() {
    VL53L0X_Error global_status = VL53L0X_ERROR_NONE;

    // desabilita todos, independente de quantos vai usar
    for (int i = 0; i < DS_AMOUNT; i++) {
        vl53l0x_turn_off(&(sensors[i]));
    }

    mcu_sleep(INIT_RESET_SLEEP_TIME_MS);

    for (int i = 0; i < DS_AMOUNT; i++) {
        if (!used_sensors[i]) {
            continue;
        }

        VL53L0X_Error status = VL53L0X_ERROR_NONE;
        VL53L0X_Dev_t* p_device = &(sensors[i]);

        status = vl53l0x_wait_boot(p_device);

        if (status == VL53L0X_ERROR_NONE) {
            status = VL53L0X_SetDeviceAddress(p_device, i2c_addresses[i]);
        }

        if (status == VL53L0X_ERROR_NONE) {
            p_device->I2cDevAddr = i2c_addresses[i];
            status = vl53l0x_init(p_device, sensors_info[i], sensors_calibration[i]);
        }

        if (status == VL53L0X_ERROR_NONE) {
            p_device->present = 1;
            p_device->calibrated = 1;
        }

        global_status |= status;
    }

    if (global_status == VL53L0X_ERROR_NONE) {
        return 0;
    }

    return 1;
}

uint8_t distance_sensors_update() {
    uint8_t status = 0;

    for (int i = 0; i < DS_AMOUNT; i++) {
        if (!used_sensors[i]) {
            continue;
        }

        sensors_status[i] =
            vl53l0x_update_range(&(sensors[i]), &(sensors_measurement[i]), &(actual_range[i]), MAX_RANGE);

        if (sensors_status[i] != 0) {
            status |= 1 << (i + 1);
        }
    }

    return status;
}

uint16_t distance_sensors_get(distance_sensor_position_t sensor) {
    if ((sensors[(int) sensor]).present) {
        return actual_range[(int) sensor];
    }

    return -1;
}

/**
 * @brief vl53l0x_plataform function, that needs to be implemented by the application
 */

VL53L0X_Error VL53L0X_PollingDelay(VL53L0X_DEV Dev) {
    mcu_sleep(VL53L0X_POLLINGDELAY_LOOPNB);
    return VL53L0X_ERROR_NONE;
}
