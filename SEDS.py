import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


# ============================================================
# 1. READ THE DATA
# ============================================================

file_name = "Depth Data.csv"

df = pd.read_csv(file_name)

# Display the first few rows
print("Original data:")
print(df.head())


# ============================================================
# 2. EXTRACT DEPTH DATA
# ============================================================

# Convert the Depth column to numbers.
# Invalid/corrupted values such as "#VALUE!" become NaN.
df["Depth (m)"] = pd.to_numeric(df["Depth (m)"], errors="coerce")

# Time is in seconds because the problem states that
# the sensor recorded one reading every second.
df["Time (s)"] = df["Point"] - 1


# ============================================================
# 3. HANDLE CORRUPTED / ERRATIC DATA
# ============================================================

# Calculate a rolling median using nearby readings.
rolling_median = df["Depth (m)"].rolling(
    window=5,
    center=True,
    min_periods=1
).median()

# Calculate the difference between each reading
# and its local median.
difference = abs(df["Depth (m)"] - rolling_median)

# A reading is considered an extreme outlier if it differs
# greatly from its local median.
#
# The threshold can be adjusted depending on the sensor data.
OUTLIER_THRESHOLD = 500

outliers = difference > OUTLIER_THRESHOLD

# Replace detected outliers with NaN.
df.loc[outliers, "Depth (m)"] = None

print("\nNumber of corrupted/outlier readings removed:",
      outliers.sum())


# ============================================================
# 4. FILL MISSING VALUES
# ============================================================

# Interpolate missing readings using surrounding values.
df["Depth Cleaned (m)"] = df["Depth (m)"].interpolate()

# Handle missing values at the beginning/end if there are any.
df["Depth Cleaned (m)"] = (
    df["Depth Cleaned (m)"]
    .bfill()
    .ffill()
)


# ============================================================
# 5. REDUCE RANDOM SENSOR NOISE
# ============================================================

# Moving average.
# Each point is averaged with nearby readings.
df["Depth Smoothed (m)"] = (
    df["Depth Cleaned (m)"]
    .rolling(window=5, center=True, min_periods=1)
    .mean()
)


# ============================================================
# 6. DISPLAY THE PROCESSED DATA
# ============================================================

print("\nProcessed data:")
print(
    df[[
        "Time (s)",
        "Depth (m)",
        "Depth Cleaned (m)",
        "Depth Smoothed (m)"
    ]].head(20)
)


# ============================================================
# 7. STATIC GRAPH
# ============================================================

plt.figure(figsize=(12, 6))

plt.plot(
    df["Time (s)"],
    df["Depth (m)"],
    label="Raw Sensor Data",
    alpha=0.5
)

plt.plot(
    df["Time (s)"],
    df["Depth Smoothed (m)"],
    label="Smoothed Data",
    linewidth=2
)

plt.xlabel("Time (seconds)")
plt.ylabel("Depth (m)")
plt.title("Ship Depth vs Time")

plt.legend()
plt.grid(True)

plt.tight_layout()
plt.show()


# ============================================================
# 8. ANIMATED GRAPH
# ============================================================

fig, ax = plt.subplots(figsize=(12, 6))

ax.set_title("Real-Time Ship Depth Monitoring")
ax.set_xlabel("Time (seconds)")
ax.set_ylabel("Depth (m)")
ax.grid(True)

# Set graph limits before animation starts.
ax.set_xlim(
    df["Time (s)"].min(),
    df["Time (s)"].max()
)

ax.set_ylim(
    df["Depth Smoothed (m)"].min() - 10,
    df["Depth Smoothed (m)"].max() + 10
)

# Line that will be updated during animation
line, = ax.plot([], [], linewidth=2, label="Smoothed Depth")

ax.legend()


# ------------------------------------------------------------
# Animation initialization
# ------------------------------------------------------------

def init():
    line.set_data([], [])
    return line,


# ------------------------------------------------------------
# Animation update
# ------------------------------------------------------------

def update(frame):

    # Display data from the beginning up to the current point.
    time_data = df["Time (s)"].iloc[:frame + 1]

    depth_data = df["Depth Smoothed (m)"].iloc[:frame + 1]

    line.set_data(time_data, depth_data)

    return line,


# ------------------------------------------------------------
# Create animation
# ------------------------------------------------------------

animation = FuncAnimation(
    fig,
    update,
    frames=len(df),
    init_func=init,
    interval=1000,       # 1000 ms = 1 second
    blit=True,
    repeat=False
)

plt.tight_layout()
plt.show()