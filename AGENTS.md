# MimiClaw — Agent Notes

**Repo:** https://github.com/memovai/mimiclaw
**Hardware:** ESP32-S3 with integrated camera, 16 MB flash, 8 MB PSRAM
**Firmware:** C/FreeRTOS via ESP-IDF v5.5+, no Linux

## After changing any code

### 1. Build

```bash
idf.py build
```

If you changed `mimi_secrets.h`, run `idf.py fullclean && idf.py build` first.

### 2. Flash and monitor

**Try serial first** (fastest, always works):

```bash
# Find the port
ls /dev/cu.usb*

# Flash and tail logs
idf.py -p /dev/cu.usbmodem* flash monitor
```

**If no serial port is available, use OTA over the network:**

```bash
# Serve the build output
cd build && python3 -m http.server 8080

# Then send this URL to the bot on Telegram or via the serial CLI:
#   http://YOUR_PC_IP:8080/mimiclaw.bin
```

### 3. Verify the logs

A healthy boot looks like:

```
I (xxx) mimi: MimiClaw - ESP32-S3 AI Agent
I (xxx) mimi: PSRAM free: ~8000000 bytes
I (xxx) wifi: WiFi connected: 192.168.x.x
I (xxx) telegram: Telegram bot token loaded
I (xxx) mimi: All services started!
```

Watch for crash dumps, assertion failures, or task stack overflows. If the device boot-loops, re-flash via USB and check the serial logs for the crash reason.

Exit the monitor with `Ctrl+]`.
