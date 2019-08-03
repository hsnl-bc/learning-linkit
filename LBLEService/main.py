import asyncio
from bleak import BleakClient

Address = "DB:42:00:2B:88:8C"
Characteristic_uuid = "72312f2c-9419-4dc8-bc95-acdfe81e8cb1"

async def run(Address, loop):
    async with BleakClient(Address, loop=loop) as client:
        while True:
            data = await client.read_gatt_char(Characteristic_uuid)
            split_array=format("".join(map(chr, data))).split(':')
            json_output={
                'humidity':split_array[0],
                'temperature': split_array[1]
            }
            print(json_output)
            await asyncio.sleep(5)
        

loop = asyncio.get_event_loop()
loop.run_until_complete(run(Address, loop))
