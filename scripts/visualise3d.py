import pandas as pd
import numpy as np
import plotly.graph_objects as plotly

df = pd.read_csv("toolpath.csv")

# get points
x = df["x"].values
y = df["y"].values
z = df["z"].values
feedrate = df["feedrate"].values

# calculations
# calculate delta (change in) points
dx = np.diff(x)
dy = np.diff(y)
dz = np.diff(z)
distances = np.sqrt(dx**2 + dy**2 + dz**2)
total_length = np.sum(distances)
avg_feedrate = np.mean(feedrate)

# create plot
toolpath_trace = plotly.Scatter3d(
    x=x, y=y, z=z,
    mode="lines",
    line=dict(
        color=feedrate,        # colour each point by feedrate value
        colorscale="Viridis",  # blue=slow, yellow=fast
        width=3,
        colorbar=dict(title="Feedrate (mm/min)")
    ),
    name="Toolpath",
    hovertemplate=(
        "X: %{x:.2f} mm<br>"
        "Y: %{y:.2f} mm<br>"
        "Z: %{z:.2f} mm<br>"
        "Feedrate: %{line.color:.0f} mm/min"
    )
)

# bounding box of the toolpath
x_min, x_max = np.min(x), np.max(x)
y_min, y_max = np.min(y), np.max(y)
z_min, z_max = np.min(z), np.max(z)

# width, height, and depth of the bounding box
width = x_max - x_min
height = y_max - y_min
depth = z_max - z_min

# speed at each interpolated point
time_ms = df["time_ms"].values
dt = np.diff(time_ms) / 1000.0  # convert ms to seconds

dt_safe = np.where(dt > 0, dt, np.nan)
speed = np.where(dt > 0, distances / dt_safe, 0)  # avoid division by zero

# summary
print(f"Total path length : {total_length:.2f} mm")
print(f"Bounding box      : {width:.1f} x {height:.1f} x {depth:.1f} mm")
print(f"Average feedrate  : {avg_feedrate:.1f} mm/min")
print(f"Max feedrate      : {np.max(feedrate):.1f} mm/min")
print(f"Min feedrate      : {np.min(feedrate):.1f} mm/min")
