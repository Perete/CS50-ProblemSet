# IMPORTS FOR FLASK SERVER AND CAMERA
from flask import Flask, render_template, Response, request
from picamera2 import Picamera2
from picamera2.encoders import H264Encoder, Quality
from picamera2.outputs import FileOutput
import libcamera
import cv2

import os
import sys
import datetime
import treeManager

# IMPORTS NEEDED FOR IMU
from mpu6050 import mpu6050
from time import sleep
import threading
import csv

app = Flask(__name__)

# READ ARGV  
flip = 0
if len(sys.argv) < 2:
    print("No argv detected.")
else:
    flip = int(sys.argv[1])
    print(f"Flip configuration: {flip}") 


gain = 1
exposure_time = 1000
THIS_PATH = os.path.dirname(__file__)

# CHECK AND READ FILE CONFIGURATION (NOT USED AT ALL)
# configValues = treeManager.checkCameraConfigFile(f"{THIS_PATH}/cameraConfig.txt")

# CHECK IF FOLDERS EXISTS IN TREE
treeManager.checkFolder(f'{THIS_PATH}/videoOutput2Cameras')
treeManager.checkFolder(f'{THIS_PATH}/imuData')

# CAMERA CONFIG
res_H = 960
res_W = 540
cam1 = Picamera2(1)
cam2 = Picamera2(0)

video_config1 = cam1.create_video_configuration(main={"size": (res_H, res_W)})
video_config2 = cam2.create_video_configuration(main={"size": (res_H, res_W)})

# FLIP THE OUTPUT IMAGE (JUST IN CASE CAMERA IS MOUNTED IN REVERSE)
if flip:
    video_config1["transform"] = libcamera.Transform(hflip=flip, vflip=1)
    video_config2["transform"] = libcamera.Transform(hflip=flip, vflip=1)


cam1.configure(video_config1)
cam2.configure(video_config2)

encoder1 = H264Encoder()
encoder2 = H264Encoder()

cam1.start()
cam2.start()

# GET INITIAL CONFIG VALUES OF CAMERA
metadata1 = cam1.capture_metadata()
controls1 = {c: metadata1[c] for c in ["ExposureTime", "AnalogueGain"]}
metadata2 = cam2.capture_metadata()
controls2 = {c: metadata2[c] for c in ["ExposureTime", "AnalogueGain"]}
print(f"Camera1: {controls1}")
print(f"Camera2: {controls2}")

# INITIALIZE IMU
sensor = mpu6050(0x68)

sensor_data = []
sensor_row = {}
stopThread = False
now = None

# FUNCTION FOR READ IMU
def read_accelerometer():
    global stopThread, sensor_row, sensor_data, now
    
    while not stopThread:
        # Save timestamp time
        sensor_row['timestamp'] = datetime.datetime.now().timestamp()

        # Retrieve accelerometer data from the sensor (m/s^2).
        accel_data = sensor.get_accel_data()
        
        # Retrieve gyroscope data from the sensor (º/sec).
        gyro_data = sensor.get_gyro_data()

        # Save data into dict 
        sensor_row['accel_x'] = accel_data['x']
        sensor_row['accel_y'] = accel_data['y']
        sensor_row['accel_z'] = accel_data['z']

        sensor_row['gyro_x'] = gyro_data['x']
        sensor_row['gyro_y'] = gyro_data['y']
        sensor_row['gyro_z'] = gyro_data['z']

        # Copy dict row into list 
        sensor_data.append(sensor_row.copy())
        
        # 0.1 seconds --> 10 Hz. Sensor up to 1kHz --> 0.001 seconds
        sleep(0.001)
    
    # Save data into csv file with timeStampData
    with open(f"imuData/mpu6050_{now}.csv", "w") as file:
        fieldnames = sensor_data[0].keys()
        writer = csv.DictWriter(file, fieldnames=fieldnames)
        writer.writeheader()

        for row in sensor_data:
            writer.writerow(row)

# GENERATE FRAMES FOR THE FLASK SERVER
def gen_frames(camera):
    while True:
        frame = camera.capture_array()
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        ret, buffer = cv2.imencode('.jpg', frame)
        frame = buffer.tobytes()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')
    

