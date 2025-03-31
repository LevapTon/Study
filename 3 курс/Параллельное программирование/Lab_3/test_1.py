import time

class Ball:
    def __init__(self, x, y, dx, dy):
        self.x = x
        self.y = y
        self.dx = dx
        self.dy = dy
        self.radius = 10

    def move(self):
        self.x += self.dx
        self.y += self.dy

    def collide_with_wall(self, width, height):
        collided = False
        if self.x <= self.radius or self.x >= width - self.radius:
            self.dx *= -1
            collided = True
        if self.y <= self.radius or self.y >= height - self.radius:
            self.dy *= -1
            collided = True
        return collided

    def collide_with_other_ball(self, other_ball):
        self.dx *= -1
        other_ball.dx *= -1
        self.dy *= -1
        other_ball.dy *= -1


class BilliardsTable:
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.balls = []

    def add_ball(self, ball):
        self.balls.append(ball)

    def move_balls(self):
        for ball in self.balls:
            ball.move()
            collided = ball.collide_with_wall(self.width, self.height)
            if collided:
                print(f"Шар столкнулся со стеной: Шар в ({ball.x}, {ball.y})")
            for other_ball in self.balls:
                if ball != other_ball:
                    distance = ((ball.x - other_ball.x) ** 2 + (ball.y - other_ball.y) ** 2) ** 0.5
                    if distance <= ball.radius * 2:
                        ball.collide_with_other_ball(other_ball)
                        print(f"Шары столкнулись: Шар в ({ball.x}, {ball.y}) и Шар в ({other_ball.x}, {other_ball.y})")

    def print_ball_info(self):
        print("Шар\tX\tY\tDX\tDY")
        for i, ball in enumerate(self.balls):
            print(f"{i+1}\t{ball.x}\t{ball.y}\t{ball.dx}\t{ball.dy}")

    def animate(self, iterations):
        print("Начальные точки:")
        self.print_ball_info()
        print()

        for i in range(iterations):
            self.move_balls()
            print(f"Итерация {i+1}:")
            self.print_ball_info()
            print()

            time.sleep(0.01)

if __name__ == "__main__":
    table_width = 100
    table_height = 60
    table = BilliardsTable(table_width, table_height)

    ball1 = Ball(5, 5, 2, 3)
    ball2 = Ball(20, 20, -3, -2)
    ball3 = Ball(30, 10, 1, -2)

    table.add_ball(ball1)
    table.add_ball(ball2)
    table.add_ball(ball3)

    table.animate(20)
