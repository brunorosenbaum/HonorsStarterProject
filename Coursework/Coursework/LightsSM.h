#pragma once
#include "BaseShader.h"

//Very basic shader, not crucial but maybe will be needed further on to be modified
//So it's better that it's all set up

class LightsSM :
    public BaseShader
{
private:

	struct ColorBufferType
	{
		XMFLOAT4 lightColor;
	};

public:
	LightsSM(ID3D11Device* device, HWND hwnd);
	~LightsSM();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, XMFLOAT4 color);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:

	ID3D11Buffer* colorBuffer;
	ID3D11Buffer* matrixBuffer;
};

