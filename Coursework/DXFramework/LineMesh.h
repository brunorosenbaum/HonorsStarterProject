#pragma once
#include "D3D.h"



class LineMesh
{
	
public:
	LineMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void draw(); 

private:
	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	void const* shaderByteCode;
	size_t byteCodeLength;
};

