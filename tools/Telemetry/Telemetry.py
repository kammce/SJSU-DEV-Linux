from __future__ import division
from flask import Flask, request, render_template, jsonify, send_from_directory
import threading
import serial
import serial.tools.list_ports
import time
import glob
import json
import re
import os
import logging
import webbrowser
from enum import Enum

log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

class State(Enum):
    OFFLINE = 0
    SYSTEM_BOOTING = 1
    ONLINE_SYS_PROMPT = 2

# INITIAL STATE
state                           = State.OFFLINE
# CONSTANTS
SERIAL_READ_CONSTANT_LENGTH     = 100000
# BAUDRATE                        = 115200
MILLIS_RATIO                    = (1/1000)
SUCCESS                         = "SUCCESS"
FAILURE                         = "FAILURE"
POSSIBLE_PROMPTS                = ("LPC: ","CLI> ")
PROMPT_CAPTURE_GROUP            = "("+("|".join(POSSIBLE_PROMPTS))+")"
FULL_TELEMETRY_PATTERN          = re.compile('(?s)'+PROMPT_CAPTURE_GROUP+'telemetry ascii(.*)[\x03][\x03][\x04][\x04][ ]{3}Finished in [0-9]+ us[\r]*\n')
PARTIAL_TELEMETETRY_PATTERN     = re.compile('(?s)'+PROMPT_CAPTURE_GROUP+'telemetry ascii(.*)')

# SETUP FLASK APPLICATION
app                             = Flask(__name__)
app.debug                       = True

# SERIAL DATA STORAGE
serial_output                   = ""
baudrate                        = 38400

# SETUP SERIAL PORT
list_ports                      = serial.tools.list_ports
ser                             = serial.Serial()
ser.baudrate                    = baudrate
ser.rts                         = False
ser.dtr                         = False
ser.timeout                     = 0
current_prompt                  = ""

# THREAD VARIABLES
lock = threading.Lock()

def read_serial():
    global serial_output
    global state
    global current_prompt

    while True:
        time.sleep(10 * MILLIS_RATIO)
        # If we are
        if ser.is_open == True:
            # Check if system is booting
            if state == State.SYSTEM_BOOTING:
                # If we find a LPC: prompt, then we change state to ONLINE_SYS_PROMPT
                for prompt in POSSIBLE_PROMPTS:
                    if serial_output.rfind(prompt) != -1:
                        state = State.ONLINE_SYS_PROMPT
                        current_prompt = prompt

            # Lock control of serial device
            lock.acquire()
            try:
                # Read from serial device
                serial_output += ser.read(SERIAL_READ_CONSTANT_LENGTH)
                serial_output = serial_output.replace('\r', '')
            except Exception, e:
                print("Serial read exception: " + str(e))
                continue
            # Release serial lock
            lock.release()

def get_telemetry():
    global serial_output
    lock.acquire()
    DELAY_PERIOD  = 10 # ms
    TIMEOUT_LIMIT = 1000 # ms
    # Define telemetry variable
    ''' The default and "invalid" telemetry value is an empty string '''
    telemetry     = ""
    done          = False
    timeout_time  = 0
    telemetry_msg = "telemetry ascii\n"

    if ser.is_open == True and state == State.ONLINE_SYS_PROMPT:
        # if serial_output.endswith(POSSIBLE_PROMPTS):
        #     ser.write(telemetry_msg)
        # else:
        #     ser.write("\n"+telemetry_msg)
        ser.write(telemetry_msg)

        while(not done):
            time.sleep(10 * MILLIS_RATIO)

            try:
                serial_output += ser.read(SERIAL_READ_CONSTANT_LENGTH)
            except Exception, e:
                print("Serial read exception" + str(e))
                continue

            end_array = FULL_TELEMETRY_PATTERN.findall(serial_output)

            # print(serial_output)
            # print(end_array)

            if len(end_array) > 0:
                telemetry  = end_array[-1][-1]
                # If telemetry is found, trim it from the serial_output
                serial_tmp = FULL_TELEMETRY_PATTERN.sub('', serial_output)
                serial_tmp = PARTIAL_TELEMETETRY_PATTERN.sub('', serial_tmp)
                serial_tmp = serial_tmp.replace('\r', '')
                serial_output = serial_tmp
                # if not serial_output.endswith(current_prompt):
                #     serial_output += current_prompt
                done = True

            timeout_time += DELAY_PERIOD
            if(timeout_time > TIMEOUT_LIMIT):
                break

    lock.release()
    return telemetry

## SERVER FILE ROUTES
# Serve up JavaScript files
@app.route('/js/<path:path>')
def send_js(path):
    return send_from_directory('js', path)
# Serve up CSS files
@app.route('/css/<path:path>')
def send_css(path):
    return send_from_directory('css', path)
# Serve files within lib (library) folder
@app.route('/lib/<path:path>')
def send_lib(path):
    return send_from_directory('lib', path)
# Serve up index.html file when GET / request is received
@app.route('/')
def index():
    return render_template("index.html", version="version 0.0.2")
# Respond to requester with SUCCESS
'''
The purpose of this function is to determine if the server is alive and can respond to this request.
The requester's XHR will return error if the server does not resolve or has a timeout.
'''
@app.route('/server-is-alive')
def server_is_alive():
    return SUCCESS
# Returns results of get_telemetry()
@app.route('/telemetry')
def telemetry():
    return get_telemetry()
# Return the list of serial devices on system
@app.route('/list')
def list():
    # Get COM port iterator list
    port_iterator = list_ports.comports()
    # Empty list to fill with COM port paths
    ports         = []
    # Iterate through port_iterator elements
    # and push to ports list
    for element in port_iterator:
        ports.append(element.device)
    # Sort the ports
    ports = sorted(ports)
    # Return JSON (array) back to client
    return json.dumps(ports)

# Connect serial to device and return success
@app.route('/connect')
@app.route('/connect/<string:device>')
def connect(device):
    global serial_output
    global state
    ser.close()
    serial_output = ""
    ser.port = "/dev/" + device
    state = State.SYSTEM_BOOTING
    ser.open()
    return SUCCESS
# Change baud rate of serial device
@app.route('/baudrate/<int:baud>')
def devicebaud(baud):
    ser.baudrate = baud
    return SUCCESS
# Disconnect from serial device
@app.route('/disconnect', methods=['GET'])
def disconnect():
    global state
    ser.close()
    state = State.OFFLINE
    return SUCCESS
# Return serial_output
@app.route('/serial')
def serial():
    return serial_output
# Serial write string (payload) to serial device
@app.route('/write/<string:payload>/<int:carriage_return>/<int:newline>')
def write(payload="", carriage_return=0, newline=0):
    lock.acquire()

    cr = ""
    nl = ""

    if carriage_return == 1:
        cr = "\r"
    if newline == 1:
        nl = "\n"

    payload = payload+cr+nl

    ser.write(payload.encode('utf-8'))
    lock.release()
    return SUCCESS
# Perform a telemetry set variable operation
@app.route('/set/<string:component_name>/<string:variable_name>/<string:value>')
def set(component_name, variable_name, value):
    lock.acquire()
    payload = "telemetry %s %s %s\n" % (component_name, variable_name, value)
    ser.write(payload.encode('utf-8'))
    lock.release()
    return SUCCESS

# Open This application's web URL in default browser
webbrowser.open('http://localhost:5001')
# Turn on read serial thread
thread = threading.Thread(target=read_serial)
# Allow it to work in the background, but also die when main thread is killed
thread.daemon = True
# Start the thread
thread.start()

