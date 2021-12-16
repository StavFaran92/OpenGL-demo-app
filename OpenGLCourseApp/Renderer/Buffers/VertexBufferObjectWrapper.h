#pragma once
class VertexBufferObjectWrapper
{
public:
	VertexBufferObjectWrapper(const void* data, unsigned int size);
	~VertexBufferObjectWrapper();

	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_id;
};

