from django.db import models
from django.db.models import fields


class TimestampMixin(models.Model):
    class Meta:
        abstract = True
    created = models.DateTimeField(name='дата создания', auto_now_add=True)
    modified = models.DateTimeField(name='дата изменения', auto_now=True)
