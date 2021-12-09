#pragma once
class IndexBuffer
{
private:
	unsigned int mRendererID;
	unsigned int mLength;
public:
	IndexBuffer(const unsigned int * data, unsigned int length);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int getLength() const;
};

