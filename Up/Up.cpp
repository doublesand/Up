#include <up/up.h>
#include <up/skybox.h>
#include <up/floor.h>
#include <up/cube.h>
#include <up/depthMap.h>

int main()
{
	GLFWwindow* window = initWindows();
	if (window == NULL) return -1;

	// ����Opengl��ȫ�����ã�������Ȼ���
	glEnable(GL_DEPTH_TEST);

	// ��ʼ����ɫ��
	Shader skyboxShader("skybox.vs", "skybox.fs");
	Shader simpleDepthShader("shadow_depth.vs", "shadow_depth.fs");
	Shader shader("shadow_map.vs", "shadow_map.fs");  

	// ������ɫ��
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// ������Ӱ��ɫ��
	shader.use();
	shader.setInt("diffuseTexture", 0);   //������ͼ
	shader.setInt("shadowMap", 1);        //��Ӱ��ͼ

	Skybox   skybox;       //���
	Floor    floor;        //�ذ�
	DepthMap depthMap;     // ��Դ��ͼ�������������Ӱ

	// ������������
	unsigned int woodTexture = loadTexture("resources/textures/sufei.jpg");
	skybox.loadCubemap();

	// ѭ����Ⱦ
	while (!glfwWindowShouldClose(window))
	{
		// ����ÿһ֡�ĸ����ٶ�������Ӧ�ĵ���
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//����������������Ӧ����Ӧ
		processInput(window);

		// change light position over time
		lightPos.x = sin(glfwGetTime()) * 3.0f;
		lightPos.z = cos(glfwGetTime()) * 2.0f;
		lightPos.y = 5.0 + cos(glfwGetTime()) * 1.0f;

		// ��Ⱦ
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�����Ȼ������ɫ����

		// �õ���Դ��ͼ�������Ϣ����Ϊ���������ͼ���� ��Ӱ���ơ�
		depthMap.renderMap(simpleDepthShader);  //�����������

		// ������һ����Ⱦ������ֻ�������˸�������� 
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		// ���ù�Դ��Ϣ�������Ϣ
		shader.setVec3("viewPos", camera.Position);
		shader.setVec3("lightPos", lightPos);
		shader.setMat4("lightSpaceMatrix", depthMap.lightSpaceMatrix);
			
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap.textureID);
		floor.draw(shader);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture);
		depthMap.renderScene(shader);      //��Ⱦ����
		skybox.draw(skyboxShader);         //��Ⱦ���ͼ

		// ���»����IO�¼�
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//���glfw��ʼ���Ķ���
	glfwTerminate();
	return 0;
}
