#version 330 core
out vec4 FragColor;

in vec3 TexCoords;  //3D��������ķ�������

uniform samplerCube skybox; //������������ͼ�����������
uniform float ratio;

void main()
{    
	//��3D��������ķ������������������������������Ⱦ
    FragColor = ratio * texture(skybox, TexCoords);
}