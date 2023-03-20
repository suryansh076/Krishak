from django.shortcuts import render
from .Serializers import UserSerializer
from rest_framework.views import APIView
from .models import *
from .Serializers import *
from django.http import HttpResponse
from rest_framework.response import Response
from django.shortcuts import render
from django.http import HttpResponse
import joblib
import numpy as np
def result(request):
   print("8888888888888888888888888888888888888888")
   cls=joblib.load("C:/Users/iamsu/OneDrive/Desktop/model.pkl")
   lis=[]
   lis.append(12)
   lis.append(20)
   lis.append(50)
   lis.append(30)
   lis.append(50)
   lis.append(6.5)
   lis.append(150)
   #print(lis)
   data_array = np.asarray(lis)
   arr= data_array.reshape(1,-1)
   ans = cls.predict(arr)
   print('====================',ans)
   return HttpResponse(f'Suitable Crop is {ans}')
# Create your views here.
class dataApi(APIView):
    def get(self,request):
        data=Data.objects.all()
        print("*****************",request.GET.get('n'),request.GET.get('p'),request.GET.get('k'),request.GET.get('t'),request.GET.get('h'))
        seri=UserSerializer(data,many=True)
        print("8888888888888888888888888888888888888888")
        cls=joblib.load("C:/Users/iamsu/OneDrive/Desktop/model.pkl")
        lis=[]
        lis.append(float(request.GET.get('n')))
        lis.append(float(request.GET.get('p')))
        lis.append(float(request.GET.get('k')))
        lis.append(float(request.GET.get('t')))
        lis.append(float(request.GET.get('h')))
        lis.append(6.5)
        lis.append(103.7)
        #print(lis)
        data_array = np.asarray(lis)
        arr= data_array.reshape(1,-1)
        ans = cls.predict(arr)
        print('====================',ans)
        return Response({'ans:':ans})
    def post(self,request):
        # Data.objects.create(n_value=request.data['n'],p_value=request.data['p'],k_value=request.data['k'],temp=request.data['t'],humidity=request.data['h'])
        data=Data.objects.all()
        seri=UserSerializer(data,many=True)
        return Response(seri.data)

    
def hello(request):
    return render(request,'index.html')
