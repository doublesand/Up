#include <up/up.h>
#include <up/skybox.h>
#include <up/floor.h>
#include <up/cube.h>

void renderScene(const Shader &shader);

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
	
	Skybox skybox;   //���
	Floor  floor;    //�ذ�
	

	// ����ذ�����
	unsigned int woodTexture = loadTexture("resources/textures/sufei.jpg");

	// ����֡��ȶ��󻺴�ʵ����Ӱ
	//���������е�������Щ���ֵ
	const unsigned int SHADOW_WIDTH = 1280, SHADOW_HEIGHT = 720;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);  // ����һ��֡�������
	unsigned int depthMap;               
	glGenTextures(1, &depthMap);         // ������Ȼ�������
	glBindTexture(GL_TEXTURE_2D, depthMap);
	// GL_DEPTH_COMPONENT �����������Ϣ
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };  //����ı߽�����Ϊ1.0����ɫ 
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	// ����Ȼ�������Ҳ����֡�������������Ϣ������ depthMap �ϡ�
	// ��ǰ��֡����洢��depthMap��������Ϣ
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	// �����������Opengl���ǲ���Ҫ��ɫ���ݣ�ֻҪ�����Ϣ
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);  //���


	// shader configuration
	// --------------------
	shader.use();
	shader.setInt("diffuseTexture", 0);
	shader.setInt("shadowMap", 1);

	//���ͼ��ʼ��
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

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
		//lightPos.x = sin(glfwGetTime()) * 3.0f;
		//lightPos.z = cos(glfwGetTime()) * 2.0f;
		//lightPos.y = 5.0 + cos(glfwGetTime()) * 1.0f;

		// ��Ⱦ
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�����Ȼ������ɫ����

		// 1. render depth of scene to texture (from light's perspective)
		// ��Ⱦһ�������������Ϣ��Ϊ���������ͼ���л�����Ӱ��(�ӹ�Դ���ӽ���)
		glm::mat4 lightProjection, lightView;        //��Դ��͸�Ӿ��󣬹�Դ�Ĺ۲����
		glm::mat4 lightSpaceMatrix;                  //��ռ�任����
		float near_plane = 1.0f, far_plane = 7.5f;   //ͶӰ���������Զ�ľ���
		//ͶӰ��ʽ
		//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));  //��Դ���򳡾����룬�������ǵ�����ǶȾͿ�������һ���������
		lightSpaceMatrix = lightProjection * lightView;  //ͶӰ*�۲�
		// �ӹ����ͼ������Ⱦ����
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		//һ��Ҫ��glViewport�����ܱ�֤�ӿڵ���Ӱ����̫С���߲�����
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);   //����Դ��ͼ�������Ϣ�浽depthMapFBO��
		glClear(GL_DEPTH_BUFFER_BIT);                     //�����Ȼ���
		glActiveTexture(GL_TEXTURE0);                     //��������
		glBindTexture(GL_TEXTURE_2D, woodTexture);        //���������
		renderScene(simpleDepthShader);                   //������ɫ��������Ⱦ
		glBindFramebuffer(GL_FRAMEBUFFER, 0);             //�����֡���������н��

		// ����ͼ�������Ϣ�㶨���ӿ�Ҫ�л���ԭ�������ӣ�����ͼ��
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 2. render scene as normal using the generated depth/shadow map  
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		// set light uniforms
		shader.setVec3("viewPos", camera.Position);
		shader.setVec3("lightPos", lightPos);
		shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
			
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		floor.draw(shader);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture);
		renderScene(shader);

		skybox.draw(skyboxShader);

		// ���»����IO�¼�
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//���glfw��ʼ���Ķ���
	glfwTerminate();
	return 0;
}

// renders the 3D scene
// --------------------
void renderScene(const Shader &shader)
{
	Cube   cube;
	// ����������
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	cube.draw(shader, model);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	cube.draw(shader, model);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	cube.draw(shader, model);
}




