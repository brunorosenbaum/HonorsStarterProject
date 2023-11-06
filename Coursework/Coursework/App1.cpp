// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	plane_mesh_ = nullptr; 
	directional_light_sphere_ = nullptr;
	cube_mesh_ = nullptr; 
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	//Load textures
	textureMgr->loadTexture(L"planeTxr", L"res/plane.png"); 

	// Initalise scene geometry meshes.
	plane_mesh_ = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	directional_light_sphere_ = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	cube_mesh_ = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	segment_mesh_ = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());

	//Initialize lights
	lights_[0] = new Light(); //DIRECTIONAL LIGHT
	lights_[0]->setAmbientColour(0.1f, 0.1f, 0.1f, 1.0f);
	lights_[0]->setDiffuseColour(1.0f, 0.9f, 1.0f, 1.0f);
	lights_[0]->setDirection(0, -1, 0);
	lights_[0]->setPosition(10, 10, 10);
		//Light direction and position for IMGUI
		light_direction_[0] = lights_[0]->getDirection().x; 
		light_direction_[1] = lights_[0]->getDirection().y; 
		light_direction_[2] = lights_[0]->getDirection().z;

		directional_position_[0] = lights_[0]->getPosition().x;
		directional_position_[1] = lights_[0]->getPosition().y;
		directional_position_[2] = lights_[0]->getPosition().z;

	lights_[1] = new Light();
	lights_[1]->setAmbientColour(0.1f, 0.1f, 0.1f, 1.0f);
	lights_[1]->setDiffuseColour(0.0f, 0.0f, 1.0f, 1.0f);
	lights_[1]->setPosition(10, 10, 10);

		directional_position_[0] = lights_[1]->getPosition().x;
		directional_position_[1] = lights_[1]->getPosition().y;
		directional_position_[2] = lights_[1]->getPosition().z;

	//Initialize shader managers
	textureSM = new TextureSM(renderer->getDevice(), hwnd);
	lightsSM = new LightsSM(renderer->getDevice(), hwnd); 


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
	
	drawObjects(worldMatrix, viewMatrix, projectionMatrix);
	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();

	return true;
}

void App1::drawPlane(XMMATRIX& world, XMMATRIX& view, XMMATRIX& projection)
{
	plane_mesh_->sendData(renderer->getDeviceContext());
	textureSM->setShaderParameters(renderer->getDeviceContext(), world, view, projection, textureMgr->getTexture(L"planeTxr"), lights_);
	textureSM->render(renderer->getDeviceContext(), plane_mesh_->getIndexCount());

}

void App1::drawObjects(XMMATRIX& world, XMMATRIX& view, XMMATRIX& projection)
{
	directional_light_sphere_->sendData(renderer->getDeviceContext());
	XMMATRIX pointLightPos = XMMatrixTranslation(directional_position_[0], directional_position_[1], directional_position_[2]);
	lightsSM->setShaderParameters(renderer->getDeviceContext(), world * pointLightPos, view, projection, lights_);
	lightsSM->render(renderer->getDeviceContext(), directional_light_sphere_->getIndexCount());

	cube_mesh_->sendData(renderer->getDeviceContext());
	lightsSM->setShaderParameters(renderer->getDeviceContext(), XMMatrixTranslation(10, 1, 10) * world, view, projection, lights_);
	lightsSM->render(renderer->getDeviceContext(), cube_mesh_->getIndexCount());

	segment_mesh_->sendData(renderer->getDeviceContext());
	lightsSM->setShaderParameters(renderer->getDeviceContext(), transformToSegment(world), view, projection, lights_);
	lightsSM->render(renderer->getDeviceContext(), segment_mesh_->getIndexCount());
}

XMMATRIX App1::transformToSegment(XMMATRIX worldMatrix) //Transforms cubes into homogenous segments
{
	XMMATRIX transformMatrix = XMMatrixIdentity();
	transformMatrix *= XMMatrixScaling(0.1, 5, 0.1);
	return worldMatrix * transformMatrix; 
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

	if (ImGui::TreeNode("Directional light config")) {
		ImGui::SliderFloat3("Direction", light_direction_, -1, 1, "%.3f", 1);
		lights_[0]->setDirection(light_direction_[0], light_direction_[1], light_direction_[2]);

		ImGui::SliderFloat3("Position", directional_position_, -20, 20, "%.3f", 1);
		lights_[1]->setPosition(directional_position_[0], directional_position_[1], directional_position_[2]);
		ImGui::TreePop();

	}

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

