from .channel import Channel
import time
from queue import Empty


class Sequencer:
    """
    Sequencer interface
    """

    def __init__(
        self,
        hardware_to_sequencer_queue,
        sequencer_to_display_queue,
        bpm: int = 120,
        num_of_channels: int = 8,
    ) -> None:
        self.hardware_to_sequencer_queue = hardware_to_sequencer_queue
        self.sequencer_to_display_queue = sequencer_to_display_queue
        self.bpm = bpm
        self.channels = [Channel(channel + 1) for channel in range(num_of_channels)]
        [print(channel) for channel in self.channels]

    def task_sequencer(self) -> None:
        self.__run()

    def __run(self) -> None:
        while True:
            print("task sequencer")
            self.__get_data_from_hardware()
            # Sequencer logic implementation
            self.sequencer_to_display_queue.put(...)
            ...
            time.sleep(60 / self.bpm)

    def __get_data_from_hardware(self) -> ...:
        try:
            print(self.hardware_to_sequencer_queue.get_nowait())
        except Empty:
            pass
