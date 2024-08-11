// Graphics.cpp
#include "stdafx.h"
#include "Graphics.h"

Graphics::Graphics(Context* context)
	:ISubsystem(context)
{
}

Graphics::~Graphics()
{
	SAFE_RELEASE(render_target_view);
	SAFE_RELEASE(device);
	SAFE_RELEASE(device_context);
	SAFE_RELEASE(swap_chain);
}

bool Graphics::Initialize()
{
	// 디바이스, 디바이스 컨텍스트, 스왑체인 생성
	{
		// DXGI_SWAP_CHAIN_DESC : 스왑체인을 만들기 위한 설명서
		DXGI_SWAP_CHAIN_DESC desc;
		ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
		desc.BufferDesc.Width	= 0; // 스왑체인이 관리할 버퍼의 크기(너비)
		desc.BufferDesc.Height	= 0; // 스왑체인이 관리할 버퍼의 크기(높이)


		/*
			RefreshRate : 화면 주사율, 화면을 어느 주기로 갱신할 것인가?
			Numerator   : 분자
			Denominator : 분모

			60/1 => 1초당 60번 화면을 갱신 하겠다. => 60프레임
		*/
		desc.BufferDesc.RefreshRate.Numerator;
		desc.BufferDesc.RefreshRate.Denominator;
		

		/*
			RGBA : 색상 채널을 의미, 무조건 색상 데이터를 담을 필요는 없다.
			R8G8B8A8 : BPS, Color Depth라고 부르며 
			각각의 채널에 얼만큼의 메모리를 할당할지를 의미한다.
			각 채널이 8비트이니 각 채널당 0 ~ 255의 범위를 가진다.

			UNORM : Unsigned Normalize
			0 ~ 1 로 정규화된 데이터로 쓰겠다
			=> 4 바이트 크기(각 채널당 8비트 * 4)를 가지는 정규화된 데이터를 담겠다.

			과제 : BPC 조사
		*/

		// 우리가 만든 버퍼가 어떤 형태의 데이터를 담을것인가?
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// Scanline : 화면을 출력할 때 한줄씩 그어서 출력하는 것
		// UNSPECIFIED : 지정하지 않음
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // 화면 축소/확대할 때 어떤 효과를 줄 것인가?

		desc.BufferCount = 1; // 버퍼의 개수
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 버퍼를 어떻게 사용할 것인가

		/*
			Aliasing : 픽셀 표현으로 인해 확대하면 외곽선이 계단처럼 보이는 현상

			Anti Aliasing(AA) - 계단 현상을 제거하는 방법
				ㅁ Super Sampling(SSAA)
					ㄴ 샘플링 배수만큼 해상도를 확대한 후에 Down Sampling(해상도를 줄여 색을 섞는다)을 하는 방식
					ㄴ 많은 연산량과 메모리가 필요하다

				ㅁ Multi Sampling(MSAA)
					ㄴ 많은 연산량과 메모리를 요규하는 SSAA를 개량해서 만든 방법
					ㄴ 폴리곤의 가장자리만 AA를 진행

			// 과제 : 안티 앨리어싱 조사하기
		*/
		desc.SampleDesc.Count = 1;		// 샘플의 개수, 배수
		desc.SampleDesc.Quality = 0;	// 샘플링의 퀄리티 수준, 0은 안하겠다는 의미


		desc.OutputWindow = Settings::Get().GetWindowHandle(); // 출력할 윈도우 핸들
		desc.Windowed = TRUE; // 창모드 여부

		// DISCARD : 스왑하여 나온 데이터를 폐기하겠다.
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 스왑시 어떤 효과를 줄 것인가?

		// FEATURE_LEVEL : 기능 수준, DX 몇 버전을 쓸 것인가?
		std::vector<D3D_FEATURE_LEVEL> feature_levels
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};

		// HRESULT : 함수 결과의 성공여부를 담는 자료형
		// S_OK(성공), S_FALSE(실패)를 반환받는다.
		HRESULT hr = D3D11CreateDeviceAndSwapChain // 디바이스, 디바이스 컨텍스트, 스왑체인 생성 함수
		(
			nullptr,					// 비디오 카드를 지정해서 사용할 때 사용
			D3D_DRIVER_TYPE_HARDWARE,	// 디바이스 타입, 하드웨어 가속을 사용하기 위해 HARDWARE로 지정
			nullptr,					// 다른 소프트웨어를 추가적으로 넣을것인가?
			0,							// 디바이스를 만들 때 어떤 기능을 추가적으로 넣을것인가?
			feature_levels.data(),		// 피처 레벨 배열의 시작 주소
			feature_levels.size(),		// 피처 레벨 카운트
			D3D11_SDK_VERSION,			// 사용하는 SDK 버전
			&desc,						// 사용할 desc 주소
			&swap_chain,				// 생성된 스왑체인을 할당받을 위치
			&device,					// 생성된 디바이스를 할당받을 위치
			nullptr,					// 선택된 기능레벨을 반환받는다.
			&device_context				// 생성된 디바이스 컨텍스트를 할당받을 위치
		);
		assert(SUCCEEDED(hr)); // SUCCEEDED 성공시 true, 실패시 false 리턴

	}

	return true;
}

