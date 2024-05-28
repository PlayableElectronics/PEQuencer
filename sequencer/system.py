from .display import Display
from .hardware import Hardware
from .sequencer import Sequencer
import threading
from queue import Queue


class ThreadManager:
    def __init__(self) -> None:
        self.hardware_to_sequencer_queue = Queue()
        self.hardware_to_display_queue = Queue()
        self.sequencer_to_display_queue = Queue()
        self.tasks = [
            Sequencer(
                self.hardware_to_sequencer_queue, self.sequencer_to_display_queue
            ),
            Hardware(self.hardware_to_sequencer_queue, self.hardware_to_display_queue),
            Display(self.hardware_to_display_queue, self.sequencer_to_display_queue),
        ]
        self.threads = (
            threading.Thread(target=self.tasks[0].task_sequencer),
            threading.Thread(target=self.tasks[1].task_hardware),
            threading.Thread(target=self.tasks[2].task_display),
        )
        self.run()

    def run(self) -> None:
        for thread in self.threads:
            thread.start()


class PEQuencer:
    """
    Facade class of sequencer Program
    """

    def __init__(self) -> None:
        ThreadManager().run()
