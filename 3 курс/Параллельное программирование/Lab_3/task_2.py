import tkinter as tk
import time

class Ball:
    def __init__(self, canvas, x, y, dx, dy):
        self.canvas = canvas
        self.x = x
        self.y = y
        self.dx = dx
        self.dy = dy
        self.radius = 10
        self.id = self.canvas.create_oval(x - self.radius, y - self.radius, x + self.radius, y + self.radius, fill="black")

    def move(self):
        self.x += self.dx
        self.y += self.dy
        self.canvas.move(self.id, self.dx, self.dy)

    def collide_with_wall(self, width, height):
        if self.x <= self.radius or self.x >= width - self.radius:
            self.dx *= -1
        if self.y <= self.radius or self.y >= height - self.radius:
            self.dy *= -1

    def collide_with_other_ball(self, other_ball):
        self.dx *= -1
        other_ball.dx *= -1
        self.dy *= -1
        other_ball.dy *= -1


class BilliardsTable(tk.Tk):
    def __init__(self, width, height):
        super().__init__()
        self.title("Прямоугольный бильярд")
        self.canvas = tk.Canvas(self, width=width, height=height, bg="green")
        self.canvas.pack()
        self.width = width
        self.height = height
        self.balls = []

    def add_ball(self, ball):
        self.balls.append(ball)

    def move_balls(self):
        for ball in self.balls:
            ball.move()
            ball.collide_with_wall(self.width, self.height)
            for other_ball in self.balls:
                if ball != other_ball:
                    distance = ((ball.x - other_ball.x) ** 2 + (ball.y - other_ball.y) ** 2) ** 0.5
                    if distance <= ball.radius * 2:
                        ball.collide_with_other_ball(other_ball)

    def animate(self):
        while True:
            self.move_balls()
            self.update()
            time.sleep(0.01)


if __name__ == "__main__":
    table_width = 600
    table_height = 400
    table = BilliardsTable(table_width, table_height)

    ball1 = Ball(table.canvas, 50, 50, 2, 3)
    ball2 = Ball(table.canvas, 200, 200, -3, -2)
    ball3 = Ball(table.canvas, 400, 100, 1, -2)

    table.add_ball(ball1)
    table.add_ball(ball2)
    table.add_ball(ball3)

    table.animate()
