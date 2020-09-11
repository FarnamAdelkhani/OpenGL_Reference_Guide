#pragma once

class index_buffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;

public:
	index_buffer(const unsigned int* data, unsigned int count);
	~index_buffer();

	void bind() const;

	void unbind() const;

	inline unsigned int GetCount() const { return m_Count;  }
};