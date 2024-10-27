# Stereo Camera
#### Video Demo:  https://youtu.be/ZavQvATgzgA
#### Description:
This projects is about the construction of a portable stereo camera with and ultrasonic sensor and also an IMU sensor.
The main porpose of this camera is to record videos and IMU data. It could be easyly mounted in any magenetic surfaces thanks to the magnets it has.
Live video stream of the camera will be available through a web interface. This web interface also have some controls about the paramentes of the camemra such the gain and time exposure.

It also have button to record and save videos of both cameras (left and right) and at the same time is going to be readinig the IMU sensor and will save all the IMU data into a CSV
with a timestamp colum.

## Overview
This project is a stereo camera system with integrated IMU (Inertial Measurement Unit) and ultrasonic sensors. Developed as an embedded application, the project captures real-time video from two cameras, gathers distance measurements from an ultrasonic sensor, and records acceleration and rotation data via the IMU. The goal is to provide a synchronized system for capturing detailed spatial and movement data for various applications, such as robotics, environmental mapping, and virtual reality.

This project was developed as the final project for the **CS50 Introduction to Hardware and Software course**.

## Features
- **Live Preview**: Real-time preview of both camera feeds through a web interface.
- **Real-Time Data Display**: Visualize live data from the IMU (acceleration and rotation) and the ultrasonic sensor (distance measurements).
- **Adjustable Camera Settings**: Manually adjust camera settings in real-time through the interface.
- **Data Recording**: Capture both video and IMU data with the option to save recordings for further analysis. Video data is stored as a video file, while IMU data is saved in CSV format.

## Components
- **Raspberry Pi**: The central processor managing data collection, sensor integration, and server functions.
- **Stereo Camera**: Two cameras (left and right) for depth perception and stereo imaging.
- **IMU (Inertial Measurement Unit)**: Captures data on acceleration and rotation for spatial tracking.
- **Ultrasonic Sensor**: Measures distance to objects, adding depth information to the stereo camera system.

## Project Structure:
   ```markdown
   .
   ├── static                    # Contains static assets (JavaScript, CSS)
   ├── templates                 # HTML templates for the user interface
   ├── test                      # For little experiments
   ├── app.py                    # Main application server file
   ├── CameraConfig.txt          # Configuration file for camera settings (not used)
   ├── requeriments.txt          # Python dependencies
   ├── treeManager.py            # Python functions for check and creating folders if not existing
   └── README.md

## Getting Started

### Prerequisites
- **Hardware**: Raspberry Pi, Stereo Camera, IMU (MPU6050), and Ultrasonic Sensor (HC-SR04).
- **Software**: Python 3.8+ installed on your Raspberry Pi.
- **Python Packages**: Required dependencies are listed in `requirements.txt`.

### Installation
1. **Clone the repository**:
   ```bash
   git clone <repository-url>
   cd project

2. **Install dependencies**:
   ```bash
   pip install -r requirements.txt

### Running the Application
1. **Start the Server**:
   ```bash
   python3 app.py

2. **Access the Web Interface**:
   Open a web browser and navigate to the Raspberry Pi's IP address and port (e.g., http://<raspberry-pi-ip>:5000).

3. **Using the Interface**:
   - View live camera feeds and real-time sensor data.
   - Adjust settings like brightness or contrast in real-time for optimal preview quality.
   - Press the Record button to start recording both video and IMU data. Video data will be stored in the videos folder, and IMU data will be saved in CSV format in the data folder.

### Recording and Accessing Data
   - Video Files: Saved in the videos folder.
   - IMU Data: Saved as CSV files in the data folder. These CSV files can be opened in data analysis software (e.g., Excel, Python) for further study.
