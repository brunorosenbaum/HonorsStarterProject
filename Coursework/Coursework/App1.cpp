// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	plane_mesh_ = nullptr; 

}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	//Load textures
	textureMgr->loadTexture(L"planeTxr", L"res/plane.png"); 

	// Initalise scene geometry meshes.
	plane_mesh_ = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());

	//Initialize shader managers
	textureSM = new TextureSM(renderer->getDevice(), hwnd);


}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (plane_mesh_) { delete plane_mesh_; plane_mesh_ = 0; }
}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.19f, 0.03f, 0.36f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	//Render geometry
	drawPlane(worldMatrix, viewMatrix, projectionMatrix); 
	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();

	return true;
}

void App1::drawPlane(XMMATRIX& world, XMMATRIX& view, XMMATRIX& projection)
{
	plane_mesh_->sendData(renderer->getDeviceContext());
	textureSM->setShaderParameters(renderer->getDeviceContext(), world, view, projection, textureMgr->getTexture(L"planeTxr"));
	textureSM->render(renderer->getDeviceContext(), plane_mesh_->getIndexCount());

}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

