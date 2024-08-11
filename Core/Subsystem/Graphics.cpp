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
	// ����̽�, ����̽� ���ؽ�Ʈ, ����ü�� ����
	{
		// DXGI_SWAP_CHAIN_DESC : ����ü���� ����� ���� ����
		DXGI_SWAP_CHAIN_DESC desc;
		ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
		desc.BufferDesc.Width	= 0; // ����ü���� ������ ������ ũ��(�ʺ�)
		desc.BufferDesc.Height	= 0; // ����ü���� ������ ������ ũ��(����)


		/*
			RefreshRate : ȭ�� �ֻ���, ȭ���� ��� �ֱ�� ������ ���ΰ�?
			Numerator   : ����
			Denominator : �и�

			60/1 => 1�ʴ� 60�� ȭ���� ���� �ϰڴ�. => 60������
		*/
		desc.BufferDesc.RefreshRate.Numerator;
		desc.BufferDesc.RefreshRate.Denominator;
		

		/*
			RGBA : ���� ä���� �ǹ�, ������ ���� �����͸� ���� �ʿ�� ����.
			R8G8B8A8 : BPS, Color Depth��� �θ��� 
			������ ä�ο� ��ŭ�� �޸𸮸� �Ҵ������� �ǹ��Ѵ�.
			�� ä���� 8��Ʈ�̴� �� ä�δ� 0 ~ 255�� ������ ������.

			UNORM : Unsigned Normalize
			0 ~ 1 �� ����ȭ�� �����ͷ� ���ڴ�
			=> 4 ����Ʈ ũ��(�� ä�δ� 8��Ʈ * 4)�� ������ ����ȭ�� �����͸� ��ڴ�.

			���� : BPC ����
		*/

		// �츮�� ���� ���۰� � ������ �����͸� �������ΰ�?
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// Scanline : ȭ���� ����� �� ���پ� �׾ ����ϴ� ��
		// UNSPECIFIED : �������� ����
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // ȭ�� ���/Ȯ���� �� � ȿ���� �� ���ΰ�?

		desc.BufferCount = 1; // ������ ����
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���۸� ��� ����� ���ΰ�

		/*
			Aliasing : �ȼ� ǥ������ ���� Ȯ���ϸ� �ܰ����� ���ó�� ���̴� ����

			Anti Aliasing(AA) - ��� ������ �����ϴ� ���
				�� Super Sampling(SSAA)
					�� ���ø� �����ŭ �ػ󵵸� Ȯ���� �Ŀ� Down Sampling(�ػ󵵸� �ٿ� ���� ���´�)�� �ϴ� ���
					�� ���� ���귮�� �޸𸮰� �ʿ��ϴ�

				�� Multi Sampling(MSAA)
					�� ���� ���귮�� �޸𸮸� ����ϴ� SSAA�� �����ؼ� ���� ���
					�� �������� �����ڸ��� AA�� ����

			// ���� : ��Ƽ �ٸ���� �����ϱ�
		*/
		desc.SampleDesc.Count = 1;		// ������ ����, ���
		desc.SampleDesc.Quality = 0;	// ���ø��� ����Ƽ ����, 0�� ���ϰڴٴ� �ǹ�


		desc.OutputWindow = Settings::Get().GetWindowHandle(); // ����� ������ �ڵ�
		desc.Windowed = TRUE; // â��� ����

		// DISCARD : �����Ͽ� ���� �����͸� ����ϰڴ�.
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // ���ҽ� � ȿ���� �� ���ΰ�?

		// FEATURE_LEVEL : ��� ����, DX �� ������ �� ���ΰ�?
		std::vector<D3D_FEATURE_LEVEL> feature_levels
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};

		// HRESULT : �Լ� ����� �������θ� ��� �ڷ���
		// S_OK(����), S_FALSE(����)�� ��ȯ�޴´�.
		HRESULT hr = D3D11CreateDeviceAndSwapChain // ����̽�, ����̽� ���ؽ�Ʈ, ����ü�� ���� �Լ�
		(
			nullptr,					// ���� ī�带 �����ؼ� ����� �� ���
			D3D_DRIVER_TYPE_HARDWARE,	// ����̽� Ÿ��, �ϵ���� ������ ����ϱ� ���� HARDWARE�� ����
			nullptr,					// �ٸ� ����Ʈ��� �߰������� �������ΰ�?
			0,							// ����̽��� ���� �� � ����� �߰������� �������ΰ�?
			feature_levels.data(),		// ��ó ���� �迭�� ���� �ּ�
			feature_levels.size(),		// ��ó ���� ī��Ʈ
			D3D11_SDK_VERSION,			// ����ϴ� SDK ����
			&desc,						// ����� desc �ּ�
			&swap_chain,				// ������ ����ü���� �Ҵ���� ��ġ
			&device,					// ������ ����̽��� �Ҵ���� ��ġ
			nullptr,					// ���õ� ��ɷ����� ��ȯ�޴´�.
			&device_context				// ������ ����̽� ���ؽ�Ʈ�� �Ҵ���� ��ġ
		);
		assert(SUCCEEDED(hr)); // SUCCEEDED ������ true, ���н� false ����

	}

	return true;
}

