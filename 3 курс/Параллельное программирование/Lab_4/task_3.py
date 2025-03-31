import threading
import time

class Philosopher(threading.Thread):
    running = True

    def __init__(self, name, left_fork, right_fork):
        threading.Thread.__init__(self)
        self.name = name
        self.left_fork = left_fork
        self.right_fork = right_fork

    def run(self):
        while self.running:
            self.think()
            self.eat()

    def think(self):
        print(f"{self.name} is thinking.")
        time.sleep(1)

    def eat(self):
        fork1, fork2 = self.left_fork, self.right_fork
        while True:
            fork1.acquire()
            locked = fork2.acquire(False)
            if locked:
                break
            fork1.release()
            print(f"{self.name} swaps forks")
            fork1, fork2 = fork2, fork1
        else:
            return

        print(f"{self.name} is eating.")
        time.sleep(1)
        fork2.release()
        fork1.release()

def main():
    num_philosophers = 5

    forks = [threading.Lock() for _ in range(num_philosophers)]

    philosophers = [Philosopher(f"Philosopher {i}", forks[i], forks[(i+1) % num_philosophers]) for i in range(num_philosophers)]

    Philosopher.running = True
    for philosopher in philosophers:
        philosopher.start()

    time.sleep(10)

    Philosopher.running = False
    print("Now stopping the philosophers...")

    for philosopher in philosophers:
        philosopher.join()

if __name__ == "__main__":
    main()
