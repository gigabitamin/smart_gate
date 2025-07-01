from django.db import models

class SensorData(models.Model):
    timestamp = models.DateTimeField(auto_now_add=True)
    temperature = models.FloatField()
    humidity = models.FloatField()
    distance = models.IntegerField()
    accel_x = models.IntegerField()
    accel_y = models.IntegerField()
    accel_z = models.IntegerField()
    gate_status = models.CharField(max_length=10)  # "PASS" or "STOP"

    def __str__(self):
        return f"{self.timestamp} | {self.distance}cm"
