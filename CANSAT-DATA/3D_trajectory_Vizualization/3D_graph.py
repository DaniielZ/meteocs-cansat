import numpy as np
import matplotlib.pyplot as plt
from sympy import symbols, Eq, solve, N
import math


def calculate_distance(x1, y1, z1, x2, y2, z2):
    distance = math.sqrt((x2 - x1)**2 + (y2 - y1)**2 + (z2 - z1)**2)
    return distance

def draw_line_intercept():
    i = 0
    for j in range(len(dist)):
        x, y, z = symbols('x y z')
        # create equations based on the distance formula
        eq1 = Eq((x - Xg[0]) ** 2 + (y - Yg[0]) ** 2 + (z - Zg[0]) ** 2, dist[j][0] ** 2)
        eq2 = Eq((x - Xg[1]) ** 2 + (y - Yg[1]) ** 2 + (z - Zg[1]) ** 2, dist[j][1] ** 2)
        eq3 = Eq((x - Xg[2]) ** 2 + (y - Yg[2]) ** 2 + (z - Zg[2]) ** 2, dist[j][2] ** 2)

        # solve the system of equations
        solutions = solve((eq1, eq2, eq3), (x, y, z))

        try:
            result_cords.append([N(solutions[0][0]), N(solutions[0][1]), N(solutions[0][2])])
            # ax.scatter(N(sol[0]), N(sol[1]), N(sol[2]), color=scatter_color, marker='o')
            # ax.text(N(sol[0]), N(sol[1]), N(sol[2]), str(i))

        except Exception as e:
            print(f"Point: {i} is unsolvable precisely")
        # print(result_cords)
    return
def show_midpoint(x1, y1, z1, x2, y2, z2):
    midpoint_x = (x1 + x2) / 2
    midpoint_y = (y1 + y2) / 2
    midpoint_z = (z1 + z2) / 2
    ax.text(midpoint_x, midpoint_y, midpoint_z,round(calculate_distance(x1, y1, z1, x2, y2, z2),1))
    ax.plot([x1, x2], [y1, y2], [z1, z2], color=scatter_color, linestyle=scatter_line_style)
    return

def draw_sphere(x,y,z,radius):
    center = (x, y, z)

    # Create a mesh grid to represent the sphere surface
    u = np.linspace(0, 2 * np.pi, 100)
    v = np.linspace(0, np.pi, 50)
    x = center[0] + radius * np.outer(np.cos(u), np.sin(v))
    y = center[1] + radius * np.outer(np.sin(u), np.sin(v))
    z = center[2] + radius * np.outer(np.ones(np.size(u)), np.cos(v))

    # Plot the transparent sphere
    ax.plot_surface(x, y, z, alpha=0.3)
    return

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
scatter_color = 'blue'
scatter_color2 = 'rebeccapurple'
scatter_line_style = '-'
triangle_color = 'red'
triangle_line_style = '--'
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_box_aspect([1, 1, 1])









# example 3 point cordinates
Xg = np.array([10, 40, 55])
Yg = np.array([30, 4, 40])
Zg = np.array([4, 3, 5])

# random vectors
num_vectors = 50
Xv = np.random.rand(num_vectors)*5
Yv = np.random.rand(num_vectors)*5
Zv = np.random.rand(num_vectors)*5

# Base station cords
triangle_X = np.array([0, 1, -1])
triangle_Y = np.array([0, 1, 1])
triangle_Z = np.array([0, 0, 0])

# all distance mesurements
dist = [[58,45,70],[52,47,70],[50,52,70],[50,60,72],[50,65,78],[55,62,70]]

# all resulting tracked points
result_cords = []

# Plot the data points
ax.scatter(Xg, Yg, Zg, color=scatter_color, marker='o')
ax.text(Xg[0], Yg[0], Zg[0], 'Uno')
ax.text(Xg[1], Yg[1], Zg[1], 'Dos')
ax.text(Xg[2], Yg[2], Zg[2], 'Tres')
show_midpoint(Xg[0], Yg[0], Zg[0],Xg[1], Yg[1], Zg[1])
show_midpoint(Xg[2], Yg[2], Zg[2],Xg[1], Yg[1], Zg[1])
show_midpoint(Xg[0], Yg[0], Zg[0],Xg[2], Yg[2], Zg[2])

# draw_sphere(Xg[0], Yg[0], Zg[0],dist[0])
# draw_sphere(Xg[1], Yg[1], Zg[1],dist[1])
# draw_sphere(Xg[2], Yg[2], Zg[2],dist[2])

draw_line_intercept()

# grpah points
for values in result_cords:
    ax.scatter(values[0], values[1], values[2], color=scatter_color2, marker='o')

# graph lines betwheen points
X = [point[0] for point in result_cords]
Y = [point[1] for point in result_cords]
Z = [point[2] for point in result_cords]
ax.plot(X, Y, Z, color=scatter_color2, linestyle='-', marker='o')

max_range = np.max(np.abs(np.concatenate([Xg, Yg])))
ax.set_xlim([0, max_range])
ax.set_ylim([0, max_range])
ax.set_zlim([0, max_range])
plt.subplots_adjust(top=1, bottom=0, left=0, right=1)

plt.show()