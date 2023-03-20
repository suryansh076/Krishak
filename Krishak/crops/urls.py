from django.urls import path
from .views import *
urlpatterns = [
    path('',hello,name='hello'),
    path('data/',dataApi.as_view(),name='data'),
    path('result/',result,name='result')
]

