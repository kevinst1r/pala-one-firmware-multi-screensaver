<img width="1892" height="1053" alt="palaOne" src="https://github.com/user-attachments/assets/0fdef5ba-eabd-4b71-9a0c-4c1dc78a4bee" />

# pala-one-firmware
Pala One — A tiny E-Ink reader project by Paul Lagier

The goal of the project was to create a simple, distraction-free reading device that feels minimal, portable and easy to build while still looking and behaving more like a real product than a typical DIY electronics project.

## Contributing

If you improve the firmware, add features or fix bugs, feel free to open a pull request.
Please clearly mention:
- which board version(s) you tested on (V1.1, V1.2, or both)
- what was changed
- how it was tested

## Board Versions

There are currently two supported Heltec Wireless Paper versions:
- `Heltec V1.1`
- `Heltec V1.2`

The board version is usually printed on the back of the PCB.

Both versions are built from the same source file (`Pala_One_2_1/Pala_One_2_1.ino`). Open it in Arduino IDE and uncomment the `#define` at the top that matches your hardware before compiling.

## Apps

Apps are compiled separately and uploaded to the device via the WiFi web interface — no firmware rebuild needed. The Apps menu discovers all `.bin` files in `/apps/` on the device and lists them by name.

### Building an app

You need:
- The `xtensa-esp32s3-elf-gcc` cross-compiler, which ships with the Arduino ESP32 board package. On Linux it is found under `~/.arduino15/packages/esp32/tools/esp-x32/<version>/bin/`; the `Makefile` locates it automatically.
- `python3` (for the post-build step that patches the entry point offset into the binary).

An app is a single C file that includes `pala_app.h` and `pala_api.h` from the firmware source and exports a `void app_main(const PalaAPI* api)` entry point. The header struct at the start of the binary carries the display name and API version check.

See `examples/click_counter/` for a complete working example with a `Makefile`.

```bash
cd examples/click_counter
make
# produces click_counter.bin
```

### Uploading an app

1. Select **Upload** from the library menu on the device.
2. Connect to the `PALA-XXXXXX` WiFi network (password: `palaread`).
3. Open `http://192.168.4.1` in a browser.
4. Use the **Upload app (.bin)** card to upload your `.bin` file.
5. Triple-click to exit upload mode — the app will appear in the Apps menu immediately.

### App API

Apps communicate with the firmware through the `PalaAPI` function pointer table passed to `app_main`. The current API version is **v3** (`PALA_API_VERSION 3` in `pala_app.h`).

#### Display

| Function | Description |
|---|---|
| `clearScreen()` | Clear the display buffer and prepare a new frame |
| `drawHeader(title)` | Draw the standard section header bar |
| `drawTextAt(x, y, text, bold)` | Draw text at a pixel position |
| `drawCenteredLarge(text)` | Draw text centred on screen in a large font |
| `refreshDisplay()` | Push the frame buffer to the e-ink panel |

#### Input

| Function | Description |
|---|---|
| `waitForEvent()` | Block until a button gesture; returns `PALA_CLICK` / `PALA_DOUBLE` / `PALA_TRIPLE` / `PALA_LONG` |
| `pollEvent()` | Non-blocking variant; returns 0 if no event is ready |
| `buttonPressed()` | Returns 1 if the button is currently held, 0 otherwise |
| `pendingPresses()` | Count of individual short press-release events since last call; bypasses multi-click grouping |

#### Timing

| Function | Description |
|---|---|
| `millisNow()` | Current uptime in milliseconds |
| `delayMs(ms)` | Yield for `ms` milliseconds |
| `rtcSeconds()` | Monotonic seconds counter that survives deep sleep; use for cross-session timing |

#### Storage

| Function | Description |
|---|---|
| `storageRead(key, buf, maxlen)` | Read from `/apps/{key}.dat`; returns bytes read, -1 on error |
| `storageWrite(key, buf, len)` | Write to `/apps/{key}.dat`; returns bytes written, -1 on error |

#### Utilities

| Function | Description |
|---|---|
| `snprintf_wrap(buf, len, fmt, ...)` | Standard `snprintf` |

