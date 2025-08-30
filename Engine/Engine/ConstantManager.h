#pragma once
struct ConstantBuffer;

class ConstantManager
{
public:
	ConstantManager();

	virtual ~ConstantManager();

	static ConstantManager* Instance();

	void Update();

	ConstantBuffer* GetConstantBuffer() const;

private:
	void CleanUp();

	ConstantBuffer* constantBuffer_;
};