"""ahck URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/2.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path
from rest_framework import  routers
from .views import (PacketViewSet, CargoViewSet, ServiceClassViewSet, 
                    FlightViewSet, SystemUserViewSet, ActOfShortageViewSet, MockViewSet)


router = routers.DefaultRouter()
router.register(r'packets', PacketViewSet, base_name='packet')
router.register(r'cargos', CargoViewSet, base_name='cargo')
router.register(r'service_class', ServiceClassViewSet, base_name='service_class')
router.register(r'flights', FlightViewSet, base_name='flight')
router.register(r'systemusers', SystemUserViewSet, base_name='systemuser')
router.register(r'acts', ActOfShortageViewSet, base_name='act')
router.register(r'mocks', MockViewSet, base_name='mock')

urlpatterns = [
    path('admin/', admin.site.urls),
]

urlpatterns += router.urls
