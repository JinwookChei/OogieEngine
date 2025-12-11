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

    void DrawLine(const Float3& start, const Float3& end, const Float4& color);
    void DrawRay(const Float3& origin, Float3& dir, float length, const Color& color);
    void Clear();

private:
    bool InitVertexBuffer();

    bool InitShaders();

    bool InitBlendState();

    void CleanUp();

    ULONG refCount_;

    std::vector<DebugLineVertex> lineList_;
    ID3D11Buffer* pVertexBuffer_; // -> LIne

    Shader* pShader_;
    BlendState* pBlendState_;
};