#pragma once

#include "esp_err.h"
#include <stdbool.h>
#include <stddef.h>

esp_err_t camera_init(void);
bool camera_is_ready(void);

/**
 * Capture a JPEG frame and save to the given path.
 * @param path    Full path (e.g. /spiffs/photos/capture.jpg)
 * @param out_len Optional; receives bytes written
 */
esp_err_t camera_capture_jpeg(const char *path, size_t *out_len);
