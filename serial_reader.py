import os
import django
import serial
import time

# Django 환경 세팅
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'smart_gate.settings')
django.setup()

from monitor.models import SensorData

SERIAL_PORT = 'COM5'
BAUD_RATE = 9600

def main():
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print("Serial port opened")
    except serial.SerialException:
        print("Cannot open serial port")
        return

    latest_data = {
        'temperature': None,
        'humidity': None,
        'distance': None,
        'accel_x': None,
        'accel_y': None,
        'accel_z': None,
        'gate_status': None
    }

    while True:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        if not line:
            time.sleep(0.1)
            continue

        print(f"Received: {line}")

        try:
            if line.startswith("DIST:"):
                latest_data['distance'] = int(line.split(":", 1)[1].strip())

            elif line.startswith("Avg Accel:"):
                parts = line.split(":", 1)[1].split(",")
                latest_data['accel_x'] = int(parts[0].strip())
                latest_data['accel_y'] = int(parts[1].strip())
                latest_data['accel_z'] = int(parts[2].strip())

            elif line.startswith("GATE:"):
                latest_data['gate_status'] = line.split(":", 1)[1].strip()

            elif line.startswith("temp:"):
                latest_data['temperature'] = float(line.split(":", 1)[1].strip())

            elif line.startswith("humidity:"):
                latest_data['humidity'] = float(line.split(":", 1)[1].strip())

            # 모든 데이터가 준비되었으면 DB 저장
            if all(value is not None for value in latest_data.values()):
                SensorData.objects.create(**latest_data)
                print("Data saved to DB:", latest_data)
                latest_data = {k: None for k in latest_data}

        except Exception as e:
            print(f"Parsing error: {e}")

        time.sleep(0.1)

if __name__ == "__main__":
    main()
