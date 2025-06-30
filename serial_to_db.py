import serial
import os
import django

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'smart_gate.settings')
django.setup()

from gate.models import SensorData  # models.py에 정의된 모델

ser = serial.Serial('COM3', 9600)  # 아두이노 포트 확인 필요

while True:
    line = ser.readline().decode('utf-8').strip()
    print(f"[수신] {line}")

    # ex: "DISTANCE:123"
    if line.startswith("DISTANCE:"):
        dist = int(line.split(":")[1])
        SensorData.objects.create(distance=dist)
