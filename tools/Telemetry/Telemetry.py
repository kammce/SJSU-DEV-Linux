from __future__ import division
from flask import Flask, request, render_template, jsonify, send_from_directory
import threading
import serial
import time
import glob
import json
import re
import os

# SETUP FLASK APPLICATION
app = Flask(__name__)
app.debug = True

# SETUP SERIAL PORT
ser = serial.Serial()
ser.baudrate = 38400
ser.rts = False
ser.dtr = False
ser.timeout = 0

# CONSTANTS
SERIAL_READ_CONSTANT_LENGTH = 100000
MILLIS_RATIO = (1/1000)
SUCCESS = "SUCCESS"
FAILURE = "FAILURE"

# SERIAL DATA STORAGE
serial_output = ""
telemetry = ""

# THREAD VARIABLES
lock = threading.Lock()

def read_serial():
    global MILLIS_RATIO

    while True:
        time.sleep(100 * MILLIS_RATIO)
        lock.acquire()

        ser.baudrate = 38400
        ser.rts = False
        ser.dtr = False
        if ser.is_open == True:
            store_serial_output()

        lock.release()

thread = threading.Thread(target=read_serial)
thread.daemon = True
thread.start()

# UTILITY FUNCTIONS
def store_serial_output():
    global serial_output
    serial_output += ser.read(SERIAL_READ_CONSTANT_LENGTH)

def parse_telemetry():
    global serial_output
    global telemetry
    global MILLIS_RATIO

    if ser.is_open == False:
        return

    lock.acquire()

    ser.baudrate = 38400
    ser.rts = False
    ser.dtr = False

    ser.write("telemetry ascii\n")
    wait = 100 * MILLIS_RATIO
    time.sleep(wait)
    data = ser.read(SERIAL_READ_CONSTANT_LENGTH)

    # print(
    #     "=====================\n"
    #     + data +
    #     "=====================\n"
    # )

    start = data.find("START:")
    end = data.find("\x03\x03\x04\x04")

    if start != -1 and end != -1:
        telemetry = data[start:end]
        # TODO: get rid of the SJ Dev message "Finished in 1883 us" at the end
        # Remove the output from telemetry from the serial_output
        end_deliminator = "us\n"
        sanatized_output = data[:start] + data[end:]
        sanatized_output = re.sub(r'telemetry ascii\n', '', sanatized_output)
        sanatized_output = re.sub(r'LPC: ', '', sanatized_output)
        sanatized_output = re.sub(r'[\x03][\x03][\x04][\x04]   Finished in .*\n', '', sanatized_output)
        serial_output += sanatized_output

        # print(
        #     "===========TELEMETRY==========\n"
        #     + telemetry +
        #     "=====================\n"
        # )
        # print("===========SANATIZED==========\n"
        #     +sanatized_output+
        #     "=====================\n"
        # )

    elif start == -1 and end == -1:
        serial_output += data
    elif start != -1 and end == -1:
        serial_output += data[:start]
    # NOTE: This case should not be possible!
    elif start == -1 and end != -1:
        serial_output += data

    lock.release()

# SERVER ROUTES
@app.route('/js/<path:path>')
def send_js(path):
    return send_from_directory('js', path)

@app.route('/css/<path:path>')
def send_css(path):
    return send_from_directory('css', path)

@app.route('/lib/<path:path>')
def send_lib(path):
    return send_from_directory('lib', path)

@app.route('/')
def index():
    return render_template("index.html", version="version 0.0.1")

@app.route('/telemetry')
def return_telemetry():
    parse_telemetry()
    return telemetry

@app.route('/list')
def list():
    serial_devices_list = glob.glob("/dev/ttyUSB*")
    sorted_list = sorted(serial_devices_list)
    return json.dumps(sorted_list)

@app.route('/connect')
@app.route('/connect/<int:device>')
def connect(device=0):
    ser.close()
    ser.port = "/dev/ttyUSB%d" % (device)
    ser.open()
    return SUCCESS

@app.route('/disconnect', methods=['GET'])
def disconnect():
    ser.close()
    return SUCCESS

@app.route('/serial')
def serial():
    return serial_output

@app.route('/write/<string:payload>')
def write(payload=""):
    lock.acquire()
    payload += "\n"
    print(payload)
    ser.write(payload.encode('utf-8'))
    lock.release()
    return SUCCESS


@app.route('/set/<string:component_name>/<string:variable_name>/<value>')
def set(component_name, variable_name, value):
    lock.acquire()
    payload = "telemetry %s %s %g\n" % (component_name, variable_name, float(value))
    ser.write(payload.encode('utf-8'))
    lock.release()
    return SUCCESS