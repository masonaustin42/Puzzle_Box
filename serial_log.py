import serial
import csv
import time

# Set up the serial connection
serial_port = 'COM5'
baud_rate = 9600  # Adjust this to match your device's baud rate
timeout = 1  # Adjust as needed

# Open the serial port
ser = serial.Serial(serial_port, baud_rate, timeout=timeout)

# Create or open the CSV file
csv_file = 'output.csv'

# Initialize the CSV writer
with open(csv_file, mode='w', newline='') as file:
    writer = csv.writer(file)
    
    try:
        while True:
            # Read data from the serial port
            if ser.in_waiting > 0:
                data = ser.readline().decode('utf-8').strip()
                # Split the incoming data assuming it's in CSV format
                csv_data = data.split(',')
                
                # Write data to the CSV file
                writer.writerow(csv_data)
                print(f"Data: {csv_data}")
    
    except KeyboardInterrupt:
        # Close the serial connection on user interruption
        print("Terminating the script.")
    
    finally:
        ser.close()
        print("Serial port closed.")