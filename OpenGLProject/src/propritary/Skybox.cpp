#include <Skybox.hpp>

Skybox::Skybox() : cubeMapID(0) {

	load_program();

	for (int i = 0;i < faces.size();i++) {
		std::cout << this->faces[i] << "\n";
	}
	model = new Model(vertices_, true);
	model->setup(0, 3, GL_FALSE, 3, (void*)0);
	program.setup(skybox_vertex_shader_source, skybox_fragment_shader_source);
	program.use();
	program.set_uniform_1i("skybox", 0);
	program.stop_using();
}

Skybox::Skybox(const std::vector<std::string>& faces) : cubeMapID(0) {
	
	load_program();
	
	this->faces = faces;
	for (int i = 0;i < faces.size();i++) {
		std::cout << this->faces[i] << "\n";
	}
	model = new Model(vertices_, true);
	model->setup(0, 3, GL_FALSE, 3, (void*)0);
	program.setup(skybox_vertex_shader_source,skybox_fragment_shader_source);
	program.use();
	program.set_uniform_1i("skybox",0);
	program.stop_using();
}

Skybox::Skybox(const std::vector<std::string>& faces, std::string vertex_shader, std::string fragment_shader) {
	load_program();

	skybox_vertex_shader_source = vertex_shader;
	skybox_fragment_shader_source = fragment_shader;

	model = new Model(vertices_, true);
	model->setup(0, 3, GL_FALSE, 3, (void*)0);
	program.setup(skybox_vertex_shader_source, skybox_fragment_shader_source);
}

Skybox::~Skybox() {
	delete model;
}

void Skybox::load_program() {
	stbi_set_flip_vertically_on_load(false);
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	
	int width, height, nrChannels;
	
	for (unsigned int i = 0;i < faces.size();i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
				width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			std::cout << "Cubemap succesfully loaded" << faces[i] << std::endl;

		}
		else
		{
			std::cout << "Cubemap failed to load at path:" << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	cubeMapID = textureID;
}

void Skybox::draw_skybox(glm::mat4 projection,glm::mat4 view) {
	GLint stencilTestEnabled;
	glGetIntegerv(GL_STENCIL_TEST, &stencilTestEnabled);

	if (stencilTestEnabled == GL_TRUE) {
		glDisable(GL_STENCIL_TEST);
	}
	glDepthFunc(GL_LEQUAL);  // Change depth function to allow skybox rendering
	program.use();
	model->use_VAO();
	model->draw_triangles(0,36);
	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);
	program.set_uniform_mat_4fv("projection", projection);
	program.set_uniform_mat_4fv("view", view);
	program.stop_using();
	model->clear_VAO();
	glDepthFunc(GL_LESS); // Restore default depth function
}