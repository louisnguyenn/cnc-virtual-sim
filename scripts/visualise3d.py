import pandas as pd
import numpy as np

df = pd.read_csv("toolpath.csv")

# print(df)

#
# calculate total path length
#

# get points
x = np.array(df["x"].values)
y = np.array(df["y"].values)
z = np.array(df["z"].values)

# print(x)

# calculate delta (change in) points
dx = np.diff(x);
dy = np.diff(y);
dz = np.diff(z);

distances = np.sqrt(dx**2 + dy**2 + dz**2)

total_length = np.sum(distances)

# print(total_length)

#
# calculate average feed rate
#

feedrate = np.array(df["feedrate"].values)

avg_feedrate = np.mean(feedrate)

# print(avg_feedrate)

#
# bounding box of the toolpath
#

x_min, x_max = np.min(x), np.max(x)
y_min, y_max = np.min(y), np.max(y)
z_min, z_max = np.min(z), np.max(z)

# width, height, and depth of the bounding box
width = x_max - x_min
height = y_max - y_min
depth = z_max - z_min

#
# speed at each point
#

time_ms = df["time_ms"].values
dt = np.diff(time_ms) / 1000.0  # convert ms to seconds

speed = np.where(dt > 0, distances / dt, 0)  # avoid division by zero

# summary
print(f"Total path length : {total_length:.2f} mm")
print(f"Bounding box      : {width:.1f} x {depth:.1f} x {height:.1f} mm")
print(f"Average feedrate  : {np.mean(feedrate):.1f} mm/min")
print(f"Max feedrate      : {np.max(feedrate):.1f} mm/min")
print(f"Min feedrate      : {np.min(feedrate):.1f} mm/min")
