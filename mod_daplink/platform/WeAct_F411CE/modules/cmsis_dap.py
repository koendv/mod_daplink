def run():
    import pyb
    import daplink
    daplink.init()
    app_request=bytes(64)
    app_response=bytes(64)
    usb_hid=pyb.USB_HID()
    while(1):
        if (usb_hid.recv(app_request) and (daplink.process_request(app_request, app_response))[1] != 0):
            usb_hid.send(app_response)
