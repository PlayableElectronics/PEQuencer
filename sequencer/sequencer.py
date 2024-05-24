from .channel import Channel
from .button import Button
from .encoder import Encoder
from .display import Display
import time


class Sequencer:
    def __init__(
        self,
        global_bpm: int = 120,
        channels: int = 8,
        buttons: int = 8,
        encoders: int = 1,
    ) -> None:
        self.global_bpm = global_bpm

        self.channels = [Channel(num + 1) for num in range(channels)]
        self.buttons = [Button(num + 1) for num in range(buttons)]
        self.encoders = [Encoder(num + 1) for num in range(encoders)]
        self.display = Display()
        self.display.run()  # mock

    def run(self) -> None:
        while True:
            self.wait(self.global_bpm)

    def wait(self, bpm) -> None:
        time.sleep(60 / bpm)
        pass
