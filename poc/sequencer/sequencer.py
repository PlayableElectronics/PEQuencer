import threading
import time


class ThreadManager:
    def __init__(self):
        self.threads = []

    def start_threads(self):
        methods = [self.thread_task_1, self.thread_task_2, self.thread_task_3]
        for i, method in enumerate(methods):
            thread = threading.Thread(target=method)
            self.threads.append(thread)
            thread.start()

    def thread_task_1(self):
        time.sleep(2)  # symulacja jakiegoś zadania

    def thread_task_2(self):
        time.sleep(2)  # symulacja jakiegoś zadania

    def thread_task_3(self):
        time.sleep(2)  # symulacja jakiegoś zadania

    def wait_for_threads_to_finish(self):
        for thread in self.threads:
            thread.join()


# Przykładowe użycie klasy ThreadManager
if __name__ == "__main__":
    manager = ThreadManager()
    manager.start_threads()
    manager.wait_for_threads_to_finish()
    print("All threads have finished.")
