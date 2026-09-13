# Signalino support

## Before reporting a problem

Record the following information:

- Signalino model or hardware generation;
- device name or displayed serial suffix;
- operating system and version;
- connection type: USB, Bluetooth Classic or BLE;
- Signalino Suite, Python package and firmware versions;
- whether the problem persists after reconnecting and restarting the device.

For software defects and reproducible integration problems, open a GitHub
issue in this repository. Include the shortest sequence of steps that produces
the problem and the relevant diagnostic log. Do not post patient data,
credentials or private recordings.

## Python API

Confirm the installed version with:

```bash
python -c "import signalino; print(signalino.__version__)"
```

Package information and installation instructions are available on
[PyPI](https://pypi.org/project/signalino/).

## Safety

Do not connect development hardware to a person while it is attached to
non-isolated mains-powered equipment. Signalino development software and
prototype hardware are intended for research and engineering evaluation, not
for diagnosis or clinical decision-making.
