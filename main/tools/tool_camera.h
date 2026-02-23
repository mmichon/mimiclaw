#pragma once

#include "esp_err.h"
#include <stddef.h>

void tool_camera_init(void);

esp_err_t tool_take_photo_execute(const char *input_json, char *output, size_t output_size);
esp_err_t tool_send_photo_execute(const char *input_json, char *output, size_t output_size);
