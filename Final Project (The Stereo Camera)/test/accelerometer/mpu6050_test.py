# GITHUB REPO FOR LIBRARY: https://github.com/m-rtijn/mpu6050
# GUIDE LINK: https://docs.sunfounder.com/projects/umsk/en/latest/05_raspberry_pi/pi_lesson05_mpu6050.html 

# Import the mpu6050 class and sleep function from respective modules.
from mpu6050 import mpu6050
from time import sleep

import datetime
import csv

# Initialize the MPU-6050 sensor with the I2C address 0x68.
sensor = mpu6050(0x68)
# sensor_data = {"TimeStamp":0, "accel_x":0, "accel_y":0, "accel_z":0, "gyro_x":0, "gyro_y":0, "gyro_z":0}
sensor_data = []
sensor_row = {}
count = 0
# Infinite loop to continuously read data from the sensor. 
while count < 5:
    sensor_row['Timestamp'] = datetime.datetime.now().timestamp()
    
    # Retrieve accelerometer data from the sensor.
    accel_data = sensor.get_accel_data()
    
    # Retrieve gyroscope data from the sensor.
    gyro_data = sensor.get_gyro_data()
    
    # Retrieve temperature data from the sensor.
    # temp = sensor.get_temp()

    sensor_row['accel_x'] = accel_data['x']
    sensor_row['accel_y'] = accel_data['y']
    sensor_row['accel_z'] = accel_data['z']

    sensor_row['gyro_x'] = gyro_data['x']
    sensor_row['gyro_y'] = gyro_data['y']
    sensor_row['gyro_z'] = gyro_data['z']

    sensor_data.append(sensor_row.copy())
    # print(sensor_data)

    # Print the temperature in Celsius.
    # print("Temp: " + str(temp) + " C\n")

    # Pause for 0.5 seconds before the next read cycle.
    count += 1
    sleep(1)

print(sensor_data)

with open("imuData.csv", "w") as file:
    fieldnames = sensor_data[0].keys()
    writer = csv.DictWriter(file, fieldnames=fieldnames)
    writer.writeheader()

    for row in sensor_data:
        writer.writerow(row)