@app.route('/video_feed/<int:cam_id>')
def video_feed(cam_id):
    if cam_id == 1:       
        return Response(gen_frames(cam1), mimetype='multipart/x-mixed-replace; boundary=frame')
    else:
        return Response(gen_frames(cam2), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/get_camera_params')
def get_camera_params():
        global controls1, controls2
        # Obtener los valores actuales de los controles
        metadata1 = cam1.capture_metadata()
        controls1 = {c: metadata1[c] for c in ["ExposureTime", "AnalogueGain"]}

        metadata2 = cam2.capture_metadata()
        controls2 = {c: metadata2[c] for c in ["ExposureTime", "AnalogueGain"]}

        return f"{controls1['ExposureTime']},{controls1['AnalogueGain']},{controls2['ExposureTime']},{controls2['AnalogueGain']}"

# GLOBAL VARIABLES FOR HANDLE RECORDING
is_recording = False
output_file = None
video_writer = None
accelerometer_thread = None

# START RECORDING CAMERAS
# @app.route('/start_recording/<int:cam_id>', methods=['POST'])
@app.route('/start_recording', methods=['POST'])
# def start_recording(cam_id):
def start_recording():
    
    global is_recording, stopThread, now, gain, exposure_time, video_writer, controls1, accelerometer_thread

    if not is_recording:
        
        is_recording = True
        stopThread = False
    
        now = datetime.datetime.now().timestamp()
        accelerometer_thread = threading.Thread(target=read_accelerometer, daemon=True)
        accelerometer_thread.start()
        print(f"Starting recording and IMU: {now}")

        # ENCODER PICAMERA
        # # encoder.output = FfmpegOutput('video_cam1.h264')
        cam1.start_encoder(encoder1, quality=Quality.HIGH)
        cam2.start_encoder(encoder2, quality=Quality.HIGH)

        # FLIP CAMERA IMAGE
        if flip:
            encoder1.output = FileOutput(f'{THIS_PATH}/videoOutput2Cameras/{now}_{exposure_time}_{gain}_VIDEO_L.h264')
            encoder2.output = FileOutput(f'{THIS_PATH}/videoOutput2Cameras/{now}_{exposure_time}_{gain}_VIDEO_R.h264')
        else:
            encoder1.output = FileOutput(f'{THIS_PATH}/videoOutput2Cameras/{now}_{exposure_time}_{gain}_VIDEO_R.h264')
            encoder2.output = FileOutput(f'{THIS_PATH}/videoOutput2Cameras/{now}_{exposure_time}_{gain}_VIDEO_L.h264')
    
        encoder1.output.start()
        encoder2.output.start()

    else:
        print("Cameras alredy recording")

    return '', 204  # Returns an empty response with a code 204 (no content)

# STOP RECORDING CAMERAS
# @app.route('/stop_recording/<int:cam_id>', methods=['POST'])
@app.route('/stop_recording', methods=['POST'])
# def stop_recording(cam_id):
def stop_recording():
    
    global is_recording, video_writer, stopThread
    
    if is_recording:
        print("STOP RECORDING AND IMU")
        is_recording = False
        stopThread = True

        # ENCODER PICAMERA
        encoder1.output.stop()
        encoder2.output.stop()
        cam1.stop_encoder(encoder1)
        cam2.stop_encoder(encoder2)

    else:
        print("Cameras are not recording!")

    return '', 204

# SET CAMERA PARAMETERS
# @app.route('/set_params/<int:cam_id', methods=['POST'])
@app.route('/set_params', methods=['POST'])
# def set_params(cam_id):
def set_params():
    global gain, exposure_time
    gain = float(request.form['gain'])
    exposure_time = int(request.form['exposure_time'])

    cam1.set_controls({"AnalogueGain": gain, "ExposureTime": exposure_time})
    cam2.set_controls({"AnalogueGain": gain, "ExposureTime": exposure_time})

    return '', 204

# STOP AND EXIT SERVER 
@app.route('/stop_server', methods=['POST'])
def stop_server():
    stop_recording()
    sleep(1)
    print("parando servidor")
    os._exit(0)

# AUTO CAMERA PARAMETERS SETTINGS
@app.route('/auto_adjust', methods=['POST'])
def auto_adjust():
    print("Auto Ajuste")
    cam1.set_controls({"AeEnable": True, "AwbEnable": True})
    cam2.set_controls({"AeEnable": True, "AwbEnable": True})
    return '', 204

@app.route('/')
def index():
    return render_template('index.html', flip = flip)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=False)
