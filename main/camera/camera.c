/*
 * Camera driver for XIAO ESP32-S3 Sense (OV2640/OV3660)
 * Uses esp32-camera component (esp_camera.h)
 */
#include "camera.h"
#include "camera_pins.h"

#include "esp_camera.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

static const char *TAG = "camera";
static bool s_camera_initialized = false;

esp_err_t camera_init(void)
{
    if (s_camera_initialized) {
        return ESP_OK;
    }

    camera_config_t config = {
        .pin_pwdn = CAM_PIN_PWDN,
        .pin_reset = CAM_PIN_RESET,
        .pin_xclk = CAM_PIN_XCLK,
        .pin_sccb_sda = CAM_PIN_SIOD,
        .pin_sccb_scl = CAM_PIN_SIOC,

        .pin_d7 = CAM_PIN_D7,
        .pin_d6 = CAM_PIN_D6,
        .pin_d5 = CAM_PIN_D5,
        .pin_d4 = CAM_PIN_D4,
        .pin_d3 = CAM_PIN_D3,
        .pin_d2 = CAM_PIN_D2,
        .pin_d1 = CAM_PIN_D1,
        .pin_d0 = CAM_PIN_D0,

        .pin_vsync = CAM_PIN_VSYNC,
        .pin_href = CAM_PIN_HREF,
        .pin_pclk = CAM_PIN_PCLK,

        .xclk_freq_hz = 16000000,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,

        .pixel_format = PIXFORMAT_JPEG,
        .frame_size = FRAMESIZE_VGA,    /* 640x480 - reliable on XIAO Sense */
        .jpeg_quality = 12,
        .fb_count = 1,
        .fb_location = CAMERA_FB_IN_PSRAM,
        .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
    };

    esp_err_t err = ESP_FAIL;
    for (int attempt = 1; attempt <= 3; attempt++) {
        err = esp_camera_init(&config);
        if (err == ESP_OK) break;
        ESP_LOGW(TAG, "Camera init attempt %d failed: %s", attempt, esp_err_to_name(err));
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera init failed after 3 attempts: %s", esp_err_to_name(err));
        return err;
    }

    s_camera_initialized = true;
    ESP_LOGI(TAG, "Camera initialized (XIAO ESP32-S3 Sense)");
    return ESP_OK;
}

bool camera_is_ready(void)
{
    return s_camera_initialized;
}

esp_err_t camera_capture_jpeg(const char *path, size_t *out_len)
{
    if (!s_camera_initialized) {
        esp_err_t err = camera_init();
        if (err != ESP_OK) return err;
    }

    /* Discard initial frames so the OV2640 AE/AWB can stabilize */
    for (int i = 0; i < 3; i++) {
        camera_fb_t *warmup = esp_camera_fb_get();
        if (warmup) esp_camera_fb_return(warmup);
    }

    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(TAG, "Camera capture failed");
        return ESP_FAIL;
    }

    if (fb->format != PIXFORMAT_JPEG) {
        ESP_LOGE(TAG, "Expected JPEG, got format %d", fb->format);
        esp_camera_fb_return(fb);
        return ESP_ERR_INVALID_STATE;
    }

    FILE *f = fopen(path, "wb");
    if (!f) {
        ESP_LOGE(TAG, "Failed to open %s for write", path);
        esp_camera_fb_return(fb);
        return ESP_FAIL;
    }

    size_t written = fwrite(fb->buf, 1, fb->len, f);
    fclose(f);
    esp_camera_fb_return(fb);

    if (written != fb->len) {
        ESP_LOGE(TAG, "Write incomplete: %d/%d", (int)written, (int)fb->len);
        return ESP_FAIL;
    }

    if (out_len) *out_len = written;
    ESP_LOGI(TAG, "Captured %d bytes to %s", (int)written, path);
    return ESP_OK;
}
