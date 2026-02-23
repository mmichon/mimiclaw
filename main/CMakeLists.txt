idf_build_get_property(idf_target IDF_TARGET)

if(idf_target STREQUAL "esp32c3")
    set(MAIN_SRCS
        "mimi.c"
        "hw_stub_esp32c3.c"
        "bus/message_bus.c"
        "wifi/wifi_manager.c"
        "telegram/telegram_bot.c"
        "llm/llm_proxy.c"
        "agent/agent_loop.c"
        "agent/context_builder.c"
        "memory/memory_store.c"
        "memory/session_mgr.c"
        "gateway/ws_server.c"
        "cli/serial_cli.c"
        "ota/ota_manager.c"
        "proxy/http_proxy.c"
        "cron/cron_service.c"
        "heartbeat/heartbeat.c"
        "tools/tool_registry.c"
        "tools/tool_cron.c"
        "tools/tool_web_search.c"
        "tools/tool_get_time.c"
        "tools/tool_files.c"
        "skills/skill_loader.c"
    )
    set(MAIN_REQUIRES
        nvs_flash esp_wifi esp_netif esp_http_client esp_http_server
        esp_https_ota esp_event json spiffs console vfs app_update esp-tls
        driver esp_timer
    )
    # No display on ESP32-C3, no banner to embed
else()
    set(MAIN_SRCS
        "mimi.c"
        "buttons/multi_button.c"
        "buttons/button_driver.c"
        "imu/I2C_Driver.c"
        "imu/QMI8658.c"
        "imu/imu_manager.c"
        "bus/message_bus.c"
        "wifi/wifi_manager.c"
        "telegram/telegram_bot.c"
        "llm/llm_proxy.c"
        "agent/agent_loop.c"
        "agent/context_builder.c"
        "memory/memory_store.c"
        "memory/session_mgr.c"
        "gateway/ws_server.c"
        "cli/serial_cli.c"
        "proxy/http_proxy.c"
        "cron/cron_service.c"
        "heartbeat/heartbeat.c"
        "tools/tool_registry.c"
        "tools/tool_cron.c"
        "tools/tool_web_search.c"
        "tools/tool_get_time.c"
        "tools/tool_files.c"
        "tools/tool_camera.c"
        "camera/camera.c"
        "skills/skill_loader.c"
    )
    set(MAIN_REQUIRES
        nvs_flash esp_wifi esp_netif esp_http_client esp_http_server
        esp_https_ota esp_event json spiffs console vfs app_update esp-tls
        driver esp_lcd esp_timer led_strip qrcode
    )
    set(MAIN_EMBED_FILES "../assets/banner_320x172.rgb565")
endif()

if(idf_target STREQUAL "esp32c3")
    idf_component_register(
        SRCS ${MAIN_SRCS}
        INCLUDE_DIRS "."
        REQUIRES ${MAIN_REQUIRES}
    )
else()
    idf_component_register(
        SRCS ${MAIN_SRCS}
        INCLUDE_DIRS "."
        EMBED_FILES ${MAIN_EMBED_FILES}
        REQUIRES ${MAIN_REQUIRES}
    )
endif()
