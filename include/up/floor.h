class Floor
{
public:
	Floor();
	void draw(Shader &shader);
	~Floor();

private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int textureID;

	float vertices[48] = {
		// λ����Ϣ             //��������y������// ��������
		 1000.0f, -0.5001f,  1000.0f,  0.0f, 1.0f, 0.0f,  1000.0f,  0.0f,
		-1000.0f, -0.5001f,  1000.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-1000.0f, -0.5001f, -1000.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1000.0f,

		 1000.0f, -0.5001f,  1000.0f,  0.0f, 1.0f, 0.0f,  1000.0f,   0.0f,
		-1000.0f, -0.5001f, -1000.0f,  0.0f, 1.0f, 0.0f,   0.0f,  1000.0f,
		 1000.0f, -0.5001f, -1000.0f,  0.0f, 1.0f, 0.0f,  1000.0f, 1000.0f
	};
};

Floor::Floor()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);  //�Ƚ��VAO�����ɺ�ϰ��
	// ����ذ�����
	textureID = loadTexture("resources/textures/caodi.jpg");
}

inline void Floor::draw(Shader & shader)
{
	// ���Ƶذ�
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);                     //��������
	glBindTexture(GL_TEXTURE_2D, textureID);        //���������
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

Floor::~Floor()
{
	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}