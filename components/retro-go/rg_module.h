#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @file rg_module.h
 * @brief Cartridge slot hardware module detection and management.
 *
 * On targets that define RG_CARTRIDGE_MODULE_ENABLED the I2C bus exposed
 * through the cartridge-slot connector is scanned at startup.  Any sensor
 * or expansion board plugged into the slot is detected and its type/name
 * made available through this API.
 *
 * Detection strategy (in order):
 *  1. Read a module-identification EEPROM at I2C address 0x50.
 *     Byte 0 = module type, byte 1 = hardware version, bytes 2-31 = name.
 *  2. If no EEPROM is present, probe well-known sensor addresses directly
 *     (MPU-6050 at 0x68, BME280/BMP280 at 0x76/0x77, DS3231 at 0x68).
 */

/** Module type identifiers stored in the identification EEPROM. */
typedef enum
{
    RG_MODULE_NONE        = 0x00, /**< No module / slot empty              */
    RG_MODULE_IMU         = 0x01, /**< IMU (MPU-6050, ICM-42688, …)        */
    RG_MODULE_ENV_SENSOR  = 0x02, /**< Environmental sensor (BME280, …)    */
    RG_MODULE_RTC         = 0x03, /**< Real-time clock (DS3231, PCF8523, …)*/
    RG_MODULE_GPS         = 0x04, /**< GPS/GNSS module (e.g. NEO-6M)       */
    RG_MODULE_EXPANSION   = 0xFE, /**< Generic I2C expansion board          */
    RG_MODULE_UNKNOWN     = 0xFF, /**< Detected but type not recognised     */
} rg_module_type_t;

/** Information about a detected cartridge module. */
typedef struct
{
    rg_module_type_t type;     /**< Module type                            */
    uint8_t          i2c_addr; /**< Primary I2C address of the module      */
    uint8_t          version;  /**< Module hardware version (from EEPROM)  */
    char             name[32]; /**< Human-readable name (from EEPROM)      */
} rg_module_info_t;

/**
 * Initialise the module subsystem and detect any plugged-in module.
 * Safe to call when RG_CARTRIDGE_MODULE_ENABLED is not defined – it will
 * return false immediately without error.
 *
 * @return true if a module was successfully detected, false otherwise.
 */
bool rg_module_init(void);

/**
 * Deinitialise the module subsystem.
 * @return true on success.
 */
bool rg_module_deinit(void);

/**
 * Return information about the currently detected module.
 * The pointer is valid until rg_module_deinit() is called.
 *
 * @return Pointer to module info, or NULL if no module is present.
 */
const rg_module_info_t *rg_module_get(void);

/**
 * Convenience helper: returns true when a module other than
 * RG_MODULE_NONE is detected and the subsystem is initialised.
 */
bool rg_module_is_present(void);
