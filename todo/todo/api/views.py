from rest_framework.authtoken.models import Token
from rest_framework.response import Response
from rest_framework.authtoken.views import ObtainAuthToken

from rest_framework.generics import (
    CreateAPIView,
    ListAPIView,
    DestroyAPIView,
    # UpdateAPIView,
)

from rest_framework.views import APIView

# from rest_framework.status import (
#     HTTP_200_OK,
#     HTTP_400_BAD_REQUEST
# )

from rest_framework import (
    permissions,
    authentication
)

from .serializers import (
    UserCreateSerializer,
    UserListSerializer,
    TaskCreateSerializer,
    TaskListSerializer,
    TaskDeleteSerializer
)

from .models import (
    User,
    Task
)

class UserCreateAPIView(CreateAPIView):
    serializer_class = UserCreateSerializer
    queryset = User.objects.all()


class UserListAPIView(ListAPIView):
    serializer_class = UserListSerializer
    queryset = User.objects.all()


class UserAuthToken(ObtainAuthToken):
    
    def post(self, request, *args, **kwargs):
        serializer = self.serializer_class(
            data=request.data,
            context={'request': request}
        )
        serializer.is_valid(raise_exception=True)
        user = serializer.validated_data['user']
        token, created = Token.objects.get_or_create(user=user)
        return Response({
            'token': token.key,
            'user_id': user.pk,
            'email': user.email
        })


class TaskAPIView(APIView):
    authentication_classes = (authentication.TokenAuthentication,)
    permission_classes = (permissions.IsAuthenticated,)

    def get(self, request, *args, **kwargs):
        user = User.objects.get(id=request.auth.user_id)
        tasks = user.tasks.all()
        response = [TaskListSerializer(task).data for task in tasks]
        return Response(response)


    def post(self, request, *args, **kwargs):
        user = User.objects.get(id=request.auth.user_id)
        serializer = TaskListSerializer(data=request.data)
        serializer.is_valid(raise_exception=True)
        name = serializer.validated_data['name']
        description = serializer.validated_data['description']
        difficult = serializer.validated_data['difficult']

        task_obj = Task(
            name=name,
            description=description,
            difficult=difficult
        )

        task_obj.save()
        user.tasks.add(task_obj)
        user.save()
        return Response(TaskListSerializer(task_obj).data)


    def delete(self, request, *args, **kwargs):
        serializer = TaskDeleteSerializer(data=request.data)
        serializer.is_valid(raise_exception=True)
        id = serializer.validated_data['id']
        print(id)
        task = Task.objects.get(id=id)
        task.delete()
        return Response('{"Delete": "Done"}')
