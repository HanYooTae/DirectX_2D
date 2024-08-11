#pragma once
#include "IComponent.h"

enum class MeshType : uint
{
	Quad,
};

class MeshRendererComponent final : public IComponent
{
public:
	using IComponent::IComponent;
	~MeshRendererComponent() = default;

	// Inherited via IComponent
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	//=============================================
	//  [Mesh] -> 형체 결정
	//=============================================
	// 리턴값만 있음
	std::shared_ptr<D3D11_VertexBuffer> GetVertexBuffer() const { return vertex_buffer; }
	std::shared_ptr<D3D11_IndexBuffer> GetIndexBuffer() const { return index_buffer; }
	// 데이터 만들기
	void SetStandardMesh();

	//=============================================
	//  [Material] - 재질 결정
	//=============================================
	std::shared_ptr<D3D11_Shader> GetVertexShader() const { return vertex_shader; }
	std::shared_ptr<D3D11_Shader> GetPixelShader() const { return pixel_shader; }
	std::shared_ptr<D3D11_InputLayout> GetInputLayout() const { return input_layout; }
	void SetStandardMaterial();


private:
	MeshType mesh_type = MeshType::Quad;

	std::shared_ptr<D3D11_VertexBuffer> vertex_buffer;
	std::shared_ptr<D3D11_IndexBuffer> index_buffer;

	std::shared_ptr<D3D11_InputLayout> input_layout;
	
	std::shared_ptr<D3D11_Shader> vertex_shader;
	std::shared_ptr<D3D11_Shader> pixel_shader;

};

