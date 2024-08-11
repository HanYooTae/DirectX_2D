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
	ID3D11Device* device = nullptr;							// DX에서 사용할 자원 생성
	ID3D11DeviceContext* device_context = nullptr;			// 파이프라인에 연결시켜주는 것
	IDXGISwapChain* swap_chain = nullptr;					// 백버퍼 관리
	ID3D11RenderTargetView* render_target_view = nullptr;	// 도화지
	D3D11_Viewport viewport = D3D11_Viewport::Undefined_viewport;	// 보여질(그려질)영역
	D3DXCOLOR clear_color = 0xff555566;//RGBA				// 화면을 지울 때 화면을 초기화할 색
	// 0 ~ 255 / 255 -> 0 ~ 1
};

/*
	I -> Com Interface 
			- 직접 메모리에 할당할 수 없고 할당을 요청하기 때문에
			Desc(설명서)가 필요하다.

			- Create로 생성을 요청, Release로 삭제를 요청

		DXGI
			- DirectX 그래픽 하부구조, 그래픽카드가 제조사마다 다르기 때문에
			프로그래머가 쉽게 사용할 수 있또록 해주는 것

		SwapChain
			- 더블 버퍼링에 사용한다, 백버퍼를 관리한다.

		RenderTargetView 
			- 도화지, 렌더링 대상이 되는 버퍼

		VIEWPORT
			- 보여질 영역

		COLOR : R G B A(투명도), 0 ~ 255 범위를 0 ~ 1(정규화)하여 관리한다.
*/

// Rendering Pipeline : 렌더링 하기 위한 수행 단계
// IA - VS - RS - PS - OM

