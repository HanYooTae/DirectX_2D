#pragma once

// Graphics.h
#include"Core/D3D11/Rasterizer/D3D11_Viewport.h"
#include "ISubsystem.h"


class Graphics final : public ISubsystem
{
public:
	Graphics(class Context* context);
	~Graphics();

	virtual bool Initialize() override;
	virtual void Update() override;
	virtual void CreateBackBuffer(const uint& width, const uint& height);

	ID3D11Device* GetDevice() { return device; }
	ID3D11DeviceContext* GetDeviceContext() { return device_context; }

	void Begin();
	void End();

private:
	ID3D11Device* device = nullptr;							// DX���� ����� �ڿ� ����
	ID3D11DeviceContext* device_context = nullptr;			// ���������ο� ��������ִ� ��
	IDXGISwapChain* swap_chain = nullptr;					// ����� ����
	ID3D11RenderTargetView* render_target_view = nullptr;	// ��ȭ��
	D3D11_Viewport viewport = D3D11_Viewport::Undefined_viewport;	// ������(�׷���)����
	D3DXCOLOR clear_color = 0xff555566;//RGBA				// ȭ���� ���� �� ȭ���� �ʱ�ȭ�� ��
	// 0 ~ 255 / 255 -> 0 ~ 1
};

/*
	I -> Com Interface 
			- ���� �޸𸮿� �Ҵ��� �� ���� �Ҵ��� ��û�ϱ� ������
			Desc(����)�� �ʿ��ϴ�.

			- Create�� ������ ��û, Release�� ������ ��û

		DXGI
			- DirectX �׷��� �Ϻα���, �׷���ī�尡 �����縶�� �ٸ��� ������
			���α׷��Ӱ� ���� ����� �� �ֶǷ� ���ִ� ��

		SwapChain
			- ���� ���۸��� ����Ѵ�, ����۸� �����Ѵ�.

		RenderTargetView 
			- ��ȭ��, ������ ����� �Ǵ� ����

		VIEWPORT
			- ������ ����

		COLOR : R G B A(����), 0 ~ 255 ������ 0 ~ 1(����ȭ)�Ͽ� �����Ѵ�.
*/

// Rendering Pipeline : ������ �ϱ� ���� ���� �ܰ�
// IA - VS - RS - PS - OM

