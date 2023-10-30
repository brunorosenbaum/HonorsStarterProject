// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"
#include "TextureSM.h"


class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void drawPlane(XMMATRIX& world, XMMATRIX& view, XMMATRIX& projection);
	void gui();

private:

	//Geometry meshes
	PlaneMesh* plane_mesh_;

	//Shader managers
	TextureSM* textureSM;

};

#endif