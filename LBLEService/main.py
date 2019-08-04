import asyncio
from bleak import BleakClient
import json
import requests
import time

# Device
Address = "DB:42:00:2B:88:8C"

# LBLE characteristic UUID
Characteristic_uuids = [
    "72312f2c-9419-4dc8-bc95-acdfe81e8cb1"
]

# MCS api format
MCS_api="https://api.mediatek.com/mcs/v2/devices/DOuHxFwk/datapoints"
MCS_headers={
    "deviceKey":"NjeKwKAxS359W0c1",
    "Content-Type":"application/json"
}
DataChnIds={
    "humidity":"1",
    "temperature":"2"
}

def mcs_request(data):
    obj = json.loads(data)
    # Return if sensor datachannel is not pre-defined
    for key in  obj["data"] :
        if key not in DataChnIds:
            print("No exist sensor channel")
            continue
        # Wrap it, ref to https://mcs.mediatek.com/resources/latest/api_references/
        body={
            "datapoints":[]
        }
        body["datapoints"].append({
            "dataChnId":DataChnIds[key],
            "values":{
                "value":str(obj["data"][key])
            }
        })
        r = requests.post(
            MCS_api,
            data=json.dumps(body),
            headers=MCS_headers
        )
    
async def run(Address, loop):
    async with BleakClient(Address, loop=loop) as client:
        status = await client.is_connected()
        print('Connected?'+str(status))
        while True:
            for uuid in Characteristic_uuids:
                data = await client.read_gatt_char(uuid)
                print(data.decode('utf-8'))
                mcs_request(data.decode('utf-8'))
            await asyncio.sleep(5)
        
        
loop = asyncio.get_event_loop()
loop.run_until_complete(run(Address, loop))
