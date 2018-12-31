#version 330 core
layout (location = 0) in vec3 aPos;  //����λ����Ϣ

uniform mat4 lightSpaceMatrix;       //��ռ�仯����
uniform mat4 model;                  //ģ�͵ı任����

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}