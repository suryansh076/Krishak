from django.contrib import admin
from .models import *
class detail(admin.ModelAdmin):
    list_display=['n_value','p_value','k_value','temp','humidity']
admin.site.register(Data,detail)