/*
 * Camera tools: take_photo, send_photo
 * For XIAO ESP32-S3 Sense with OV2640/OV3660
 */
#include "tools/tool_camera.h"
#include "camera/camera.h"
#include "telegram/telegram_bot.h"
#include "bus/message_bus.h"
#include "mimi_config.h"

#include <string.h>
#include <stdio.h>
#include "esp_log.h"
#include "cJSON.h"

static const char *TAG = "tool_camera";

#define PHOTO_PATH "/spiffs/capture.jpg"
#define PHOTO_PREFIX "photo:"
#define PHOTO_PREFIX_LEN 6

void tool_camera_init(void)
{
    /* Camera init is lazy on first capture */
}

esp_err_t tool_take_photo_execute(const char *input_json, char *output, size_t output_size)
{
    cJSON *root = cJSON_Parse(input_json ? input_json : "{}");
    if (!root) root = cJSON_CreateObject();

    char send_to_chat_id[32] = {0};
    cJSON *j = cJSON_GetObjectItem(root, "send_to_chat_id");
    if (cJSON_IsString(j) && j->valuestring && j->valuestring[0]) {
        strncpy(send_to_chat_id, j->valuestring, sizeof(send_to_chat_id) - 1);
    }
    cJSON_Delete(root);

    esp_err_t err = camera_capture_jpeg(PHOTO_PATH, NULL);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera capture failed: %s", esp_err_to_name(err));
        snprintf(output, output_size, "{\"error\":\"Camera capture failed: %s\"}",
                 esp_err_to_name(err));
        return err;
    }

    if (send_to_chat_id[0] && strcmp(send_to_chat_id, "cron") != 0) {
        mimi_msg_t msg = {0};
        strncpy(msg.channel, MIMI_CHAN_TELEGRAM, sizeof(msg.channel) - 1);
        strncpy(msg.chat_id, send_to_chat_id, sizeof(msg.chat_id) - 1);
        size_t len = strlen(PHOTO_PREFIX) + strlen(PHOTO_PATH) + 1;
        msg.content = malloc(len);
        if (msg.content) {
            snprintf(msg.content, len, "%s%s", PHOTO_PREFIX, PHOTO_PATH);
            if (message_bus_push_outbound(&msg) == ESP_OK) {
                snprintf(output, output_size,
                         "{\"path\":\"%s\",\"sent\":true,\"chat_id\":\"%s\"}",
                         PHOTO_PATH, send_to_chat_id);
            } else {
                free(msg.content);
                snprintf(output, output_size,
                         "{\"path\":\"%s\",\"sent\":false,\"error\":\"Outbound queue full\"}",
                         PHOTO_PATH);
            }
        } else {
            snprintf(output, output_size, "{\"path\":\"%s\",\"sent\":false}", PHOTO_PATH);
        }
    } else {
        snprintf(output, output_size, "{\"path\":\"%s\"}", PHOTO_PATH);
    }

    return ESP_OK;
}

esp_err_t tool_send_photo_execute(const char *input_json, char *output, size_t output_size)
{
    cJSON *root = cJSON_Parse(input_json ? input_json : "{}");
    if (!root) {
        snprintf(output, output_size, "{\"error\":\"Invalid JSON\"}");
        return ESP_ERR_INVALID_ARG;
    }

    cJSON *path_item = cJSON_GetObjectItem(root, "path");
    cJSON *chat_item = cJSON_GetObjectItem(root, "chat_id");

    char path_buf[64] = {0};
    char chat_id_buf[32] = {0};
    if (cJSON_IsString(path_item) && path_item->valuestring)
        strncpy(path_buf, path_item->valuestring, sizeof(path_buf) - 1);
    if (cJSON_IsString(chat_item) && chat_item->valuestring)
        strncpy(chat_id_buf, chat_item->valuestring, sizeof(chat_id_buf) - 1);
    cJSON_Delete(root);

    if (path_buf[0] == '\0') {
        snprintf(output, output_size, "{\"error\":\"Missing required field: path\"}");
        return ESP_ERR_INVALID_ARG;
    }

    if (chat_id_buf[0] == '\0' || strcmp(chat_id_buf, "cron") == 0) {
        snprintf(output, output_size, "{\"error\":\"Missing or invalid chat_id\"}");
        return ESP_ERR_INVALID_ARG;
    }

    mimi_msg_t msg = {0};
    strncpy(msg.channel, MIMI_CHAN_TELEGRAM, sizeof(msg.channel) - 1);
    strncpy(msg.chat_id, chat_id_buf, sizeof(msg.chat_id) - 1);
    size_t len = strlen(PHOTO_PREFIX) + strlen(path_buf) + 1;
    msg.content = malloc(len);
    if (!msg.content) {
        snprintf(output, output_size, "{\"error\":\"Out of memory\"}");
        return ESP_ERR_NO_MEM;
    }
    snprintf(msg.content, len, "%s%s", PHOTO_PREFIX, path_buf);

    esp_err_t push_err = message_bus_push_outbound(&msg);
    if (push_err != ESP_OK) {
        free(msg.content);
        snprintf(output, output_size, "{\"error\":\"Outbound queue full\"}");
        return push_err;
    }

    snprintf(output, output_size, "{\"status\":\"Photo queued for send to %s\"}", chat_id_buf);
    return ESP_OK;
}
