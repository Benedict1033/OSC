#! /usr/bin/python3

import os
import sys
import time
import serial

BAUD_RATE = 115200

def print_progress_bar(iteration, total, elapsed_time, prefix='', suffix='', decimals=1, length=50, fill='█'):
    """
    Call in a loop to create terminal progress bar with elapsed time
    @params:
        iteration     - Required  : current iteration (Int)
        total         - Required  : total iterations (Int)
        elapsed_time  - Required  : time since start (Float)
        prefix        - Optional  : prefix string (Str)
        suffix        - Optional  : suffix string (Str)
        decimals      - Optional  : positive number of decimals in percent complete (Int)
        length        - Optional  : character length of bar (Int)
        fill          - Optional  : bar fill character (Str)
    """
    percent = ("{0:." + str(decimals) + "f}").format(100 * (iteration / float(total)))
    filled_length = int(length * iteration // total)
    bar = fill * filled_length + '-' * (length - filled_length)
    sys.stdout.write(f'\r{prefix} |{bar}| {percent}% {suffix} Time: {elapsed_time:.2f}s')
    sys.stdout.flush()

def send_img(ser, kernel):
    print("Please sent the kernel image size:")
    kernel_size = os.stat(kernel).st_size
    ser.write((str(kernel_size) + "\n").encode())
    print("Start to load the kernel image...")

    start_time = time.time()  # Record the start time
    sent = 0  # keep track of bytes sent
    with open(kernel, "rb") as image:
        while True:
            chunk = image.read(1)
            if not chunk:
                break
            ser.write(chunk)
            time.sleep(0.01)  # Adjust this to match your device's buffering and processing capabilities
            sent += len(chunk)
            elapsed_time = time.time() - start_time  # Calculate elapsed time
            print_progress_bar(sent, kernel_size, elapsed_time, prefix='Progress:', suffix='Complete', length=50)

    print("\nKernel image sent successfully.")

if __name__ == "__main__":
    ser = serial.Serial("/dev/ttyUSB0", BAUD_RATE, timeout=500)
    #ser = serial.Serial("/dev/pts/1", BAUD_RATE, timeout=500)
    send_img(ser, "./kernel8.img")

