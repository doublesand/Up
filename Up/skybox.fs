#version 330 core
out vec4 FragColor;

in vec3 TexCoords;  //3D��������ķ�������

uniform samplerCube skybox; //������������ͼ�����������
uniform float bili;

void main()
{    
	//��3D��������ķ������������������������������Ⱦ
    FragColor = texture(skybox, TexCoords);
}