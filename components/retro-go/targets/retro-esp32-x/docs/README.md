# Retro-ESP32-X

- Status: Community
- Ref: https://github.com/retro-esp32/

# Hardware info

- Module: ESP32-WROVER-B
- Extended with cartridge slot on the rear Game Boy housing for modular sensor/expansion boards

# Cartridge Module System

The RetroESP32-X exposes an I2C bus (SDA=GPIO16, SCL=GPIO17) through the Game Boy-style
cartridge slot on the back of the housing. This enables plug-in sensor and expansion modules
to be detected and used at runtime.

## How it works

1. At boot, the firmware scans the cartridge slot I2C bus for connected modules.
2. If a module identification EEPROM (AT24CXX at address 0x50) is found, the module type
   and name are read from it.
3. If no EEPROM is present, the firmware probes well-known sensor addresses directly
   (e.g. MPU-6050 at 0x68, BME280 at 0x76/0x77, DS3231 at 0x68).
4. The detected module is made available via `rg_module_get()` for use by applications.

## Module identification EEPROM layout

When you build a custom module, include a small I2C EEPROM (e.g. AT24C02) at address 0x50
with the following layout:

| Byte offset | Length | Description                          |
|-------------|--------|--------------------------------------|
| 0           | 1      | Module type ID (see `rg_module_type_t`) |
| 1           | 1      | Module hardware version              |
| 2           | 30     | Module name (null-terminated string) |

## Supported module types

| ID   | Type              | Example ICs           |
|------|-------------------|-----------------------|
| 0x01 | IMU               | MPU-6050, ICM-42688   |
| 0x02 | Environmental     | BME280, BMP280        |
| 0x03 | Real-time clock   | DS3231, PCF8523       |
| 0x04 | GPS/GNSS          | NEO-6M (via UART, needs extra wiring) |
| 0xFE | Generic expansion | Custom I2C peripheral |

## Building a module

1. Wire your sensor/module to the cartridge slot connector:
   - SDA → cartridge pin connected to GPIO16
   - SCL → cartridge pin connected to GPIO17
   - VCC → 3.3 V rail from the cartridge connector
   - GND → GND rail from the cartridge connector
2. Optionally add an AT24C02 EEPROM at address 0x50 with the layout above so
   the firmware can identify your module by name and type.
3. Use `rg_module_get()` in your application to obtain the module info and
   `rg_i2c_read()` / `rg_i2c_write()` to communicate with your sensor.

> **Note on UART-based modules (e.g. GPS/GNSS):** The cartridge slot I2C bus
> uses GPIO16/GPIO17.  UART-based modules such as the NEO-6M need additional
> signal wiring (TX/RX lines) and are not directly addressable via I2C.
> For such modules, include an I2C identification EEPROM (type byte = 0x04)
> so that the firmware can detect the module's presence, while your application
> communicates with the GPS chip separately over UART.

## Cartridge slot pinout (example mapping)

Adapt these to your specific housing / PCB design:

| Cartridge pin | ESP32 GPIO | Function      |
|---------------|-----------|---------------|
| B4 / D4       | GPIO16    | I2C SDA       |
| B3 / D3       | GPIO17    | I2C SCL       |
| VCC           | 3V3       | Power supply  |
| GND           | GND       | Ground        |

# Images

![device.jpg](device.jpg)
