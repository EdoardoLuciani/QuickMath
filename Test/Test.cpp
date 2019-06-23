#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <stdio.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>

GLuint CompileShaders(bool vs_b, bool tcs_b, bool tes_b, bool gs_b, bool fs_b);

int main(int argc, char *argv[]) {
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(700, 700, "TheTemple", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint shader = CompileShaders(true, false, false, false, true);
	glUseProgram(shader);

	// ---- program start ----

	/*GLfloat* positions = new GLfloat[100*2];

	for (int i = 1; i <= 200; i=i+2) {
		positions[i] = i-1;
		positions[i + 1] = pow(positions[i], 2);
	}*/

	GLfloat positions[] = {	0.0f, 0.0f,
							1.0f, 1.0f,
							2.0f, 4.0f,
							3.0f, 9.8f,
							4.0f, 16.0f,
							5.0f, 25.0f
	};

	float v_max = INT_MIN, v_min = INT_MAX;

	for (int i = 0; i < 12; i++) {
		if (positions[i] > v_max) {
			v_max = positions[i];
		}
		if (positions[i] < v_min) {
			v_min = positions[i];
		}
	}

	for (int i = 0; i < 12; i++) {
		positions[i] = (((positions[i] - v_min) * (1 - 0)) / (v_max - v_min)) + 0;
	}



	GLuint vao;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint buffer;
	glCreateBuffers(1, &buffer);

	glNamedBufferStorage(buffer, 200, positions, 0);
	glVertexArrayVertexBuffer(vao, 0, buffer, 0, sizeof(float) * 2);
	glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao, 0, 0);
	glEnableVertexArrayAttrib(vao, 0);

	glPointSize(2);
	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawArrays(GL_POINTS, 0, 100);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}


GLuint CompileShaders(bool vs_b, bool tcs_b, bool tes_b, bool gs_b, bool fs_b) {

	GLuint shader_programme = glCreateProgram();

	GLuint vs, tcs, tes, gs, fs;

	if (vs_b) {
		FILE* vs_file;
		long vs_file_len;
		char* vertex_shader;

		vs_file = fopen("shaders//vs.glsl", "rb");

		fseek(vs_file, 0, SEEK_END);
		vs_file_len = ftell(vs_file);
		rewind(vs_file);

		vertex_shader = (char*)malloc(vs_file_len + 1);


		fread(vertex_shader, vs_file_len, 1, vs_file);
		vertex_shader[vs_file_len] = '\0';
		fclose(vs_file);

		vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertex_shader, NULL);
		glCompileShader(vs);

		GLint isCompiled = 0;
		glGetShaderiv(vs, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* error = (char*)malloc(maxLength);
			glGetShaderInfoLog(vs, maxLength, &maxLength, error);
			printf("Vertex shader error: ");
			printf(error);
			free(error);
		}

		glAttachShader(shader_programme, vs);
		free(vertex_shader);
	}
	if (tcs_b) {
		FILE* tcs_file;
		long tcs_file_len;
		char* tessellation_control_shader;

		tcs_file = fopen("shaders//tcs.glsl", "rb");

		fseek(tcs_file, 0, SEEK_END);
		tcs_file_len = ftell(tcs_file);
		rewind(tcs_file);

		tessellation_control_shader = (char*)malloc(tcs_file_len + 1);

		fread(tessellation_control_shader, tcs_file_len, 1, tcs_file);
		tessellation_control_shader[tcs_file_len] = '\0';
		fclose(tcs_file);


		tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tcs, 1, &tessellation_control_shader, NULL);
		glCompileShader(tcs);

		GLint isCompiled = 0;
		glGetShaderiv(tcs, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(tcs, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* error = (char*)malloc(maxLength);
			glGetShaderInfoLog(tcs, maxLength, &maxLength, error);
			printf("Tessellation control shader error: ");
			printf(error);
			free(error);
		}

		glAttachShader(shader_programme, tcs);
		free(tessellation_control_shader);
	}
	if (tes_b) {
		FILE* tes_file;
		long tes_file_len;
		char* tessellation_evaluation_shader;

		tes_file = fopen("shaders//tes.glsl", "rb");

		fseek(tes_file, 0, SEEK_END);
		tes_file_len = ftell(tes_file);
		rewind(tes_file);

		tessellation_evaluation_shader = (char*)malloc(tes_file_len + 1);

		fread(tessellation_evaluation_shader, tes_file_len, 1, tes_file);
		tessellation_evaluation_shader[tes_file_len] = '\0';
		fclose(tes_file);

		tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tes, 1, &tessellation_evaluation_shader, NULL);
		glCompileShader(tes);

		GLint isCompiled = 0;
		glGetShaderiv(tes, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(tes, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* error = (char*)malloc(maxLength);
			glGetShaderInfoLog(tes, maxLength, &maxLength, error);
			printf("Tessellation evaluation shader error: ");
			printf(error);
			free(error);
		}

		glAttachShader(shader_programme, tes);
		free(tessellation_evaluation_shader);
	}
	if (gs_b) {
		FILE* gs_file;
		long gs_file_len;
		char* geometry_shader;

		gs_file = fopen("shaders//gs.glsl", "rb");

		fseek(gs_file, 0, SEEK_END);
		gs_file_len = ftell(gs_file);
		rewind(gs_file);

		geometry_shader = (char*)malloc(gs_file_len + 1);

		fread(geometry_shader, gs_file_len, 1, gs_file);
		geometry_shader[gs_file_len] = '\0';
		fclose(gs_file);

		gs = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gs, 1, &geometry_shader, NULL);
		glCompileShader(gs);

		GLint isCompiled = 0;
		glGetShaderiv(gs, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(gs, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* error = (char*)malloc(maxLength);
			glGetShaderInfoLog(gs, maxLength, &maxLength, error);
			printf("Geometry shader error: ");
			printf(error);
			free(error);
		}

		glAttachShader(shader_programme, gs);
		free(geometry_shader);
	}
	if (fs_b) {
		FILE* fs_file;
		long fs_file_len;
		char* fragment_shader;

		fs_file = fopen("shaders//fs.glsl", "rb");

		fseek(fs_file, 0, SEEK_END);
		fs_file_len = ftell(fs_file);
		rewind(fs_file);

		fragment_shader = (char*)malloc(fs_file_len + 1);

		fread(fragment_shader, fs_file_len, 1, fs_file);
		fragment_shader[fs_file_len] = '\0';
		fclose(fs_file);

		fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fragment_shader, NULL);
		glCompileShader(fs);

		GLint isCompiled = 0;
		glGetShaderiv(fs, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* error = (char*)malloc(maxLength);
			glGetShaderInfoLog(fs, maxLength, &maxLength, error);
			printf("Fragment shader error: ");
			printf(error);
			free(error);
		}

		glAttachShader(shader_programme, fs);
		free(fragment_shader);
	}

	glLinkProgram(shader_programme);

	if (vs_b) {
		glDeleteShader(vs);
	}
	if (tcs_b) {
		glDeleteShader(tcs);
	}
	if (tes_b) {
		glDeleteShader(tes);
	}
	if (gs_b) {
		glDeleteShader(gs);
	}
	if (fs_b) {
		glDeleteShader(fs);
	}

	return shader_programme;
}