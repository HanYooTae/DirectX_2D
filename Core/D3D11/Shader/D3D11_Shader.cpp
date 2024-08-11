#include "stdafx.h"
#include "D3D11_Shader.h"

D3D11_Shader::D3D11_Shader(Graphics* graphics)
{
    device = graphics->GetDevice();
}

D3D11_Shader::~D3D11_Shader()
{
    Clear();
}

const char* D3D11_Shader::GetEntryPoint() const
{
    static const char* entry_point_empty = nullptr;
    static const char* entry_point_vs = "VS";
    static const char* entry_point_ps = "PS";

    switch (shader_scope)
    {
    case ShaderScope_VS: return entry_point_vs;
    case ShaderScope_PS: return entry_point_ps;
    }

    return entry_point_empty;
}

const char* D3D11_Shader::GetTargetProfile() const
{
#ifdef SHADER_4_0
    static const char* target_profile_empty = nullptr;
    static const char* target_profile_vs = "vs_4_0";
    static const char* target_profile_ps = "ps_4_0";
#endif // SHADER_4_0

#ifdef SHADER_5_0
    static const char* target_profile_empty = nullptr;
    static const char* target_profile_vs = "vs_5_0";
    static const char* target_profile_ps = "ps_5_0";
#endif // SHADER_4_0

    switch (shader_scope)
    {
    case ShaderScope_VS: return target_profile_vs;
    case ShaderScope_PS: return target_profile_ps;
    }

    return target_profile_empty;
}

const char* D3D11_Shader::GetShaderModel() const
{
    static const char* shader_model = "5_0";

    return shader_model;
}

void D3D11_Shader::Create(const ShaderScope& scope, const std::string& shader)
{
    shader_scope = scope;

    compilation_state = CompilationState::Compling;
    resource = Compile(scope, shader);
    compilation_state = HasResource() ? CompilationState::Succeeded : CompilationState::Failed;
}

void D3D11_Shader::Clear()
{
    switch (shader_scope)
    {
    case ShaderScope_VS:
    {
        auto shader = static_cast<ID3D11VertexShader*>(resource);
        SAFE_RELEASE(shader);
        break;
    }
    case ShaderScope_PS:
        auto shader = static_cast<ID3D11PixelShader*>(resource);
        SAFE_RELEASE(shader);
        break;
    }

    SAFE_RELEASE(shader_blob);
    name = "";
    path = "";
    shader_scope = ShaderScope_Unknown;
    compilation_state = CompilationState::Unknown;
}

void* D3D11_Shader::Compile(const ShaderScope& scope, const std::string& shader)
{
    ID3DBlob* error = nullptr;

    HRESULT result = D3DX11CompileFromFileA
    (
        shader.c_str(),
        nullptr,
        nullptr,
        GetEntryPoint(),
        GetTargetProfile(),
        0,
        0,
        nullptr,
        &shader_blob,
        &error,
        nullptr
    );

    if (error != nullptr)
    {
        auto str = static_cast<const char*>(error->GetBufferPointer());
        // MessageBoxA(핸들(생략가능), 내용, 제목, 박스 타입) : 메시지 박스 출력 함수
        MessageBoxA(nullptr, str, "Shader Error!!", MB_OK);
    }
    assert(SUCCEEDED(result));

    void* shader_resource = nullptr;

    if (shader_blob != nullptr)
    {
        switch (shader_scope)
        {
        case ShaderScope_VS:
        {
            result = device->CreateVertexShader
            (
                shader_blob->GetBufferPointer(),
                shader_blob->GetBufferSize(),
                nullptr,
                reinterpret_cast<ID3D11VertexShader**>(&shader_resource)
            );
            assert(SUCCEEDED(result));
            break;
        }
        case ShaderScope_PS:
        {
            result = device->CreatePixelShader
            (
                shader_blob->GetBufferPointer(),
                shader_blob->GetBufferSize(),
                nullptr,
                reinterpret_cast<ID3D11PixelShader**>(&shader_resource)
            );
            assert(SUCCEEDED(result));
            break;
        }
        }
    }

    return shader_resource;
}
