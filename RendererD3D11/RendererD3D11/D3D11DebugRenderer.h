#pragma once

struct DebugLineVertex
{
    Float3 pos;
    Float4 color;
};

class DebugRenderer : public IDebugRenderer
{
public:
    DebugRenderer();
    ~DebugRenderer();

    HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

    ULONG __stdcall AddRef() override;

    ULONG __stdcall Release() override;

    // device/context은 외부에서 생성된 것을 전달
    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context);

    // 렌더링 절차
    void __stdcall SetViewProj(const Float4x4& view, const Float4x4& proj) override;
    void __stdcall DrawLine(const Float3& a, const Float3& b, const Float4& color) override;
    void __stdcall DrawRay(const Float3& origin, Float3& dir, float length, const Color& color) override;
    void __stdcall RenderAll() override;
    void __stdcall Clear() override;

private:
    void CleanUp();

    ULONG refCount_;

    ID3D11Device* pDevice_;
    ID3D11DeviceContext* pContext_;

    // 머트리얼
    ID3D11VertexShader* pVS_;
    ID3D11PixelShader* pPS_;
    ID3D11SamplerState* pSamplerState_;

    // 카메라
    ID3D11Buffer* pCBViewProj_; // constant buffer for viewProj

    // 레벨.
    std::vector<DebugLineVertex> lineList_;
    ID3D11Buffer* pVertexBuffer_; // ->Mesh;
    ID3D11InputLayout* pInputLayout_;

    // 내부 헬퍼
    bool CompileShaders();
    bool CreateBuffers();
    bool CreateSamplerState();
    void ReleaseResources();
};