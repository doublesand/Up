#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <up/shader.h>
#include <up/camera.h>
#include <up/model.h>

#include <iostream>

//-----------------------------------------------------------ȫ�ֱ���
// ���ô��ڵĸߺͿ�
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// ����������������������Ĭ�ϲ����������ƶ�һ�㣨Ҳ����Z�ᣩ
Camera camera(glm::vec3(0.0f, 0.0f, 7.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// ��������֡����Ⱦ���������Ӧ�ĸı䣬��֤��������
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// ��Դ
glm::vec3 lightPos(3.4f, 9.6f, 16.9f);

//���ڰ����ҹ
float ratio = 1.0;

//�����˲���
float robot_x = 0.0;
float robot_z = 1.0;
float robot_step = 1;
float robot_rotate = 0.0;
float robot_turn_front = 0.0;

//-----------------------------------------------------------��������
//һЩ����Ҫ�õ��Ŀ�ĳ�ʼ��
GLFWwindow* initWindows();

//����window���ڵİ�����������Ӧ
void processInput(GLFWwindow *window);
//�������ڵĴ�С�任�����ӿڵı仯
void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
//��������ƶ��ı�ƫ���Ǻ͸�����
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//�������Ĺ����ı�͸��ͶӰ����Ұ
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//����ͼƬ������GPU��������������ID
unsigned int loadTexture(const char *path);


//-----------------------------------------------------------����ʵ��
inline GLFWwindow * initWindows()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}
	return window;
}

// ���̼�������
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)   //ESC�˳�
		glfwSetWindowShouldClose(window, true);

	//deltaTime����Ⱦ���ʱ�䣬���������ٶ�
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)        //W��ǰ��
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)        //S�Ǻ���
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)        //A������
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)        //D������
		camera.ProcessKeyboard(RIGHT, deltaTime);

	// ���ں�ҹ����
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		if(ratio < 1.0)
			ratio += 0.05;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		if (ratio > 0.2)
			ratio -= 0.05;

	// ����������
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		robot_turn_front = 0.0;
		robot_rotate += robot_step;
		robot_z += 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		robot_turn_front = 0.0;
		robot_rotate += robot_step;
		robot_z -= 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		robot_turn_front = 90.0;
		robot_rotate += robot_step;
		robot_x += 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		robot_turn_front = 90.0;
		robot_rotate += robot_step;
		robot_x -= 0.01;
	}
	if (robot_rotate > 30) robot_step = -robot_step;
	if (robot_rotate < -30) robot_step = -robot_step;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// ȷ���ӿ����µĴ��ڳߴ���ƥ�䣬ע�ⲻҪ���ӿڴ����Լ�����Ļ
	glViewport(0, 0, width, height);
}

// ����ƶ��������������
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)  //������һ�ν����ӿھͻᱻ��׽
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX; //X�����ƫ����
	float yoffset = lastY - ypos; //Y�����ƫ������������Ļ����ϵ��Щ��ͬ

	lastX = xpos;  //���µ�ǰλ��
	lastY = ypos;  

	camera.ProcessMouseMovement(xoffset, yoffset);  //��ƫ�����������
}

// ����������
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// �������������������ID
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	//ͼƬ���ߡ��Լ���ɫͨ������ɫͨ������ȥ��ͼƬ��һЩֵ��
	int width, height, nrComponents;
	//����stb_image�⽫ͼƬת���ֽ���
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;    
		if (nrComponents == 1)       //��ɫ
			format = GL_RED;
		else if (nrComponents == 3)  //��ɫ
			format = GL_RGB;
		else if (nrComponents == 4)  //��͸����
			format = GL_RGBA;

		//������
		glBindTexture(GL_TEXTURE_2D, textureID);
		//2D�������ò������ݵ�GPU
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		//�༶��Զ������
		glGenerateMipmap(GL_TEXTURE_2D);

		//����S�����T����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//Ϊ��С�ͷŴ�ָ������ģʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}

