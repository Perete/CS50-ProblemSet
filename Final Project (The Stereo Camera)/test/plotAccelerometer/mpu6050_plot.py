# GITHUB REPO FOR LIBRARY: https://github.com/m-rtijn/mpu6050
# GUIDE LINK: https://docs.sunfounder.com/projects/umsk/en/latest/05_raspberry_pi/pi_lesson05_mpu6050.html 

# Import the mpu6050 class and sleep function from respective modules.
from mpu6050 import mpu6050
from time import sleep

import datetime
import csv

import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Initialize the MPU-6050 sensor with the I2C address 0x68.
sensor = mpu6050(0x68)
sensor_data = []

# Variables to store the data for plotting
timestamps = []
accel_x_data = []
accel_y_data = []
accel_z_data = []

# Setup the plot
fig, ax = plt.subplots()
line1, = ax.plot([], [], label="Accel X")
line2, = ax.plot([], [], label="Accel Y")
line3, = ax.plot([], [], label="Accel Z")

# Formatting the plot
ax.set_xlim(0, 10)  # Time window to show in seconds
ax.set_ylim(-20, 20)  # Adjust according to expected values from accelerometer
plt.xlabel('Time (s)')
plt.ylabel('Acceleration (m/s²)')
plt.title('MPU6050 Accelerometer Data')
plt.legend()

# Function to initialize the plot
def init():
    line1.set_data([], [])
    line2.set_data([], [])
    line3.set_data([], [])
    return line1, line2, line3

# Function to update the plot in real-time
def update(frame):
    # Get the current timestamp
    current_time = datetime.datetime.now().timestamp()
    
    # Retrieve accelerometer data from the sensor
    accel_data = sensor.get_accel_data()
    
    # Update the lists with new data
    timestamps.append(current_time)
    accel_x_data.append(accel_data['x'])
    accel_y_data.append(accel_data['y'])
    accel_z_data.append(accel_data['z'])
    
    # Keep the time window to the last 10 seconds of data
    if len(timestamps) > 100:  # Assuming 10 Hz data rate (update every second)
        timestamps.pop(0)
        accel_x_data.pop(0)
        accel_y_data.pop(0)
        accel_z_data.pop(0)
    
    # Update the plot data
    line1.set_data(timestamps, accel_x_data)
    line2.set_data(timestamps, accel_y_data)
    line3.set_data(timestamps, accel_z_data)
    
    # Adjust x-axis to follow the data in real time
    ax.set_xlim(timestamps[0], timestamps[-1])
    
    return line1, line2, line3

# Run the animation, updating the plot every 1000ms (1 second)
ani = FuncAnimation(fig, update, init_func=init, blit=True, interval=100)

# Show the plot in real-time
plt.show()