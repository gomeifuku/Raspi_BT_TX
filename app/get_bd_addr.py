from bluepy import btle

def get_bd_addr(AD_Type_name):
    # Initialize Scannar Device Class
    #/dev/hci0
    scanner = btle.Scanner(0)
    # Scan Device
    devices = scanner.scan(3.0)
    for device in devices:
        for(adTypeCode, description, valueText) in device.getScanData():
            if(description=="Complete Local Name") and (valueText==AD_Type_name):
                print('MACアドレス：',device.addr)
                return device.addr