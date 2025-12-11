#pragma once

class Shader;
class SamplerStateT;
class BlendStateT;


struct DebugLineVertex
{
    Float3 pos;
    Float4 color;
};

class DebugPass : public IUnknown
{
    friend class Renderer;
    DebugPass();
    ~DebugPass();

    bool Init();
public:
    

    HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

    ULONG __stdcall AddRef() override;

    ULONG __stdcall Release() override;

    void Render();

    // 렌더링 절차
    //void SetViewProj(const Float4x4& view, const Float4x4& proj);
    void DrawLine(const Float3& start, const Float3& end, const Float4& color);
    void DrawRay(const Float3& origin, Float3& dir, float length, const Color& color);
    //void RenderAll();
    void Clear();

private:
    bool InitVertexBuffer();

    bool InitShaders();

    bool InitBlendState();

    //bool InitSamplerState();

    void CleanUp();

    ULONG refCount_;

    std::vector<DebugLineVertex> lineList_;
    ID3D11Buffer* pVertexBuffer_; // -> LIne

    Shader* pShader_;
    //SamplerStateT* pSamplerState_;
    BlendState* pBlendState_;

    







    //ID3D11Device* pDevice_;
//ID3D11DeviceContext* pContext_;

    // 머트리얼
//ID3D11VertexShader* pVS_;
//ID3D11PixelShader* pPS_;
    //ID3D11SamplerState* pSamplerState_;
    //ID3D11InputLayout* pInputLayout_;

    // 카메라
//ID3D11Buffer* pCBViewProj_; // constant buffer for viewProj
// 
// 
    // 내부 헬퍼
    //bool CompileShaders();
    //bool CreateBuffers();
    //bool CreateSamplerState();
    //void ReleaseResources();
};