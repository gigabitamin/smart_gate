from django.db import models

# Create your models here.

from django.db import models

class SensorData(models.Model):
    timestamp = models.DateTimeField(auto_now_add=True)
    distance = models.IntegerField()
    status = models.CharField(max_length=20, default="PASS")  # 차단기 상태
