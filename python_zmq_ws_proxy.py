#!/usr/bin/python3
import asyncio
import websockets
import json
import base64
import zmq

CONF_FILE = "./configuration.json"

def main():
    configuration = json.load(open(CONF_FILE, 'r'))

    context = zmq.Context()

    # State Front-End
    statefe = context.socket(zmq.PULL)
    statefe.connect(configuration["ipc_filename"])

    system_id = statefe.recv()
    
    while True:
        # assuming 0 packet loss.
        pic1 = b''.join(statefe.recv_multipart())
        pic2 = b''.join(statefe.recv_multipart())


        asyncio.get_event_loop().run_until_complete(
            send_ws(configuration["home_address"], system_id, [pic1, pic2])
        )

    statefe.close()


async def send_ws(home_address ,system_id, images: [bytes]):
    #TODO make this connection persistent
    async with websockets.connect(home_address, max_size=None) as websocket:
        await websocket.send(jsoner(system_id, images))


def jsoner(system_id, images):
    # image to base64 conversion
    i2b64 = lambda image : "data:image/jpeg;base64," + base64.b64encode(image).decode("utf-8")

    return json.JSONEncoder().encode(
        {
            "id": system_id.decode("utf-8"), 
            "images": [ i2b64(image) for image in images]
        }
    )

if __name__ == '__main__':
    main()
# asyncio.get_event_loop().run_until_complete(main())