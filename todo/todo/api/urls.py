from django.urls import path

from .views import (
    UserCreateAPIView,
    UserAuthToken,
    UserListAPIView,
    TaskAPIView
)

urlpatterns = [
    path('register/', UserCreateAPIView.as_view()),
    path('login/', UserAuthToken.as_view()),
    path('user/', UserListAPIView.as_view()),
    path('task/', TaskAPIView.as_view())
]
