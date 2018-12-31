#version 330 core
layout (location = 0) in vec3 aPos;           //����λ����Ϣ
layout (location = 1) in vec3 aNormal;        //���㷨��
layout (location = 2) in vec2 aTexCoords;     //��������

out vec2 TexCoords;                           //�����������ƬԪ��ɫ��

out VS_OUT {                                  
    vec3 FragPos;                             //ƬԪλ��
    vec3 Normal;                              //ƬԪ����
    vec2 TexCoords;                           //��������
    vec4 FragPosLightSpace;                   //ƬԪλ���ڹ�ռ��λ��
} vs_out;

uniform mat4 projection;                      //͸�Ӿ���
uniform mat4 view;                            //�۲����
uniform mat4 model;                           //ģ�;���
uniform mat4 lightSpaceMatrix;                //��ռ��ģ�Ӿ���

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));               //��������ϵ�ϵ�λ��
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;    //��������ϵ�ϵķ���
    vs_out.TexCoords = aTexCoords;                                //�������겻�ñ�
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0); //��������ϵ�仯����ռ�ϵ 
	
    gl_Position = projection * view * model * vec4(aPos, 1.0); //�������ϵ��λ��
}