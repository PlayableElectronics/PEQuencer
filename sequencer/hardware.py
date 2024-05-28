import time


class Hardware:
    """
    Asyncio handler hardware interface
    """

    def __init__(self, hardware_to_sequencer_queue, hardware_to_display_queue) -> None:
        self.hardware_to_sequencer_queue = hardware_to_sequencer_queue
        self.hardware_to_display_queue = hardware_to_display_queue

    def task_hardware(self) -> None:
        self.__run()

    def __run(self) -> None:
        while True:
            print("task hardware")
            """
            Handling hardware
            """
            # TODO: assign queues to Callbacks
            self.hardware_to_sequencer_queue.put(...)
            self.hardware_to_display_queue.put(...)
            time.sleep(1)  # Mock
