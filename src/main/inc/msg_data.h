#ifndef __MSG_DATA_H__
#define __MSG_DATA_H__

#include <stdint.h>
#include <stdio.h>

/* 基本类型定义 */
typedef float f32;
typedef double f64;
typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
typedef size_t usize;

/**
 * TODO: 传感器数据结构体
 */
// AP3216C
typedef struct {
    float ir;
    float ps;
    float als;
    uint8_t status;
} ap3216c_data_t;

// BEEP
typedef struct {
    uint8_t open;
    uint8_t close;
    uint8_t status;
} beep_data_t;

// DHT11
typedef struct {
    float temp;
    float humi;
    uint8_t status;
} dht11_data_t;

// FIRE
typedef struct {
    float raw;
    float voltage;
    uint8_t status;
} fire_data_t;

// ICM20608
typedef struct {
    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
    uint8_t status;
} icm20608_data_t;

// LED
typedef struct {
    uint8_t rled;
    uint8_t gled;
    uint8_t bled;
} led_data_t;

// LIGHT
typedef struct {
    float raw;
    float voltage;
    float lux;
    uint8_t status;
} light_data_t;

#endif // __MSG_DATA_H__