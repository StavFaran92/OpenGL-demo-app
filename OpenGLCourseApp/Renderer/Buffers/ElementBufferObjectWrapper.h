#pragma once

#include <iostream>

class ElementBufferObjectWrapper
{
public:
	ElementBufferObjectWrapper(unsigned int* data, unsigned int length);
	~ElementBufferObjectWrapper();

	void Bind() const;
	void Unbind() const;

	unsigned int getLength() const;
private:
	unsigned int m_id = 0;
	unsigned int m_length = 0;
};
