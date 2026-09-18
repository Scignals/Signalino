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
| Signalino Suite | Connect, inspect impedances, view EEG, record sessions and publish LSL | [0.11.0 Preview](https://github.com/Scignals/Signalino/releases/tag/v0.11.0) for macOS, Windows and Linux |
| Android Bridge + HepatoSense | BLE/paired Classic acquisition, recording, EDF export and exploratory qEEG | [Android 0.4.0 Preview](https://github.com/Scignals/Signalino/releases/tag/v0.10.0), Android 12+ |
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

**Installing on another PC?** Use the
[Windows/Linux installation and test checklist](docs/PRUEBAS_0.10.0.md)
(Spanish), with direct download links and a short results template.

Signalino Suite preview installers are available from this repository's
[Releases](https://github.com/Scignals/Signalino/releases). Firmware images
will be added after the corresponding hardware release is frozen. During the
current development phase, devices are programmed and supported directly by
Scignals.

See [firmware availability and model selection](docs/firmware/README.md).

See the [0.11.0 installation notes and known limitations](docs/releases/0.11.0.md).
The software mock needs Python; physical Windows USB/Classic/BLE acquisition does
not. Automated mock tests do not establish physical Bluetooth compatibility.

Android downloads include **both** Signalino Bridge and HepatoSense. Install the
matched pair; no Python or BrainFlow is needed on the phone. Read ANDROID_README.md
in the ZIP before updating development APKs: the distribution signature differs,
and uninstalling a development app deletes its private recordings and settings.
Export and verify your recordings first. Android does not yet publish LSL over
Wi-Fi or acquire over USB.

The historical public repository has been preserved as
[signalino-legacy](https://github.com/Scignals/signalino-legacy).

## Support

See [SUPPORT.md](SUPPORT.md) before opening an issue. Please include the device
generation, operating system, connection type and relevant software versions.

Signalino is currently intended for research and development. It is not a
substitute for certified medical equipment.

Copyright (c) 2026 Scignals.