void Graphics::Update()
{
}

void Graphics::CreateBackBuffer(const uint& width, const uint& height)
{

	// RTV ����
	// ResizeBuffers : ���� ü���� ���� ������ �����ϴ� �Լ�
	// �������� ���� ������ �⺻���� �־��ش�.
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
			- �������̽��� �����ϱ� ���� �ĺ����̴�.
			- GUID ���� �ϳ�

		Globally Unique Identifier(GUID)
			- MS���� ����ϴ� ���� �׸��� �����ϱ� ���� �ĺ���
			- 16����Ʈ�� ũ�⸦ ������.
			- UUID��� �ִµ� ���̰� ���� �����ص� �ȴ�.

		���� : GUID, UUID ����
	*/

	hr = swap_chain->GetBuffer // ����ü�� ������ ���۸� ������.
	(
		0, // ���� ������ �ε��� ��ȣ
		__uuidof(ID3D11Texture2D), // �� ���۸� �޴� �������̽��� Ÿ��
		reinterpret_cast<void**>(&back_buffer) // ��ȯ�� �������̽��� �޴� ���� ������
	);
	assert(SUCCEEDED(hr));

	hr = device->CreateRenderTargetView
	(
		back_buffer, // �信�� ������ ���ҽ�, �ش� ���ҽ��� ����Ͽ�  RTV�����.
		nullptr, // RTV desc, nullptr�Ͻ� ���ҽ��� ������ ������.
		&render_target_view // RTV ���� ����
	);
	assert(SUCCEEDED(hr));

	// ����Ʈ �ʱ�ȭ
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
		Output Merger Stage(OM) - ��� ���� �ܰ�
		- �׸��� �׸� ���� Ÿ���� �����ϰ�, ���� ��� �������ΰ�(����)�� �����Ѵ�.
		- OM�� ���õ� �� �ִ� ���� Ÿ���� ������ 8���̴�. -> 8���� ���ÿ� �׸� �� �ִ�.
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
	HRESULT hr = swap_chain->Present(1, 0); // Present() : �ĸ� ���۸� ���� ���۷� ����Ѵ�.
	assert(SUCCEEDED(hr));
}
 
/*
	ID3D11Resource
		- DX���� �����ϴ� �ڿ����� ��ӹ޴� Ŭ����
		- ũ�� Buffer, Texture�� ������.

	ID3D11Buffer
		- ����ڰ� ����ü�� ����� GPU�� �ڿ��� �ѱ涧 ���

	ID3D11Texture1D, ID3D11Texture2D ,ID3D11Texture3D
		- 1D, 2D, 3D �̹����� �����ϴ� �ڿ�
		- �̹����� ��� ������ ������ �ϱ� ������ �ڷ��������δ� 
		��� �뵵�� �����ϱ� ��ƴ�
		- �뵵�� �����ϱ����� Resource View�� ����Ѵ�.
			�� Resource View
				- Texture �ڿ��� ��� �뵵�� ��Ȯ�� ���ִ� ����
				�� ����
					- ID3D11RenderTargetView	: ȭ�鿡 �׸��� ���� �뵵
					- ID3D11ShaderResourceView	: �ڿ��� �ѱ�� ���� �뵵
					- ID3D11DepthStencilView	: ���� ������ �����ϴ� �뵵
					- ID3D11UnorderedAccessView	: �������� �����ϴ� �ڿ�
*/