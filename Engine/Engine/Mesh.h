#pragma once

class Mesh
{
public:
	Mesh() = delete;

	Mesh(IVertex* pVertex);

	virtual ~Mesh();

	void Setting();

	bool AddInputLayout(const char* semanticName, uint32_t semanticIndex, uint32_t format, uint32_t inputSlot, bool isInstanceData);

	bool Draw();	

	void SetVertex(IVertex* pVertex);

	IVertex* GetVertex() const;

private:
	void CleanUp();

	IVertex* pVertexImpl_;
};