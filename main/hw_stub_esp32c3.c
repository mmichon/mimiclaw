/*
 * Hardware stub for ESP32-C3 - no display, RGB, buttons, IMU.
 * Provides no-op implementations so the rest of MimiClaw can build and run.
 */

#include "display/display.h"
#include "rgb/rgb.h"
#include "buttons/button_driver.h"
#include "ui/config_screen.h"
#include "imu/imu_manager.h"
#include "esp_err.h"
#include "buttons/multi_button.h"

/* button_driver.h declares this */
PressEvent BOOT_KEY_State = NONE_PRESS;

/* display */
esp_err_t display_init(void) { return ESP_OK; }
void display_show_banner(void) { }
void display_set_backlight_percent(uint8_t percent) { (void)percent; }
uint8_t display_get_backlight_percent(void) { return 50; }
void display_cycle_backlight(void) { }
bool display_get_banner_center_rgb(uint8_t *r, uint8_t *g, uint8_t *b) {
    if (r) *r = 0;
    if (g) *g = 0;
    if (b) *b = 0;
    return false;
}
void display_show_config_screen(const char *qr_text, const char *ip_text,
                                const char **lines, size_t line_count, size_t scroll,
                                size_t selected, int selected_offset_px) {
    (void)qr_text; (void)ip_text; (void)lines; (void)line_count;
    (void)scroll; (void)selected; (void)selected_offset_px;
}
void display_show_message_card(const char *title, const char *body) {
    (void)title; (void)body;
}

/* rgb */
esp_err_t rgb_init(void) { return ESP_OK; }
void rgb_set(uint8_t r, uint8_t g, uint8_t b) { (void)r; (void)g; (void)b; }

/* buttons */
void button_Init(void) { }

/* config_screen */
void config_screen_init(void) { }
void config_screen_toggle(void) { }
bool config_screen_is_active(void) { return false; }
void config_screen_scroll_down(void) { }

/* imu */
void imu_manager_init(void) { }
void imu_manager_set_shake_callback(imu_shake_cb_t cb) { (void)cb; }
