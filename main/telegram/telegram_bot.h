#pragma once

#include "esp_err.h"

/**
 * Initialize the Telegram bot.
 */
esp_err_t telegram_bot_init(void);

/**
 * Start the Telegram polling task (long polling on Core 0).
 */
esp_err_t telegram_bot_start(void);

/**
 * Send a text message to a Telegram chat.
 * Automatically splits messages longer than 4096 chars.
 * @param chat_id  Telegram chat ID (numeric string)
 * @param text     Message text (supports Markdown)
 */
esp_err_t telegram_send_message(const char *chat_id, const char *text);

/**
 * Send a photo from SPIFFS to a Telegram chat.
 * @param chat_id  Telegram chat ID (numeric string)
 * @param path     Full path to JPEG file (e.g. /spiffs/photos/capture.jpg)
 */
esp_err_t telegram_send_photo(const char *chat_id, const char *path);

/**
 * Save the Telegram bot token to NVS.
 */
esp_err_t telegram_set_token(const char *token);

