#include "TextureManager.h"

GLuint programObject;
TEXTURE *Texture;

struct Vertex
{
	float p[3];
	float t[2];
};

Vertex vertices[] = {
	{ {-0.5f, 0.5f, 0.0f},	{1.0f, 0.0f}},
	{ {-0.5f, -0.5f, 0.0f},	{1.0f, 1.0f}},
	{ {0.5f, -0.5f, 0.0f},	{0.0f, 1.0f}},
	{ {0.5f, 0.5f, 0.0f},	{0.0f, 0.0f}}
};

GLshort indices[] = { 0, 1, 2, 0, 2, 3};

int Init ( ESContext *esContext )
{
	ResourceLoader.RegisterAllLoaders();

	if(!TextureManager.Load("Abomination.blp")) return 0;
	Texture = TextureManager.GetTexture("Abomination.blp");

	BUFFER Buffer;
	if(!FileLoader.LoadFromFile("Abomination.mdx", Buffer))
	{
		Error.SetMessage("Unable to load \"Abomination.mdx\", file does not exist!");
		return 0;
	}
	if(!ResourceLoader.LoadModel(Model, "Abomination.mdx", Buffer)) return 0;

	programObject = esLoadProgramFromFile("Simple.vert", "Simple.frag");
	glUseProgram(programObject);

	glClearColor ( 0.5f, 0.5f, 0.5f, 1.0f );

	return TRUE;
}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT);

	glFrontFace(GL_CCW);
	glEnable(GL_BACK);
	glCullFace(GL_BACK);
	glEnable(GL_TEXTURE_2D);

	GLuint positionSlot = glGetAttribLocation(programObject, "Position");
	glEnableVertexAttribArray(positionSlot);
	glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &vertices->p[0]);

	GLuint texcoordSlot = glGetAttribLocation(programObject, "TexCoord");
	glEnableVertexAttribArray(texcoordSlot);
	glVertexAttribPointer(texcoordSlot, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &vertices->t[0]);

	GLuint sampler = glGetUniformLocation(programObject, "Sampler");
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(sampler, 0);

	glBindTexture(GL_TEXTURE_2D, Texture->GetTextureId());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

	glDisable(GL_TEXTURE_2D);

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

int main ( int argc, char *argv[] )
{
	ESContext esContext;

	esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", 800, 600, ES_WINDOW_RGB );

	if ( !Init ( &esContext ) )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );

	esMainLoop ( &esContext );
}
