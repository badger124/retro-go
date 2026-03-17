#include "rg_system.h"
#include "rg_module.h"
#include "rg_i2c.h"

#include <string.h>

#ifdef RG_CARTRIDGE_MODULE_ENABLED

/* -----------------------------------------------------------------------
 * I2C addresses of the module identification EEPROM and known sensors
 * ---------------------------------------------------------------------- */

/* AT24CXX EEPROM used as a module-ID ROM (standard address range 0x50-0x57) */
#define MODULE_EEPROM_ADDR      0x50

/* EEPROM layout offsets */
#define EEPROM_OFF_TYPE         0   /* 1 byte: rg_module_type_t            */
#define EEPROM_OFF_VERSION      1   /* 1 byte: hardware version            */
#define EEPROM_OFF_NAME         2   /* 30 bytes: null-terminated name      */
#define EEPROM_NAME_LEN         30

/* Well-known sensor I2C addresses for EEPROM-less modules */
#define IMU_MPU6050_ADDR        0x68  /* WHO_AM_I register 0x75 → 0x68    */
#define IMU_MPU6050_WHOAMI_REG  0x75
#define IMU_MPU6050_WHOAMI_VAL  0x68

#define ENV_BME280_ADDR_A       0x76  /* SDO pulled low  */
#define ENV_BME280_ADDR_B       0x77  /* SDO pulled high */
#define ENV_BME280_CHIPID_REG   0xD0
#define ENV_BME280_CHIPID_VAL   0x60  /* BME280 chip id  */
#define ENV_BMP280_CHIPID_VAL   0x58  /* BMP280 chip id  */

/* NOTE: DS3231 and MPU-6050 share address 0x68, making direct probing
 * ambiguous.  RTC modules should include an identification EEPROM so they
 * are detected via read_eeprom_info() rather than probe_known_sensors(). */

/* -----------------------------------------------------------------------
 * Module state
 * ---------------------------------------------------------------------- */

static rg_module_info_t current_module;
static bool module_initialized = false;

/* -----------------------------------------------------------------------
 * Internal helpers
 * ---------------------------------------------------------------------- */

/** Try to read module information from an identification EEPROM at addr. */
static bool read_eeprom_info(uint8_t addr, rg_module_info_t *out)
{
    uint8_t buf[1 + 1 + EEPROM_NAME_LEN] = {0};

    if (!rg_i2c_read(addr, EEPROM_OFF_TYPE, buf, sizeof(buf)))
        return false;

    /* Sanity check: type byte must be a recognised value */
    if (buf[EEPROM_OFF_TYPE] == RG_MODULE_NONE)
        return false;

    out->type     = (rg_module_type_t)buf[EEPROM_OFF_TYPE];
    out->version  = buf[EEPROM_OFF_VERSION];
    out->i2c_addr = addr;
    memcpy(out->name, buf + EEPROM_OFF_NAME, EEPROM_NAME_LEN);
    out->name[sizeof(out->name) - 1] = '\0'; /* guarantee termination */
    return true;
}

/** Probe well-known sensor addresses when there is no EEPROM. */
static bool probe_known_sensors(rg_module_info_t *out)
{
    int val;

    /* BME280 / BMP280 environmental sensor */
    uint8_t bme_addrs[] = {ENV_BME280_ADDR_A, ENV_BME280_ADDR_B};
    for (int i = 0; i < (int)RG_COUNT(bme_addrs); ++i)
    {
        val = rg_i2c_read_byte(bme_addrs[i], ENV_BME280_CHIPID_REG);
        if (val == ENV_BME280_CHIPID_VAL || val == ENV_BMP280_CHIPID_VAL)
        {
            out->type     = RG_MODULE_ENV_SENSOR;
            out->i2c_addr = bme_addrs[i];
            out->version  = 0;
            strncpy(out->name, val == ENV_BME280_CHIPID_VAL ? "BME280" : "BMP280", sizeof(out->name) - 1);
            return true;
        }
    }

    /* MPU-6050 IMU */
    val = rg_i2c_read_byte(IMU_MPU6050_ADDR, IMU_MPU6050_WHOAMI_REG);
    if (val == IMU_MPU6050_WHOAMI_VAL)
    {
        out->type     = RG_MODULE_IMU;
        out->i2c_addr = IMU_MPU6050_ADDR;
        out->version  = 0;
        strncpy(out->name, "MPU-6050", sizeof(out->name) - 1);
        return true;
    }

    return false;
}

/* -----------------------------------------------------------------------
 * Public API
 * ---------------------------------------------------------------------- */

bool rg_module_init(void)
{
    if (module_initialized)
        return current_module.type != RG_MODULE_NONE;

    memset(&current_module, 0, sizeof(current_module));
    current_module.type = RG_MODULE_NONE;

    if (!rg_i2c_init())
    {
        RG_LOGE("Module init failed: I2C bus unavailable.\n");
        module_initialized = true;
        return false;
    }

    /* 1. Try module identification EEPROM first */
    if (read_eeprom_info(MODULE_EEPROM_ADDR, &current_module))
    {
        RG_LOGI("Cartridge module detected via EEPROM: type=%d ver=%d name='%s'\n",
                current_module.type, current_module.version, current_module.name);
        module_initialized = true;
        return true;
    }

    /* 2. Fall back to probing known sensor addresses */
    if (probe_known_sensors(&current_module))
    {
        RG_LOGI("Cartridge module detected via probe: type=%d name='%s' addr=0x%02X\n",
                current_module.type, current_module.name, current_module.i2c_addr);
        module_initialized = true;
        return true;
    }

    RG_LOGI("No cartridge module detected.\n");
    module_initialized = true;
    return false;
}

bool rg_module_deinit(void)
{
    memset(&current_module, 0, sizeof(current_module));
    current_module.type = RG_MODULE_NONE;
    module_initialized  = false;
    return true;
}

const rg_module_info_t *rg_module_get(void)
{
    if (!module_initialized || current_module.type == RG_MODULE_NONE)
        return NULL;
    return &current_module;
}

bool rg_module_is_present(void)
{
    return module_initialized && current_module.type != RG_MODULE_NONE;
}

#else /* RG_CARTRIDGE_MODULE_ENABLED not defined */

bool rg_module_init(void)   { return false; }
bool rg_module_deinit(void) { return true;  }
const rg_module_info_t *rg_module_get(void) { return NULL; }
bool rg_module_is_present(void) { return false; }

#endif /* RG_CARTRIDGE_MODULE_ENABLED */
