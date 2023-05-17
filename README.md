# IntelBTPatcher

# All of the features are already merged into [IntelBluetoothFirmware](https://github.com/OpenIntelWireless/IntelBluetoothFirmware) project, if you are looking for the `IntelBTPatcher.kext`, please go to there for latest version instead, for now this repo is only for personal test.

A `Lilu` base patcher that fix Intel Bluetooth on Bigsur, Catalina, Mojave, High sierra etc, tested with Bigsur and Catalina all working good.

Note this is not prepare for daily using, while it is tested ok, it will be merge into `IntelBluetoothFirmware` repository.

## usage
Add `IntelBTPatcher.kext` to the bootloader, with `IntelBluetoothFirmware.kext` and `IntelBluetoothInjector.kext`

If succesfully connected or not, please open an issue to report with below information:

- network card eg: ax200
- usb id eg: 0x8087 0x0032
- platform eg: Bigsur
- device: eg: mx master2
- result: eg: succeed/fail

if fail, attach packetlogger log can help to resolve problems.
  
**Do not use it on Monterey**
