#pragma once
class IndexBuffer
{
public:
	IndexBuffer(const unsigned int * data, unsigned int length);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int getLength() const;
private:
	unsigned int m_id = 0;
	unsigned int m_length = 0;
};

