// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"
#include "TextureSM.h"
#include "LightsSM.h"

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
	void drawObjects(XMMATRIX& world, XMMATRIX& view, XMMATRIX& projection);
	XMMATRIX transformToSegment(XMMATRIX worldMatrix);
	void gui();

private:

	//Geometry meshes
	PlaneMesh* plane_mesh_;
	SphereMesh* directional_light_sphere_;
	CubeMesh* cube_mesh_;
	CubeMesh* segment_mesh_;

	//Lights
	Light* lights_[2]; //Directional 1, point 2
	float light_direction_[3];
	float directional_position_[3]; 

	//Shader managers
	TextureSM* textureSM;
	LightsSM* lightsSM; 
};

#endif