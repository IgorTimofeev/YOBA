# Work in fucking progress

Inf days left, go away for now, touch some grass instead of me...

------

# YOBA | Your Breathtaking Application

A comprehensive and fully independent UI framework written on modern C++ for embedded devices.

# ESP-IDF

```cpp
    ...
```

# Arduino

```cpp
    ...
```

# Platformio

C++17 or greater and RTTI are required, so you'll need to tweak `platformio.ini` like

```ini
[env:yobaExample]
platform = espressif32
board = esp32dev
framework = arduino

; This ↓
build_unflags =
	-fno-rtti
	-std=gnu++11
build_flags =
	-std=gnu++17
```
