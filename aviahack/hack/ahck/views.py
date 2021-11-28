from rest_framework.generics import ListAPIView
from rest_framework.response import Response
from rest_framework import viewsets
from .models import Packet, Cargo, ServiceClass, SystemUser, Flight, ActOfShortage, Mock
from .serializers import (PacketSerializer, CargoSerializer, ServiceClassSerializer, 
                            SystemUserSerializer, FlightSerializer, ActOfShortageSerializer, MockSerializer)


class PacketViewSet(viewsets.ModelViewSet):
    serializer_class = PacketSerializer
    queryset = Packet.objects.all()

    def list(self, request, *args, **kwargs):
        queryset = self.filter_queryset(self.get_queryset())
        serializer = self.get_serializer(queryset, many=True)

        return Response(serializer.data)


class CargoViewSet(viewsets.ModelViewSet):
    serializer_class = CargoSerializer
    queryset = Cargo.objects.all()


class ServiceClassViewSet(viewsets.ModelViewSet):
    serializer_class = ServiceClassSerializer
    queryset = ServiceClass.objects.all()


class FlightViewSet(viewsets.ModelViewSet):
    serializer_class = FlightSerializer
    queryset = Flight.objects.all()


class SystemUserViewSet(viewsets.ModelViewSet):
    serializer_class = SystemUserSerializer
    queryset = SystemUser.objects.all()


class ActOfShortageViewSet(viewsets.ModelViewSet):
    serializer_class = ActOfShortageSerializer
    queryset = ActOfShortage.objects.all()


class MockViewSet(viewsets.ModelViewSet):
    serializer_class = MockSerializer
    queryset = Mock.objects.all()
