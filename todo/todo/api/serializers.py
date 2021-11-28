from rest_framework.authtoken.models import Token

from rest_framework.serializers import (
    Serializer,
    ModelSerializer,
    IntegerField
)

from .models import (
    User,
    Task
)


class UserCreateSerializer(ModelSerializer):

    class Meta:
        model = User
        fields = (
            'username',
            'password',
            'email'
        )
        extra_kwargs = {
            "password": {
                "write_only": True
            }
        }

    def create(self, validated_data):
        username = validated_data['username']
        password = validated_data['password']
        email = validated_data['email']
        user_obj = User(
            username=username,
            email=email
        )
        user_obj.set_password(password)
        user_obj.save()
        return validated_data


class UserListSerializer(ModelSerializer):

    class Meta:
        model = User
        fields = ('username', 'email', 'level')


class TaskCreateSerializer(ModelSerializer):

    class Meta:
        model = Task
        fields = (
            'name',
            'description',
            'difficult'
        )
    
    def create(self, validated_data):
        name = validated_data['name']
        description = validated_data['description']
        difficult = validated_data['difficult']

        task_obj = Task(
            name=name,
            description=description,
            difficult=difficult
        )

        task_obj.save()
        return validated_data


class TaskListSerializer(ModelSerializer):

    class Meta:
        model = Task
        fields = '__all__'


class TaskDeleteSerializer(Serializer):
    id = IntegerField()


class TaskUpdateSerializer(ModelSerializer):
    users = UserListSerializer(many=True)
    id = IntegerField()

    class Meta:
        model = Task
        fields = '__all__'

    def update(self, instance, validated_data):
        instance.name = validated_data.get('name', instance.name)
        instance.description = validated_data.get('description', instance.description)
        instance.status = validated_data.get('status', instance.status)
        instance.difficult = validated_data.get('difficult', instance.difficult)
        instance.deadline = validated_data.get('deadline', instance.deadline)
        instance.save()
        return instance
