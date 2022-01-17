// Include MicroPython API.
#include "py/obj.h"
#include "py/objarray.h"
#include "py/objstr.h"
#include "py/runtime.h"

#include "DAP.h"

STATIC const char daplink_help_text[] =
    "daplink.init() -- reset dap adapter\n"
    "daplink.process_request(request, response) -- process dap request\n"
    "daplink.hid_info -- hid descriptor for pyb.usb_mode\n"
;

STATIC mp_obj_t mp_daplink_help();
STATIC mp_obj_t mp_daplink___init__();
STATIC mp_obj_t mp_daplink_init();
STATIC mp_obj_t mp_daplink_process_request(mp_obj_t req, mp_obj_t resp);
STATIC const mp_rom_obj_tuple_t mp_daplink_hidinfo_obj;

STATIC MP_DEFINE_CONST_FUN_OBJ_0(mp_daplink_help_obj, mp_daplink_help);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mp_daplink___init___obj, mp_daplink___init__);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mp_daplink_init_obj, mp_daplink_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mp_daplink_process_request_obj, mp_daplink_process_request);

STATIC const mp_rom_map_elem_t daplink_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_daplink) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&mp_daplink___init___obj) },
    { MP_ROM_QSTR(MP_QSTR_help), MP_ROM_PTR(&mp_daplink_help_obj) },
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&mp_daplink_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_process_request), MP_ROM_PTR(&mp_daplink_process_request_obj) },
    { MP_ROM_QSTR(MP_QSTR_hid_info), MP_ROM_PTR(&mp_daplink_hidinfo_obj) },
};

STATIC MP_DEFINE_CONST_DICT(daplink_module_globals, daplink_module_globals_table);

const mp_obj_module_t daplink_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&daplink_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_daplink, daplink_module, 1);

/* dap hid descriptor for usb_mode(hid=daplink.hid_info) */

STATIC const uint8_t
    USBD_HID_DAP_ReportDesc[] = {
    0x05, 0x01,      // Usage Page (Generic Desktop Ctrls)
    0x09, 0x00,      // Usage (Undefined)
    0xa1, 0x01,      // Collection (Application)
    0x15, 0x00,      //   Logical Minimum (0)
    0x26, 0xff, 0x00, //   Logical Maximum (255)
    0x75, 0x08,      //   Report Size (8)
    0x95, 0x40,      //   Report Count (64)
    0x09, 0x00,      //   Usage (Undefined)
    0x81, 0x82,      //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x75, 0x08,      //   Report Size (8)
    0x95, 0x40,      //   Report Count (64)
    0x09, 0x00,      //   Usage (Undefined)
    0x91, 0x82,      //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Volatile)
    0xc0,            // End Collection
};

STATIC const mp_obj_str_t usb_hid_daplink_desc_obj = {
    {&mp_type_bytes},
    0, // hash not valid
    sizeof(USBD_HID_DAP_ReportDesc),
    USBD_HID_DAP_ReportDesc,
};

STATIC const mp_rom_obj_tuple_t mp_daplink_hidinfo_obj = {
    {&mp_type_tuple},
    5,
    {
        MP_ROM_INT(0),                     // subclass: none
        MP_ROM_INT(0),                     // protocol: none
        MP_ROM_INT(64),                    // full-speed usb. max packet length: 64
        MP_ROM_INT(8),                     // polling interval: 8ms
        MP_ROM_PTR(&usb_hid_daplink_desc_obj), // report descriptor
    },
};

/* code */

STATIC void mp_obj_get_data(mp_obj_t mp_obj, size_t *len, uint8_t **items);

STATIC mp_obj_t mp_daplink_help() {
    mp_print_str(MP_PYTHON_PRINTER, daplink_help_text);
    return mp_const_none;
}

/* auto initialize */

STATIC mp_obj_t mp_daplink___init__() {
    DAP_Setup();
    return mp_const_none;
}

STATIC mp_obj_t mp_daplink_init() {
    DAP_Setup();
    return mp_const_none;
}

/* call dap from micropython.
   arguments:
     - request: string, bytes, or bytearray (64 bytes)
     - response: string, bytes, or bytearray (64 bytes)
   returns true if response needs to be sent; false if response does not need to
   be sent.
*/

STATIC mp_obj_t mp_daplink_process_request(mp_obj_t req, mp_obj_t resp) {
    size_t app_request_len;
    uint8_t *app_request_buffer = {0};
    size_t app_response_len;
    uint8_t *app_response_buffer = {0};

    mp_obj_get_data(req, &app_request_len, &app_request_buffer);
    mp_obj_get_data(resp, &app_response_len, &app_response_buffer);

    uint32_t retval = DAP_ProcessCommand(app_request_buffer, app_response_buffer);

    mp_obj_tuple_t *tuple = MP_OBJ_TO_PTR(mp_obj_new_tuple(2, NULL));
    tuple->items[0] = MP_OBJ_NEW_SMALL_INT((retval >> 16) & 0xffff);  // number of bytes in request
    tuple->items[1] = MP_OBJ_NEW_SMALL_INT(retval & 0xffff);          // number of bytes in response
    return MP_OBJ_FROM_PTR(tuple);
}

/* access to bytes/bytearray contents and length */

/*
   input:
   mp_obj: micropython string, bytes or bytearray
   output:
   len: mp_obj length
   items: mp_obj data
 */

STATIC void mp_obj_get_data(mp_obj_t mp_obj, size_t *len, uint8_t **items) {
    if (mp_obj_is_type(MP_OBJ_FROM_PTR(mp_obj), &mp_type_bytearray)) {
        mp_obj_array_t *barray = MP_OBJ_FROM_PTR(mp_obj);
        *len = barray->len;
        *items = barray->items;
        return;
    }
    if (mp_obj_is_type(MP_OBJ_FROM_PTR(mp_obj), &mp_type_bytes) ||
        mp_obj_is_type(MP_OBJ_FROM_PTR(mp_obj), &mp_type_str)) {
        mp_obj_str_t *str = MP_OBJ_FROM_PTR(mp_obj);
        *len = str->len;
        *items = (void *)str->data;
        return;
    }
    mp_raise_TypeError(MP_ERROR_TEXT("expected string, bytes or bytearray"));
}

/* not truncated */
