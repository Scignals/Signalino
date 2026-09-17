# Signalino firmware downloads

Status: 2026-09-17. There are no customer firmware downloads in this repository's
releases yet. Current devices are programmed and supported directly by Scignals.

| Amplifier | Firmware family | Download status |
| --- | --- | --- |
| Original Signalino with Teensy 3.6 | Signalino Teensy | Release package pending validation and distribution review |
| Signalino Controller Rev D5, ESP32-S3-WROOM-1-N8 | Signalino S3 D5 | Definitive hardware validation and release package pending |

Firmware packages will appear in
[Releases](https://github.com/Scignals/Signalino/releases) with the supported
hardware revision, version, installation/recovery instructions, license notices
and integrity checksums. Application and firmware versions are independent.

Do not interchange Teensy and S3 firmware or assume that any ESP32-S3 development
board has the D5 pinout, flash size or PSRAM configuration. Bench emulators and
synthetic hardware mocks are not amplifier firmware downloads.

No firmware update is required merely to install the current Suite preview.
For recovery of an existing amplifier, use [the support instructions](../../SUPPORT.md)
and identify the hardware model before attempting a flash.
