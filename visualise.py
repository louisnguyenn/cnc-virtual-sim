import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("toolpath.csv")

# print(df.head())    # print the first few rows

# create figure and axes
fig, ax = plt.subplots(figsize=(8,8))

# plot x and y as a line
ax.plot(df["x"], df["y"], lw=1.0, color="steelblue", label="Toolpath")

# add green dot at first position
ax.scatter(df["x"].iloc[0], df["y"].iloc[0], color="green", s=80, zorder=5, label="Start")

# add red dot at final position
ax.scatter(df["x"].iloc[-1], df["y"].iloc[-1], color="red", s=80, zorder=5, label="End")

# labels
ax.set_xlabel("X (mm)")
ax.set_ylabel("Y (mm)")
ax.set_title("CNC Toolpath - XY Plane")
ax.legend()

plt.tight_layout()

plt.savefig("toolpath.png", dpi=150)
print("Saved toolpath.png")
