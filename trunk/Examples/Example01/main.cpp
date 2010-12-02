#include "TextureManager.h"

GLuint programObject;

int Init ( ESContext *esContext )
{
	ResourceLoader.RegisterAllLoaders();

	if(!TextureManager.Load("Textures/AshenNatural.blp")) return 0;

	BUFFER Buffer;
	if(!FileLoader.LoadFromFile("AshenBush0.mdx", Buffer))
	{
		Error.SetMessage("Unable to load \"AshenBush0.mdx\", file does not exist!");
		return 0;
	}
	if(!ResourceLoader.LoadModel(Model, "AshenBush0.mdx", Buffer)) return 0;

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

	INT TimeDifference;
	Model.Render(TimeDifference);


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
