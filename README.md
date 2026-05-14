# pala-one-firmware
Pala One — A tiny E-Ink reader project by Paul Lagier

The goal of the project was to create a simple, distraction-free reading device that feels minimal, portable and easy to build while still looking and behaving more like a real product than a typical DIY electronics project.

## Contributing

If you improve the firmware, add features or fix bugs, feel free to open a pull request.
Please clearly mention:
- which board version you modified
- what was changed
- how it was tested

## Board Versions

There are currently two supported Heltec Wireless Paper versions:
- `Heltec V1.1`
- `Heltec V1.2`
Please make sure to use the correct firmware version for your specific board revision.
The board version is usually printed on the back of the PCB.

## Features

- TXT book support
- Adjustable font size
- Adjustable line spacing
- Deep sleep mode
- Reading progress saving
- USB-C charging
- Lightweight portable design
- Open-source firmware

## Fork Updates (Firmware 2.1.1)

This fork includes multiple firmware and WebUI improvements for the Heltec V1.2 build:

- Bionic Reading toggle in settings (bold word starts for faster scanning)
- Reading font dropdown with `Default` and `OpenDyslexia` options
- Dark mode toggle in WebUI
- Reading-location retention on layout changes so font/font-size/line-spacing updates keep the user near the same sentence instead of jumping to page 1
- Single and multi screensaver modes in the WebUI Settings page
- Mode-specific UI (single mode and multi mode are shown separately)
- Multi-screensaver rotation list with per-image thumbnail previews
- Add/remove images from rotation directly in WebUI
- Rotation order options: cycle or shuffle
- Shuffle avoids immediate back-to-back repeats
- Single custom screensaver also shows a thumbnail with inline remove action
- Screensaver thumbnails are generated on-device using the same bitmap pipeline as displayed images

### Multi-screensaver behavior

- Multi mode stores rotation images in slot files (`/sleep-slot-0.bin`, etc.)
- Each rotation image must be raw XBM bytes, exactly `3904` bytes (`250x122`, 1-bit, LSB-first)
- Single mode uses `/sleep.bin`
- If multi mode is enabled and rotation slots exist, the single custom image is ignored until single mode is re-enabled

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
