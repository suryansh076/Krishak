from django.db import models

class Data(models.Model):
    n_value=models.IntegerField()
    p_value=models.IntegerField()
    k_value=models.IntegerField()
    temp=models.IntegerField()
    humidity=models.IntegerField()
