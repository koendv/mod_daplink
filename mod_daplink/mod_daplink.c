// Include MicroPython API.
#include "py/obj.h"
#include "py/objarray.h"
#include "py/objstr.h"
#include "py/runtime.h"

#include "DAP.h"

static const char daplink_help_text[] =
    "daplink.init() -- reset dap adapter\n"
    "daplink.process_request(request, response) -- process dap request\n"
    "daplink.hid_info -- hid descriptor for pyb.usb_mode\n"
;

static mp_obj_t mp_daplink_help();
static mp_obj_t mp_daplink___init__();
static mp_obj_t mp_daplink_init();
static mp_obj_t mp_daplink_process_request(mp_obj_t req, mp_obj_t resp);
static const mp_rom_obj_tuple_t mp_daplink_hidinfo_obj;

static MP_DEFINE_CONST_FUN_OBJ_0(mp_daplink_help_obj, mp_daplink_help);
static MP_DEFINE_CONST_FUN_OBJ_0(mp_daplink___init___obj, mp_daplink___init__);
static MP_DEFINE_CONST_FUN_OBJ_0(mp_daplink_init_obj, mp_daplink_init);
static MP_DEFINE_CONST_FUN_OBJ_2(mp_daplink_process_request_obj, mp_daplink_process_request);

static const mp_rom_map_elem_t daplink_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_daplink) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&mp_daplink___init___obj) },
    { MP_ROM_QSTR(MP_QSTR_help), MP_ROM_PTR(&mp_daplink_help_obj) },
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&mp_daplink_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_process_request), MP_ROM_PTR(&mp_daplink_process_request_obj) },
    { MP_ROM_QSTR(MP_QSTR_hid_info), MP_ROM_PTR(&mp_daplink_hidinfo_obj) },
};

static MP_DEFINE_CONST_DICT(daplink_module_globals, daplink_module_globals_table);

const mp_obj_module_t daplink_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&daplink_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_daplink, daplink_module);

/* dap hid descriptor for usb_mode(hid=daplink.hid_info) */

static const uint8_t
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

static const mp_obj_str_t usb_hid_daplink_desc_obj = {
    {&mp_type_bytes},
    0, // hash not valid
    sizeof(USBD_HID_DAP_ReportDesc),
    USBD_HID_DAP_ReportDesc,
};

static const mp_rom_obj_tuple_t mp_daplink_hidinfo_obj = {
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

static mp_obj_t mp_daplink_help() {
    mp_print_str(MP_PYTHON_PRINTER, daplink_help_text);
    return mp_const_none;
}

/* auto initialize */

static mp_obj_t mp_daplink___init__() {
    DAP_Setup();
    return mp_const_none;
}

static mp_obj_t mp_daplink_init() {
    DAP_Setup();
    return mp_const_none;
}

/* call dap from micropython.
   arguments:
     - request: string or bytes, 64 bytes long
     - response: string or bytes, 64 bytes long
   daplink.process_request(app_request, app_response))[1] is zero if a response does not need to be sent.
*/

static mp_obj_t mp_daplink_process_request(mp_obj_t req, mp_obj_t resp) {
    if (!mp_obj_is_str_or_bytes(req) || !mp_obj_is_str_or_bytes(resp)) {
        mp_raise_ValueError(MP_ERROR_TEXT("expected string or bytes"));
    }
    GET_STR_DATA_LEN(req, app_request_buffer, app_request_len);
    GET_STR_DATA_LEN(resp, app_response_buffer, app_response_len);
    if ((app_request_len != 64) || (app_response_len != 64)) {
        mp_raise_ValueError(MP_ERROR_TEXT("expected 64 bytes"));
    }

    uint32_t retval = DAP_ProcessCommand(app_request_buffer, (byte *)app_response_buffer);

    mp_obj_tuple_t *tuple = MP_OBJ_TO_PTR(mp_obj_new_tuple(2, NULL));
    tuple->items[0] = MP_OBJ_NEW_SMALL_INT((retval >> 16) & 0xffff);  // number of bytes in request
    tuple->items[1] = MP_OBJ_NEW_SMALL_INT(retval & 0xffff);          // number of bytes in response
    return MP_OBJ_FROM_PTR(tuple);
}

/* not truncated */
