#pragma once
#include "BaseShader.h"
class TextureSM :
    public BaseShader
{
public:
	TextureSM(ID3D11Device* device, HWND hwnd);
	~TextureSM();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture);

protected:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* samplerState;
};

