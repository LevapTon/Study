import numpy as np


def f(x):
    '''
    Расчет значения функции
    '''
    x1, x2 = x
    return x1**2 + 2*x2**2 + np.exp(x1**2 + x2**2) - x1 + 2*x2


def nelder_mead(f, x0, alpha=.9, beta=0.4, gamma=1.8, epsilon=1e-6):    
  n = len(x0)
  x = np.array(x0)
  points = [x]
  for i in range(1, n + 1):
    x_i = np.array(x)
    x_i[i - 1] += 1
    points.append(x_i)

  nit = 0  # Счетчик итераций

  while True:
    nit += 1
    points.sort(key=lambda x: f(x))
    x_l = points[0]
    x_g = points[-2]
    x_h = points[-1]

    x_c = sum(points[:-1]) / n
    f_c = f(x_c)

    x_r = (1 + alpha) * x_c - alpha * x_h
    f_r = f(x_r)

    if f_r < f(x_g):
      x_e = (1 - gamma) * x_c + gamma * x_r
      if f(x_e) < f_r:
        x_h = x_e
      else:
        x_h = x_r
    elif f_r >= f(x_h):
      x_k = (1 - beta) * x_c + beta * x_h
      if f(x_k) < f(x_h):
        x_h = x_k
      else:
        for i in range(1, n + 1):
          points[i] = (points[i] - x_l) / 2 + x_l
    else:
      x_h = x_r

    points[-1] = x_h
    sigma = np.sqrt(sum([(f(x_i) - f_c)**2 for x_i in points]) / (n + 1))
    if sigma < epsilon:
      break

  return x_h, f(x_h), nit


# Начальная точка
x0 = [2, 2]

# Вызов функции
x_min, f_min, nit = nelder_mead(f, x0)

# Вывод результатов
print("Минимум функции: %10.7f" % f_min)
print("Точка минимума:\nx1 = %10.7f \nx2 = %10.7f" % (x_min[0], x_min[1]))
print("Количество итераций:", nit)