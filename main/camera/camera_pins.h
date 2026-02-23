/*
 * XIAO ESP32-S3 Sense camera pin definitions (OV2640 / OV3660)
 * See: https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/#camera-pins
 */
#pragma once

/* XIAO ESP32-S3 Sense DVP camera pins */
#define CAM_PIN_PWDN    -1
#define CAM_PIN_RESET   -1
#define CAM_PIN_XCLK    10
#define CAM_PIN_SIOD    40   /* SCCB SDA / I2C data */
#define CAM_PIN_SIOC    39   /* SCCB SCL / I2C clock */

#define CAM_PIN_D0      15   /* DVP Y2 */
#define CAM_PIN_D1      17   /* DVP Y3 */
#define CAM_PIN_D2      18   /* DVP Y4 */
#define CAM_PIN_D3      16   /* DVP Y5 */
#define CAM_PIN_D4      14   /* DVP Y6 */
#define CAM_PIN_D5      12   /* DVP Y7 */
#define CAM_PIN_D6      11   /* DVP Y8 */
#define CAM_PIN_D7      48   /* DVP Y9 */

#define CAM_PIN_VSYNC   38
#define CAM_PIN_HREF    47
#define CAM_PIN_PCLK    13
