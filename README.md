# Signalino

Signalino is an eight-channel platform for EEG and biopotential research. It
combines ADS1299-based acquisition hardware with desktop, Python, LSL,
BrainFlow and BCI2000 software integrations.

This repository is the public home of the Signalino project. Installers,
firmware releases and public documentation will be published here as they
complete hardware validation.

## Software

| Component | Purpose | Availability |
| --- | --- | --- |
| Signalino Suite | Connect, inspect impedances, view EEG, record sessions and publish LSL | macOS, Windows and Linux installers in preparation |
| `signalino` for Python | Native USB, Bluetooth Classic and BLE acquisition API | [Available on PyPI](https://pypi.org/project/signalino/) |
| BrainFlow integration | Use Signalino from BrainFlow-compatible applications | Release candidate |
| BCI2000 integration | Native Signalino source and impedance support | Under validation |
| S4CP | Compact device control and diagnostics protocol | Documentation in preparation |

Install the public Python API with:

```bash
python -m pip install signalino
```

Optional LSL, MNE and viewer support can be installed with:

```bash
python -m pip install "signalino[all]"
```

## Hardware generations

### Signalino S3

The current controller is based on ESP32-S3 and ADS1299. It supports native
USB and Bluetooth Low Energy, device identity, impedance measurements,
microSD recording and auxiliary sensor data when present.

### Signalino Teensy

Original Teensy 3.6 units remain supported over USB and paired Bluetooth
Classic. Updated firmware uses the same S4CP control protocol and 33-byte EEG
frame as the current software stack.

## Data interfaces

- Eight signed 24-bit EEG channels at 250 samples per second by default.
- OpenBCI-compatible 33-byte binary frames.
- Stable device identity for selecting one of several Signalinos.
- LSL EEG, sensor and marker streams through Signalino Suite.
- Direct Python acquisition over USB, Bluetooth Classic or BLE.
- Synthetic and ADS1299 test sources for diagnostics.

## Downloads

Validated Signalino Suite installers and firmware images will be attached to
this repository's [Releases](https://github.com/Scignals/Signalino/releases).
During the current development phase, devices are programmed and supported
directly by Scignals.

The historical public repository has been preserved as
[signalino-legacy](https://github.com/Scignals/signalino-legacy).

## Support

See [SUPPORT.md](SUPPORT.md) before opening an issue. Please include the device
generation, operating system, connection type and relevant software versions.

Signalino is currently intended for research and development. It is not a
substitute for certified medical equipment.

Copyright (c) 2026 Scignals.
