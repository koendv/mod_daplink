#include "info.h"
#include "esp_system.h"
#include "py/mpconfig.h"
#include "py/misc.h"

// Strings
static char string_unique_id[48 + 1];
static char string_version[] = MICROPY_VERSION_STRING;
static const char hexdigits[] = "0123456789ABCDEF";

static char * hexify(char *hex, const void *buf, size_t size) {
    char *tmp = hex;
    const uint8_t *b = buf;

    while (size--) {
        *tmp++ = hexdigits[*b >> 4];
        *tmp++ = hexdigits[*b++ & 0xF];
    }
    *tmp++ = 0;

    return hex;
}

const char *info_get_unique_id(void) {
    uint8_t id[6];
    esp_efuse_mac_get_default(id);
    uint32_t id_len = MIN(sizeof(id), sizeof(string_unique_id) / 2 - 1);
    hexify(string_unique_id, &id, id_len);
    return string_unique_id;
}

const char *info_get_version(void) {
    return string_version;
}