Return from `app_main` to exit back to the Apps menu. Apps decide their own exit gesture — the firmware does not impose one.

**Constraints:**
- Apps must be compiled `-fPIC -mlongcalls` (position-independent).
- Apps must not use static mutable variables — the loader does not patch `.data` relocations.
- Maximum binary size: 48 KB.
- The `api_version` field in `PalaAppHeader` must match `PALA_API_VERSION` exactly — the firmware rejects mismatched binaries.

## Features

- TXT book support
- Adjustable font size
- Adjustable line spacing
- Deep sleep mode
- Reading progress saving
- USB-C charging
- Lightweight portable design
- Open-source firmware

## Kevinst1r Fork Updates

This fork includes multiple firmware and WebUI improvements for the Heltec V1.2 build:

- Bionic Reading toggle in settings (bold word starts for faster scanning)
- Reading font dropdown with `Default` and `OpenDyslexia` options (see below)
- Dark mode toggle in WebUI
- Reading-location retention on layout changes so font/font-size/line-spacing updates keep the user near the same sentence instead of jumping to page 1* (*Haven't re-added yet.  Need to test new firmware version first)
- Single and multi screensaver modes in the WebUI Settings page
- Mode-specific UI (single mode and multi mode are shown separately)
- Multi-screensaver rotation list with per-image thumbnail previews
- Add/remove images from rotation directly in WebUI
- Rotation order options: cycle or shuffle
- Shuffle avoids immediate back-to-back repeats
- Single custom screensaver also shows a thumbnail with inline remove action
- Screensaver thumbnails are generated on-device using the same bitmap pipeline as displayed images
- Built-in WebUI screensaver editor (phone/browser-side processing, no on-device image conversion)

### OpenDyslexia reading font

The `OpenDyslexia` setting uses **embedded [OpenDyslexic](https://github.com/antijingoist/open-dyslexic)** Regular and Bold outlines (SIL Open Font License), rasterized into **u8g2** bitmap fonts in firmware (`opendyslexic_u8g2_fonts.h`). It is not a separate file on disk and does not use the old Lucida Sans stand-in.

- **Glyph coverage:** printable ASCII only (`0x20`–`0x7E`). Other Unicode characters are not drawn with this face.
- **Size presets:** the 8 / 10 / 12 / 14 pt labels match the same **pixel heights** as the stock u8g2 Helvetica reader fonts (rasterized at **11 / 14 / 17 / 20 px** respectively), so line count and visual scale stay comparable to `Default`.

### Multi-screensaver behavior

- Multi mode stores rotation images in slot files (`/sleep-slot-0.bin`, etc.)
- Each rotation image must be raw XBM bytes, exactly `3904` bytes (`250x122`, 1-bit, LSB-first)
- Single mode uses `/sleep.bin`
- If multi mode is enabled and rotation slots exist, the single custom image is ignored until single mode is re-enabled

### WebUI screensaver editor

The Settings page now includes a built-in editor for creating and uploading valid sleep images directly from phone or desktop browser.

- Live preview rendered as exact device output (`250x122`, 1-bit)
- Controls: black tolerance and invert are always visible
- `Precise Control` section (collapsed by default): zoom + move X/Y
- Touch gestures on preview: drag to pan and pinch to zoom
- In multi mode, default destination is **Add to rotation slot (next free)** (no manual slot selection)
- If all 8 rotation slots are full, upload is blocked with a clear UI message

## Hardware

Pala One is based on:
- Heltec Wireless Paper
- 3D printed housing
- LiPo battery

## Downloads

This repository contains the firmware source code for the project.

Additional files such as:
- STL files
- STEP files
- assembly guides
- printable files
- project downloads

are available separately via Ko-fi:

https://ko-fi.com/s/e14ed892ea

## Community & Modifications

Community improvements, forks and firmware modifications are welcome.  
If you build your own version or improve the project, feel free to share it with the community.

## License & Copyright

The firmware in this repository is provided for personal and educational use.

Please do not:
- reupload paid project files
- redistribute complete download packages
- resell the project files
- commercially redistribute modified versions of paid assets

The design, branding, documentation and paid project assets remain copyright © Paul Lagier.

---

Created by Paul Lagier
