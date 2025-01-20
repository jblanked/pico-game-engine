import _thread
from time import sleep


class EasyThread:
    def __init__(self, target, args=(), kwargs=None, lock_thread=False):
        if kwargs is None:
            kwargs = {}
        self.target = target
        self.args = list(args)
        self.kwargs = kwargs
        self.thread = None
        self.lock_alloc = _thread.allocate_lock()
        self.lock_thread = lock_thread
        self.should_stop = False  # Flag to signal the thread to stop

    def start(self):
        self.should_stop = False  # Reset the stop flag
        self.thread = _thread.start_new_thread(self.run, ())

    def run(self):
        try:
            # Keep calling target until a stop is signaled
            while not self.should_stop:
                self.target(*self.args, **self.kwargs)
                # sleep a bit to avoid hogging CPU if target returns quickly:
                sleep(0.01)
        except Exception as e:
            print(f"Thread encountered an error: {e}")
        finally:
            self.thread = None  # Mark thread as finished
            _thread.exit()

    def end(self):
        self.should_stop = True  # Signal the thread to stop

    def join(self):
        # Busy-wait until the thread is finished.
        while self.is_alive():
            sleep(0.01)

    def is_alive(self):
        return self.thread is not None

    def __del__(self):
        self.end()

    def __enter__(self):
        if self.lock_thread:
            self.lock_alloc.acquire()
        self.start()
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.end()
        self.join()
        if self.lock_thread:
            self.lock_alloc.release()
