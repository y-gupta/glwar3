#include "Misc.h"

GLuint g_simpleProgram;

int Init ( ESContext *esContext )
{
	glViewport(0, 0, esContext->width, esContext->height);

	g_simpleProgram = esLoadProgramFromFile("Simple.vert", "Simple.frag");
	
	glUseProgram(g_simpleProgram);

	return true;
}

void Draw ( ESContext *esContext )
{
	glClearColor ( 0.5f, 0.5f, 0.5f, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );
	

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update(ESContext *esContext, float deltaTime)
{

}

int main ( int argc, char *argv[] )
{
	ESContext esContext;

	esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", 320, 240, ES_WINDOW_RGB );

	if ( !Init ( &esContext ) )
		return 0;

	esRegisterDrawFunc(&esContext, Draw);
	esRegisterUpdateFunc(&esContext, Update);
	esMainLoop ( &esContext );
}
