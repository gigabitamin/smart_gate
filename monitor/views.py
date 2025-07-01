import json
from django.shortcuts import render
from django.http import JsonResponse
from .models import SensorData
from django.http import HttpResponse
import serial
from django.views.decorators.csrf import csrf_exempt
from django.shortcuts import redirect


SERIAL_PORT = 'COM5'
BAUD_RATE = 9600

# def index(request):
#     return HttpResponse("Smart Gate Monitor")


def dashboard(request):
    data = SensorData.objects.order_by('-timestamp')[:50]  # 최신 50개 데이터

    # 리스트로 직렬화 (시간은 문자열로 변환)
    data_list = [{
        'timestamp': entry.timestamp.strftime('%H:%M:%S'),
        'temperature': entry.temperature,
        'humidity': entry.humidity,
        'distance': entry.distance,
    } for entry in reversed(data)]  # reversed 해서 오래된 순으로 정렬

    context = {
        'data': data,
        'data_json': json.dumps(data_list),
    }
    return render(request, 'monitor/dashboard.html', context)

def sensor_data_api(request):
    # 최근 50개 데이터 가져오기 (필요에 따라 조정)
    data = SensorData.objects.order_by('-timestamp')[:50]
    data = reversed(data)  # 시간 순 정렬

    result = {
        "timestamps": [d.timestamp.strftime("%H:%M:%S") for d in data],
        "temperature": [d.temperature for d in data],
        "humidity": [d.humidity for d in data],
        "distance": [d.distance for d in data],
        "accel_x": [d.accel_x for d in data],
        "accel_y": [d.accel_y for d in data],
        "accel_z": [d.accel_z for d in data],
    }
    return JsonResponse(result)

@csrf_exempt
def toggle_gate(request):
    if request.method == 'POST':
        try:
            with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser:
                ser.write(b'OPEN\n')
        except Exception as e:
            return HttpResponse(f'Error: {e}', status=500)
    return redirect('dashboard')  # URL name을 사용했을 경우. 아니면 '/dashboard'로 변경