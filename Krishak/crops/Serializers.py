from rest_framework import  serializers
from .models import *
class UserSerializer(serializers.ModelSerializer):
    class Meta:
        model = Data
        fields = '__all__'