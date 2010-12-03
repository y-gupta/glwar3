#include "TextureManager.h"


std::string MdxFileName = "AshenBush0.mdx";

int Init ( ESContext *esContext )
{
	Graphics.SetShader();
	ResourceLoader.RegisterAllLoaders();

	BUFFER Buffer;
	if(!FileLoader.LoadFromFile(MdxFileName, Buffer))
	{
		Error.SetMessage("Unable to load \"AshenBush0.mdx\", file does not exist!");
		return 0;
	}
	if(!ResourceLoader.LoadModel(Model, MdxFileName, Buffer)) return 0;

	glClearColor ( 0.5f, 0.5f, 0.5f, 1.0f );

	return TRUE;
}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT);

	glFrontFace(GL_CCW);
	glEnable(GL_BACK);
	glCullFace(GL_BACK);

	INT TimeDifference = 0;
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
