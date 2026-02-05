#pragma once

class Renderer
{
private:
	friend class Engine;
	static HMODULE GRendererModule;
	static IRenderer* GRenderer;
	static IRenderResourceFactory* GRenderResourceFactory;
	static bool LoadRenderer();

public:
	static IRenderer* Instance();
	static IRenderResourceFactory* GetFactory();

	static void ShutDown();
};
