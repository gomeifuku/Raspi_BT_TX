from bluepy import btle
from APP_SETTING import BD_TARGET_UUID
from get_bd_addr import get_bd_addr
import struct
#Set Complete Local Name
bd_addr=get_bd_addr("Blank")
p = btle.Peripheral(bd_addr,btle.ADDR_TYPE_RANDOM)

#Set Specific UUID
s = p.getServiceByUUID(BD_TARGET_UUID)
print("Get Service Success")
c = s.getCharacteristics()[0]
w_data=bytes("Hello world\n", "utf-8")
print('Write:',w_data)
c.write(w_data, withResponse=True)
print('Read:',c.read())

p.disconnect()

        