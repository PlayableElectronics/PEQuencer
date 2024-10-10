import asyncio

class Tracks():
    def __init__(self):
        self.tracks = [
            {"id": "T1", "head": 0, "sleep": 0.2, "seq": [
                0x00, 0x01, 0x02, 0x03, 0x04
            ]},
            {"id": "T2", "head": 0, "sleep": 0.1, "seq": [
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05
            ]},
            {"id": "T3", "head": 0, "sleep": 0.3, "seq": [
                0x00, 0x01
            ]},
            {"id": "T4", "head": 0, "sleep": 0.05, "seq": [
                0x0e, 0x01, 0x0c, 0x0f, 0x0c
            ]},
        ]
        self.var = {"screen": 2}

    async def handle_track(self, track):
        while True:
            track["head"] = (track["head"] + 1) % len(track["seq"])
            await asyncio.sleep(track["sleep"])
