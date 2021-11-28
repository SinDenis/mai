from django.db import models
from django.contrib.auth.models import AbstractUser


class User(AbstractUser):
    level = models.FloatField(default=0, blank=True)
    tasks = models.ManyToManyField('Task', related_name='users')


class Task(models.Model):

    STATUSES = (
        (0, 'Not performed'),
        (1, 'Done'),
        (2, 'Expired')
    )

    name = models.CharField(max_length=50)
    description = models.CharField(max_length=255, blank=True)
    status = models.IntegerField(choices=STATUSES, default=0)
    creation_date = models.DateTimeField(auto_now_add=True)
    expiration_date = models.DateTimeField(blank=True, null=True)
    difficult = models.FloatField(default=1)
    deadline = models.DateTimeField(blank=True, null=True)
