/**
 * @file dsitance_sensors.h
 *
 * @brief Example using 5 Time Of Flight sensors with the VL53L0X lib
 *
 * @date 08/2019
 */

#ifndef DISTANCE_SENSORS_H
#define DISTANCE_SENSORS_H

#include "vl53l0x.h"

/*****************************************
 * Public Constants
 *****************************************/

typedef enum _distance_sensor_position {
    DS_SIDE_RIGHT = 0,
    DS_FRONT_RIGHT = 1,
    DS_FRONT_CENTER = 2,
    DS_FRONT_LEFT = 3,
    DS_SIDE_LEFT = 4,

    DS_AMOUNT = 5
} distance_sensor_position_t;

/*****************************************
 * Public Functions Prototypes
 *****************************************/

uint8_t distance_sensors_init();

uint8_t distance_sensors_update();

uint16_t distance_sensors_get(distance_sensor_position_t sensor);

#endif
