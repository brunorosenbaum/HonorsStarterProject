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
	parent_segment_ = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	child_segment_1 = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	child_segment_2 = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());

	//Initialize lights
	lights_[0] = new Light(); //DIRECTIONAL LIGHT
	lights_[0]->setAmbientColour(0.1f, 0.1f, 0.1f, 1.0f);
	lights_[0]->setDiffuseColour(1.0f, 0.9f, 1.0f, 1.0f);
	lights_[0]->setDirection(0, -1, 0.5);
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

	rotX = rand() % 33 + (-degrees); //32 Possible values between 16 and -16
	rotZ = rand() % 33 + (-degrees); //32 Possible values between 16 and -16
	

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

float App1::InvLerp(float a, float b, float v)
{ //inverse lerp returns a fraction t, based on a value between a and b
	return (v - a) / (b - a);
}

float App1::Lerp(float a, float b, float t)
{//Lerp returns a blend between a and b, based on a fraction t
	return (1.0f - t) * a + b*t;
}

float App1::Remap(float iMin, float iMax, float oMin, float oMax, float v)
{ //Remap takes a value within a given input range into a given output range, which is basically a combined inverse lerp and lerp
	float t = InvLerp(iMin, iMax, v);
	return Lerp(oMin, oMax, t); 

}

void App1::drawObjects(XMMATRIX& world, XMMATRIX& view, XMMATRIX& projection)
{
	//Cube, sphere
	directional_light_sphere_->sendData(renderer->getDeviceContext());
	XMMATRIX pointLightPos = XMMatrixTranslation(directional_position_[0], directional_position_[1], directional_position_[2]);
	lightsSM->setShaderParameters(renderer->getDeviceContext(), world * pointLightPos, view, projection, lights_);
	lightsSM->render(renderer->getDeviceContext(), directional_light_sphere_->getIndexCount());

	cube_mesh_->sendData(renderer->getDeviceContext());
	lightsSM->setShaderParameters(renderer->getDeviceContext(), XMMatrixTranslation(10, 1, 10) * world, view, projection, lights_);
	lightsSM->render(renderer->getDeviceContext(), cube_mesh_->getIndexCount());

	//Lightning dummy geometry ---------------------------------------------------------------------------------------------------------
	XMMATRIX scale = XMMatrixIdentity(); //Matrix for scaling cubes into long segments
	scale *= XMMatrixScaling(1, 10, 1);

	const XMFLOAT3 parentOrigin = { 10, 50, 20 }; //Sets parent segment's origin
	XMMATRIX parentPosMatrix = XMMatrixTranslation(parentOrigin.x, parentOrigin.y, parentOrigin.z);

	XMMATRIX parentM =  scale; 
	//Parent segment
	parent_segment_->sendData(renderer->getDeviceContext());
	lightsSM->setShaderParameters(renderer->getDeviceContext(), world * parentM * parentPosMatrix, view, projection, lights_);
	lightsSM->render(renderer->getDeviceContext(), parent_segment_->getIndexCount());


	//Convert to radians
	float xRot = XMConvertToRadians(rotX);
	float zRot = XMConvertToRadians(rotZ);

	float zTilt = Remap(XMConvertToRadians(-degrees), XMConvertToRadians(degrees), -2.5, 2.5, xRot); 
	float xTilt = Remap(XMConvertToRadians(-degrees), XMConvertToRadians(degrees), -2.5, 2.5, -zRot); 
	


	/*XMMATRIX localTransform = XMMatrixTranslation(0, 0, 0);
	parentM *= localTransform;*/

	//Translates next segment into lower end of parent segment
	XMMATRIX endTranslation = XMMatrixTranslation(0,-20, 0);
	parentM *= endTranslation;

	//Angle rot
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(xRot, 0, zRot);
	parentM *= rotation; 

	XMMATRIX tilt = XMMatrixTranslation(xTilt, 0, zTilt);
	parentM *= tilt;

	
	//Child 1
	XMMATRIX child1Pos = parentM; 
	child_segment_1->sendData(renderer->getDeviceContext());
	lightsSM->setShaderParameters(renderer->getDeviceContext(), world * child1Pos * parentPosMatrix, view, projection, lights_);
	lightsSM->render(renderer->getDeviceContext(), child_segment_1->getIndexCount());

	//Child 2
	//parentM *= localTransform;
	parentM *= endTranslation; 
	parentM *= rotation;
	parentM *= tilt; 

	XMMATRIX child2Pos = parentM; 
	child_segment_2->sendData(renderer->getDeviceContext());
	lightsSM->setShaderParameters(renderer->getDeviceContext(), world * child2Pos * parentPosMatrix, view, projection, lights_);
	lightsSM->render(renderer->getDeviceContext(), child_segment_2->getIndexCount());
}



XMMATRIX App1::transformToSegment(XMMATRIX worldMatrix) //Transforms cubes into homogenous segments
{
	XMMATRIX transformMatrix = XMMatrixIdentity();
	transformMatrix *= XMMatrixScaling(0.1, 5, 0.1);
	return worldMatrix * transformMatrix; 
}

XMMATRIX App1::transformChildSegment(XMMATRIX parentPos)
{
	//Stores generated angles at init in local vars
	float xRot = rotX;
	float zRot = rotZ;

	//Convert to radians
	xRot = XMConvertToRadians(xRot);
	zRot = XMConvertToRadians(zRot);

	////Assign radians to new variables
	//float zTilt = xRot;
	//float xTilt = zRot;
	//float temp = XMConvertToRadians(90);
	////Essentially if the tilt is negative (-0.7), translate 0.3 in the z axis, if it's positive (0.7), translate -0.3
	///*xTilt = (xTilt < 0) ? (xTilt + 1) : (1 - xTilt); 
	//zTilt = (zTilt < 0) ? (zTilt + 1) : (1 - zTilt);*/
	//xTilt = (xTilt < 0) ? (1 + xTilt) : (1 - xTilt);
	//zTilt = (zTilt < 0) ? (1 + zTilt) : (1 - zTilt);
	//XMMATRIX tiltTranslationMatrix = XMMatrixTranslation(xTilt, 0, zTilt); //0.83 Since it's 1 - 0.17 (tilts forward so it matches the parents end)

	//Scaling
	XMMATRIX scale = XMMatrixIdentity(); //Matrix for scaling cubes into long segments
	scale *= XMMatrixScaling(0.1, 5, 0.1);

	//Translates next segment into lower end of parent segment
	XMMATRIX endTranslation = XMMatrixTranslation(0, -10, 0);

	//Angle rot
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(xRot, 0, zRot);

	
	//parentPos *= tiltTranslationMatrix; 
	/*tiltTranslationMatrix *= parentPos; */
	XMMATRIX finalM = parentPos * scale * rotation * endTranslation; 

	return finalM; 

}

XMMATRIX App1::translateChild(/*XMMATRIX endParent*/)
{
	XMMATRIX translationMatrix = XMMatrixTranslation(0, -5, 0);
	/*endParent *= translationMatrix;*/
	return translationMatrix; 

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
	ImGui::SliderFloat("X Angle", &rotX, -degrees, degrees, "%f"); 
	ImGui::SliderFloat("Z Angle", &rotZ, -degrees, degrees, "%f"); 

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

