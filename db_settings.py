DATABASES = {
    "default": {
        "ENGINE": "django.db.backends.mysql",
        "NAME": "smart_gate",
        "PORT": "3306",
        
        "USER": "root",         
        "PASSWORD": "1234",         
        "HOST": "localhost",
    
    }
}

SECRET_KEY = "django-insecure-m+3q5446@-f+)_f8@p5yc1xmrzgd($tp81#e)jjh@^rjnvl++m"

ALLOWED_HOSTS = [
    'localhost',
    '127.0.0.1',
    '192.168.44.9',    
]

CORS_ALLOWED_ORIGINS = [
    'http://127.0.0.1:8000',
    'http://192.168.44.9:8000',
    'http://localhost:8000',
]

DjangoServer = 'http://localhost:8000'