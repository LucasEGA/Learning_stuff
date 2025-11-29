import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider

# --- logistic map function ----------------------------------------------------
def logistic_orbit(r: float, x0: float, n_iter: int) -> np.ndarray:
    """
    Compute orbit of the logistic map:
        x_{n+1} = r * x_n * (1 - x_n)
    """
    x = np.empty(n_iter, dtype=float)
    x[0] = x0
    for i in range(1, n_iter):
        x[i] = r * x[i - 1] * (1.0 - x[i - 1])
    return x


# --- initial parameters -------------------------------------------------------
r0 = 3.5       # initial r (you can change here too)
x0 = 0.2       # initial x0
n_iter = 200   # number of iterations

n = np.arange(n_iter)
x = logistic_orbit(r0, x0, n_iter)

# --- create plot --------------------------------------------------------------
fig, ax = plt.subplots()
plt.subplots_adjust(bottom=0.25)  # make room for slider

(line,) = ax.plot(n, x, "-")
ax.set_xlabel("n")
ax.set_ylabel("x_n")
ax.set_ylim(0.0, 1.0)
ax.set_title(f"Logistic map orbit (r = {r0:.3f})")

# --- slider for r -------------------------------------------------------------
# [left, bottom, width, height] in figure coordinates (0..1)
ax_r = plt.axes([0.15, 0.1, 0.7, 0.03])
slider_r = Slider(
    ax=ax_r,
    label="r",
    valmin=1.0,
    valmax=4.0,
    valinit=r0,
    valstep=0.001,  # smooth slider
)


def update(val):
    r = slider_r.val
    x_new = logistic_orbit(r, x0, n_iter)
    line.set_ydata(x_new)
    ax.set_title(f"Logistic map orbit (r = {r:.3f})")
    fig.canvas.draw_idle()


slider_r.on_changed(update)

plt.show()
