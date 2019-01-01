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
	Shader modelShader("model.vs", "model.fs");
	Shader lampShader("lamp.vs", "lamp.fs");

	// ������ɫ��
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// ������Ӱ��ɫ��
	shader.use();
	shader.setInt("diffuseTexture", 0);   //������ͼ
	shader.setInt("shadowMap", 1);        //��Ӱ��ͼ

	modelShader.use();
	modelShader.setInt("material.diffuse", 0);

	Skybox   skybox;       //���
	Floor    floor;        //�ذ�
	DepthMap depthMap;     // ��Դ��ͼ�������������Ӱ
	Model nanosuit("resources/objects/nanosuit/nanosuit.obj");
	Model house("resources/objects/house/house.obj");
	Model dog("resources/objects/dog/12228_Dog_v1_L2.obj");
	Model balloon("resources/objects/balloon/11809_Hot_air_balloon_l2.obj");
	Model tree("resources/objects/tree/12150_Christmas_Tree_V2_L2.obj");

	// ������������
	unsigned int woodTexture = loadTexture("resources/textures/wood.png");
	//�������ͼ����
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

		// ��ʱ��仯
		//lightPos.x = sin(glfwGetTime()) * 3.0f;
		//lightPos.z = cos(glfwGetTime()) * 2.0f;
		//lightPos.y = 5.0 + cos(glfwGetTime()) * 1.0f;

		// ��Ⱦ
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�����Ȼ������ɫ����

		// �õ���Դ��ͼ�������Ϣ����Ϊ���������ͼ���� ��Ӱ���ơ�
		depthMap.renderMap(simpleDepthShader);  //�����������
		
												
		//------------------------��shader��һЩ����
		shader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		//����ɫ�������������������Դ�������Լ���ռ����
		shader.setVec3("viewPos", camera.Position);
		shader.setVec3("lightPos", lightPos);
		shader.setMat4("lightSpaceMatrix", depthMap.lightSpaceMatrix);
			
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap.textureID);

		floor.draw(shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture);
		depthMap.renderScene(shader);      //��Ⱦ����



		 // ----------------------------------------��ȾOBJ�ļ�
		modelShader.use();
		modelShader.setVec3("light.position", lightPos);
		modelShader.setVec3("viewPos", camera.Position);

		// light properties
		modelShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		modelShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		modelShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		// material properties
		modelShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		modelShader.setFloat("material.shininess", 32.0f);


		// view/projection transformations
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("view", view);

		// ��Ⱦ������
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.5f, -0.5f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		modelShader.setMat4("model", model);
		nanosuit.Draw(modelShader);

		// ��Ⱦ����
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(20.0f, -0.5f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
		modelShader.setMat4("model", model);
		house.Draw(modelShader);

		// ��ȾС��
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, -0.5f, 11.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		modelShader.setMat4("model", model);
		dog.Draw(modelShader);

		// ��Ⱦʥ����
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(20.0f, -0.5f, 24.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		modelShader.setMat4("model", model);
		tree.Draw(modelShader);

		// ��Ⱦ������
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-30.0f, -0.5f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		modelShader.setMat4("model", model);
		balloon.Draw(modelShader);

		//---------------------------------���ƻ�����
		// ���ƻ�����
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lampShader.setMat4("model", model);
		Cube lamp;
		lamp.draw(lampShader, model);

		//---------------------------------��Ⱦ���ͼ
		skybox.draw(skyboxShader);         

		// ���»����IO�¼�
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//���glfw��ʼ���Ķ���
	glfwTerminate();
	return 0;
}
