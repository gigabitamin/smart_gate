from django.urls import path
from . import views

urlpatterns = [
    # path('', views.index, name='index'),
    path('', views.dashboard, name='dashboard'),
    path('api/sensor_data/', views.sensor_data_api, name='sensor_data_api'),
    path('toggle_gate', views.toggle_gate, name='toggle_gate'), 
    path('api/data/', views.sensor_data_api),
]