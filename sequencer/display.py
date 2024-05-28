import time
from queue import Empty


class Display:
    def __init__(
        self,
        hardware_to_display_queue,
        sequencer_to_display_queue,
        width: int = 128,
        height: int = 64,
        refresh_rate: int = 60,
    ) -> None:
        self.hardware_to_display_queue = hardware_to_display_queue
        self.sequencer_to_display_queue = sequencer_to_display_queue
        self.width = width
        self.height = height
        self.refresh_rate = refresh_rate
        ...  # TODO: Create here local table with local state of menus etc

    def task_display(self) -> None:
        self.__run()

    def __run(self) -> None:
        while True:
            print("task display")
            self.__get_data_from_sequencer()
            self.__get_data_from_hardware()
            # TODO: Update local table with queue get
            ...
            time.sleep(1 / self.refresh_rate)

    def __get_data_from_sequencer(self) -> ...:
        try:
            print(self.sequencer_to_display_queue.get_nowait())
        except Empty:
            pass

    def __get_data_from_hardware(self) -> ...:
        try:
            print(self.hardware_to_display_queue.get_nowait())
        except Empty:
            pass
