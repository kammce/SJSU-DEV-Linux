from __future__ import division
from flask import Flask, request, render_template, jsonify, send_from_directory
import threading
import serial
import time
import glob
import json
import re
import os
from enum import Enum

class State(Enum):
    OFFLINE = 0
    SYSTEM_BOOTING = 1
    ONLINE_SYS_PROMPT = 2

# INITIAL STATE
state = State.OFFLINE
previous_prompt = -1

# CONSTANTS
SERIAL_READ_CONSTANT_LENGTH = 100000
MILLIS_RATIO = (1/1000)
SUCCESS = "SUCCESS"
FAILURE = "FAILURE"
COMPILED_PATTERN = re.compile('(?s)LPC: telemetry ascii\n(.*?)\n\x03\x03\x04\x04[ ]{3}Finished in [0-9]+ us\n')

# SETUP FLASK APPLICATION
app = Flask(__name__)
app.debug = True

# SETUP SERIAL PORT
ser = serial.Serial()
ser.baudrate = 38400
ser.rts = False
ser.dtr = False
ser.timeout = 0

# SERIAL DATA STORAGE
serial_output = ""
telemetry = ""
new_serial = ""

# THREAD VARIABLES
lock = threading.Lock()


def read_serial():
    global MILLIS_RATIO
    global COMPILED_PATTERN
    global telemetry
    global serial_output
    global new_serial
    global previous_prompt
    global state

    while True:
        time.sleep(100 * MILLIS_RATIO)

        ser.baudrate = 38400
        ser.rts = False
        ser.dtr = False

        print(state)

        if state == State.OFFLINE:
            found_prompt = serial_output.rfind("LPC:")
            if found_prompt > previous_prompt:
                previous_prompt = found_prompt
                state = State.ONLINE_SYS_PROMPT

        if ser.is_open == True:
            serial_output += ser.read(4096)
            # Find a last LPC telemetry request
            start = serial_output.rfind("LPC: telemetry ascii")
            # Find a single End of output substring
            end_array = re.findall(r"[\x03][\x03][\x04][\x04][ ]{3}Finished in [0-9]+ us", serial_output)
            end = -1

            if len(end_array) != 0:
                last_occurance = end_array[-1]
                end = serial_output.rfind(last_occurance)

            if start < end:
                arr = COMPILED_PATTERN.findall(serial_output)
                # print(start, end, serial_output, arr)
                if len(arr) != 0:
                    telemetry = arr[-1]
                serial_output = COMPILED_PATTERN.sub('', serial_output)
                # serial_output = ""
            elif start == -1 and end == -1:
                pass
                #serial_output += serial_output
                # serial_output = ""

thread = threading.Thread(target=read_serial)
thread.daemon = True
thread.start()

# UTILITY FUNCTIONS
def request_telemetry():
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

@app.route('/server-is-alive')
def server_is_alive():
    return SUCCESS

@app.route('/telemetry')
def return_telemetry():
    payload = ""

    if state == State.ONLINE_SYS_PROMPT:
        request_telemetry()
        payload = telemetry

    return payload

@app.route('/list')
def list():
    ttyUSB_list = glob.glob("/dev/ttyUSB*")
    ttyACM_list = glob.glob("/dev/ttyACM*")
    tty_list = ttyUSB_list + ttyACM_list
    sorted_tty_list = sorted(tty_list)
    return json.dumps(sorted_tty_list)

@app.route('/connect')
@app.route('/connect/<int:device>')
def connect(device=0):
    ser.close()
    ser.port = "/dev/ttyUSB%d" % (device)
    state = State.OFFLINE
    ser.open()
    return SUCCESS

@app.route('/disconnect', methods=['GET'])
def disconnect():
    ser.close()
    state = State.OFFLINE
    return SUCCESS

@app.route('/serial')
def serial():
    # print(serial_output)
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