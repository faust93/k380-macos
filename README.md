### Logitech K380 Bluetooth Keyboard F-n keys mode switcher for MacOS

F-n keys are working in non-standard mode by default on this keyboard.
I do not want to mess with "Logi Options" just for switching so I made this little app.

#### How to use
$ sudo ./k380 -f on|off

### Additional info to build from sources

The app depends on https://github.com/libusb/hidapi to communicate with HID subsystem
But there's a known problem [https://github.com/libusb/hidapi/issues/127] with hidapi library and device paths enumeration on MacOS so patch is required:
https://github.com/flirc/hidapi/commit/8d251c3854c3b1877509ab07a623dafc8e803db5
( or `0001-Mac-use-IORegistryEntryGetRegistryEntryID-to-resolve.patch` which matches with hidapi sources version 0.10.1 )

