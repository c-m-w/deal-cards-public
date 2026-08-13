import serial
import re
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from collections import defaultdict, deque
import math

# ============== CONFIG ==============
PORT = "COM7"
BAUD = 115200
MAX_POINTS = 1000
# ====================================

# Regex: >plot_name:line_name(x,y)
PATTERN = re.compile(
    r">\s*([a-zA-Z0-9_]+)\s*:\s*([a-zA-Z0-9_]+)\s*\(\s*([-+]?\d*\.?\d+)\s*,\s*([-+]?\d*\.?\d+)\s*\)"
)

ser = serial.Serial(PORT, BAUD, timeout=0.05)

# data[plot_name][line_name] = {"x": deque, "y": deque}
data = defaultdict(lambda: defaultdict(lambda: {"x": deque(maxlen=MAX_POINTS), "y": deque(maxlen=MAX_POINTS)}))
lines = defaultdict(dict)          # lines[plot_name][line_name] = Line2D
axes = {}                          # axes[plot_name] = Axes

fig = plt.figure(figsize=(10, 6))
plt.tight_layout()

def rebuild_layout():
    """Recreate the subplot layout based on current plots."""
    fig.clf()
    axes.clear()

    plot_names = list(data.keys())
    n = len(plot_names)

    if n == 0:
        return

    # Simple vertical stacking
    for i, name in enumerate(plot_names):
        ax = fig.add_subplot(n, 1, i + 1)
        axes[name] = ax
        ax.set_title(name)
        ax.grid(True)
        if i == n - 1:
            ax.set_xlabel("X")
        ax.set_ylabel("Y")

        # Recreate line objects
        for line_name in data[name]:
            line, = ax.plot([], [], label=line_name, linewidth=1.5)
            lines[name][line_name] = line

        if data[name]:
            ax.legend(loc="upper left")

    fig.tight_layout()

def clear_all():
    """Remove all plots and data."""
    data.clear()
    lines.clear()
    axes.clear()
    fig.clf()
    fig.canvas.draw_idle()
    print("[All plots cleared]")

def update(frame):
    need_rebuild = False
    import time
    time.sleep(0.001)

    while ser.in_waiting:
        try:
            raw = ser.readline().decode("utf-8", errors="ignore").strip()
            if not raw:
                continue

            if raw == ">clear":
                clear_all()
                continue

            match = PATTERN.search(raw)
            if match:
                plot_name = match.group(1)
                line_name = match.group(2)
                x = float(match.group(3))
                y = float(match.group(4))

                # New plot?
                if plot_name not in data:
                    need_rebuild = True

                data[plot_name][line_name]["x"].append(x)
                data[plot_name][line_name]["y"].append(y)

                # New line on an existing plot?
                if plot_name in axes and line_name not in lines[plot_name]:
                    ax = axes[plot_name]
                    line, = ax.plot([], [], label=line_name, linewidth=1.5)
                    lines[plot_name][line_name] = line
                    ax.legend(loc="upper left")

            print(raw)

        except Exception as e:
            print(f"[Error] {e}")

    if need_rebuild:
        rebuild_layout()

    # Update all existing lines
    for plot_name, line_dict in lines.items():
        if plot_name not in axes:
            continue
        ax = axes[plot_name]
        for line_name, line in line_dict.items():
            line.set_data(data[plot_name][line_name]["x"],
                          data[plot_name][line_name]["y"])
        ax.relim()
        ax.autoscale_view()

    return []

ani = FuncAnimation(fig, update, interval=30, blit=False, cache_frame_data=False)
plt.show()