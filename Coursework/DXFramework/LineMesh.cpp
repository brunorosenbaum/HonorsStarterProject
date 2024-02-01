#include "LineMesh.h"

LineMesh::LineMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	m_states = std::make_unique<CommonStates>(device);

	m_effect = std::make_unique<BasicEffect>(device);
	m_effect->SetVertexColorEnabled(true);

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	//DX::ThrowIfFailed(
	//	device->CreateInputLayout(VertexPositionColor::InputElements,
	//		VertexPositionColor::InputElementCount,
	//		shaderByteCode, byteCodeLength,
	//		m_inputLayout.ReleaseAndGetAddressOf()));

	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(deviceContext);

	deviceContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	deviceContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	deviceContext->RSSetState(m_states->CullNone());

	m_effect->Apply(deviceContext);

	deviceContext->IASetInputLayout(m_inputLayout.Get());

	
}

void LineMesh::draw()
{
	m_batch->Begin();

	VertexPositionColor v1(SimpleMath::Vector3(-1.f, -1.0f, 0.5f), Colors::Yellow);
	VertexPositionColor v2(SimpleMath::Vector3(1.0f, 1.0f, 0.5f), Colors::Yellow);

	m_batch->DrawLine(v1, v2);

	m_batch->End();
}
