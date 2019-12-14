import bluepy

HANDLE_ID = 0x000b
devadr = "00:1E:C0:55:14:06"
class BLE_Manager:
    def __init__(self):
        self._peri = bluepy.btle.Peripheral()
        self._peri.connect(devadr, bluepy.btle.ADDR_TYPE_PUBLIC)

    def sendTriggerRotation(self):
        self._peri.writeCharacteristic(HANDLE_ID,b'\x01',True)
