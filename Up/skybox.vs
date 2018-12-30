#version 330 core
layout (location = 0) in vec3 aPos;    //��������Ϊ0��aPos

out vec3 TexCoords;       //��������������������������꣬��������ͼ��Ⱦ������֮��

uniform mat4 projection;  //͸�Ӿ���������ʵ��
uniform mat4 view;        //�۲���������ͼ��Ӧ��һ�ӽǲ��䣬Ӫ����պܴ�ĸо�

void main()
{
    TexCoords = aPos;      //���ͼ�������������ʵ������
    vec4 pos = projection * view * vec4(aPos, 1.0);  //���ͼû��model���������ͼ���������
    gl_Position = pos.xyww;    
}  