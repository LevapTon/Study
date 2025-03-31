import random
import threading
import queue

class Actor(threading.Thread):
    def __init__(self, actor_id, maze, start_position, exit_position, message_queue):
        super(Actor, self).__init__()
        self.actor_id = actor_id
        self.maze = maze
        self.position = start_position
        self.exit_position = exit_position
        self.message_queue = message_queue

    def run(self):
        while self.position != self.exit_position:
            self.move()

    def move(self):
        possible_moves = self.get_possible_moves()
        if possible_moves:
            move = random.choice(possible_moves)
            self.position = move
            self.message_queue.put((self.actor_id, move))

    def get_possible_moves(self):
        moves = []
        x, y = self.position
        if x > 0 and self.maze[y][x - 1] != '#':
            moves.append((x - 1, y))
        if x < len(self.maze[0]) - 1 and self.maze[y][x + 1] != '#':
            moves.append((x + 1, y))
        if y > 0 and self.maze[y - 1][x] != '#':
            moves.append((x, y - 1))
        if y < len(self.maze) - 1 and self.maze[y + 1][x] != '#':
            moves.append((x, y + 1))
        return moves

def main():
    maze = [
        "##########",
        "#S       #",
        "#  ##### #",
        "#    #   #",
        "###    ##",
        "#   ##   #",
        "# #### ##",
        "#   #    #",
        "#   ######",
        "#E       #",
        "##########"
    ]
    start_position = (1, 1)
    exit_position = (8, 9)

    num_actors = 1
    message_queue = queue.Queue()

    actors = [Actor(i, maze, start_position, exit_position, message_queue) for i in range(num_actors)]

    for actor in actors:
        actor.start()

    while True:
        actor_id, position = message_queue.get()
        print(f"Actor {actor_id} moved to position {position}")
        if position == exit_position:
            print(f"Actor {actor_id} reached the exit!")
            break

if __name__ == "__main__":
    main()
