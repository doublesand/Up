class DepthMap
{
public:
	DepthMap();
	void renderMap(Shader & shader);
	void renderScene(const Shader & shader);
	~DepthMap();

	unsigned int depthMapFBO;     //֡�������
	unsigned int textureID;        //��Ȼ�������
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	glm::mat4 lightProjection, lightView;        //��Դ��͸�Ӿ��󣬹�Դ�Ĺ۲����
	glm::mat4 lightSpaceMatrix;                  //��ռ�任����
	float near_plane = 1.0f, far_plane = 7.5f;   //ͶӰ���������Զ�ľ���
};

DepthMap::DepthMap()
{
	// ����֡��ȶ��󻺴�ʵ����Ӱ
	//���������е�������Щ���ֵ
	glGenFramebuffers(1, &depthMapFBO);  // ����һ��֡�������
	glGenTextures(1, &textureID);         // ������Ȼ�������

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// GL_DEPTH_COMPONENT �����������Ϣ
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };  //����ı߽�����Ϊ1.0����ɫ 
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	
	// ����Ȼ�������Ҳ����֡�������������Ϣ������ depthMap �ϡ�
	// ��ǰ��֡����洢��depthMap��������Ϣ
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);
	// �����������Opengl���ǲ���Ҫ��ɫ���ݣ�ֻҪ�����Ϣ
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);  //���
}

inline void DepthMap::renderMap(Shader & shader)
{
	//ͶӰ��ʽ
	//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));  //��Դ���򳡾����룬�������ǵ�����ǶȾͿ�������һ���������
	lightSpaceMatrix = lightProjection * lightView;  //ͶӰ*�۲�
	// �ӹ����ͼ������Ⱦ����
	shader.use();
	shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	//һ��Ҫ��glViewport�����ܱ�֤�ӿڵ���Ӱ����̫С���߲�����
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);   //����Դ��ͼ�������Ϣ�浽depthMapFBO��
	glClear(GL_DEPTH_BUFFER_BIT);                     //�����Ȼ���

	renderScene(shader);                              //������ɫ��������Ⱦ
	glBindFramebuffer(GL_FRAMEBUFFER, 0);             //�����֡���������н��
	
	// ����ͼ�������Ϣ�㶨���ӿ�Ҫ�л���ԭ�������ӣ���Ⱦʵ��
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline void DepthMap::renderScene(const Shader & shader)
{
	Cube   cube;
	// ����������
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	cube.draw(shader, model);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(4.0f, 0.5f, 2.0));
	model = glm::scale(model, glm::vec3(0.25f));
	cube.draw(shader, model);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	cube.draw(shader, model);
}

DepthMap::~DepthMap()
{
	glDeleteFramebuffers(1, &depthMapFBO);
	glDeleteTextures(1, &textureID);
}