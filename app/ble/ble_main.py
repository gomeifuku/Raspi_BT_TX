import bluepy

HANDLE_ID = 0x000b
devadr = "04:91:62:1A:D8:7E" #kari
class BLE_Manager:
    def __init__(self):
        self._peri = bluepy.btle.Peripheral()
        self._peri.connect(devadr, bluepy.btle.ADDR_TYPE_PUBLIC)

    def sendCWRotation(self):
        self._peri.writeCharacteristic(HANDLE_ID,b'\x01',True)

    def sendACWRotation(self):
        self._peri.writeCharacteristic(HANDLE_ID,b'\x00',True)