void Graphics::Update()
{
}

void Graphics::CreateBackBuffer(const uint& width, const uint& height)
{

	// RTV 생성
	// ResizeBuffers : 스왑 체인의 버퍼 정보를 변경하는 함수
	// 변경하지 않을 정보는 기본값을 넣어준다.
	HRESULT hr = swap_chain->ResizeBuffers
	(
		0,
		width,
		height,
		DXGI_FORMAT_UNKNOWN,
		0
	);
	assert(SUCCEEDED(hr));

	ID3D11Texture2D* back_buffer = nullptr;

	/*
		Interface Identifier(IID)
			- 인터페이스를 구분하기 위한 식별자이다.
			- GUID 값중 하나

		Globally Unique Identifier(GUID)
			- MS에서 사용하는 여러 항목을 구분하기 위한 식별자
			- 16바이트의 크기를 가진다.
			- UUID라고도 있는데 차이가 없다 생각해도 된다.

		과제 : GUID, UUID 조사
	*/

	hr = swap_chain->GetBuffer // 스왑체인 내부의 버퍼를 꺼낸다.
	(
		0, // 꺼낼 버퍼의 인덱스 번호
		__uuidof(ID3D11Texture2D), // 백 버퍼를 받는 인터페이스의 타입
		reinterpret_cast<void**>(&back_buffer) // 반환된 인터페이스를 받는 변수 포인터
	);
	assert(SUCCEEDED(hr));

	hr = device->CreateRenderTargetView
	(
		back_buffer, // 뷰에서 접근할 리소스, 해당 리소스를 사용하여  RTV만든다.
		nullptr, // RTV desc, nullptr일시 리소스의 형식을 따른다.
		&render_target_view // RTV 받을 변수
	);
	assert(SUCCEEDED(hr));

	// 뷰포트 초기화
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(width);
	viewport.height = static_cast<float>(height);
	viewport.min_depth = 0.0f;
	viewport.max_depth = 1.0f;

	SAFE_RELEASE(back_buffer);
}


// IA VS RS PS OM
void Graphics::Begin()
{

	/*
		Output Merger Stage(OM) - 출력 병합 단계
		- 그림을 그릴 렌더 타겟을 지정하고, 색을 어떻게 섞을것인가(블렌딩)을 지정한다.
		- OM에 세팅될 수 있는 렌더 타겟의 개수는 8개이다. -> 8장을 동시에 그릴 수 있다.
	*/
	device_context->OMSetRenderTargets(1, &render_target_view, nullptr);


	D3D11_VIEWPORT d3d11_viewport;

	d3d11_viewport.TopLeftX = viewport.x;
	d3d11_viewport.TopLeftY = viewport.y;
	d3d11_viewport.Width = viewport.width;
	d3d11_viewport.Height = viewport.height;
	d3d11_viewport.MinDepth = viewport.min_depth;
	d3d11_viewport.MaxDepth = viewport.max_depth;

	device_context->RSSetViewports(1, &d3d11_viewport);

	device_context->ClearRenderTargetView(render_target_view, clear_color);

}

void Graphics::End()
{
	HRESULT hr = swap_chain->Present(1, 0); // Present() : 후면 버퍼를 전면 버퍼로 출력한다.
	assert(SUCCEEDED(hr));
}
 
/*
	ID3D11Resource
		- DX에서 제공하는 자원들이 상속받는 클래스
		- 크게 Buffer, Texture로 나뉜다.

	ID3D11Buffer
		- 사용자가 구조체를 만들어 GPU에 자원을 넘길때 사용

	ID3D11Texture1D, ID3D11Texture2D ,ID3D11Texture3D
		- 1D, 2D, 3D 이미지를 저장하는 자원
		- 이미지는 사용 범위가 광범위 하기 때문에 자료형만으로는 
		사용 용도를 예측하기 어렵다
		- 용도를 구분하기위해 Resource View를 사용한다.
			ㅁ Resource View
				- Texture 자원의 사용 용도를 명확히 해주는 개념
				ㅁ 종류
					- ID3D11RenderTargetView	: 화면에 그리기 위한 용도
					- ID3D11ShaderResourceView	: 자원을 넘기기 위한 용도
					- ID3D11DepthStencilView	: 깊이 정보를 저장하는 용도
					- ID3D11UnorderedAccessView	: 순서없이 접근하는 자원
*/