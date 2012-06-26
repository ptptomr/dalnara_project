
#ifndef __IQB_CLASS_3D_VERTEX_H__
#define __IQB_CLASS_3D_VERTEX_H__

#include "iqb_base_type.h"
#include "iqb_class_3d.h"
#include "iqb_class_3d_texture.h"
#include "iqb_class_3d_shader.h"


enum EVertexFormat
{
	VERTEXFORMAT_POS_NOR_TEX,
	VERTEXFORMAT_POS_DIF_TEX,
	VERTEXFORMAT_POS_TEX,
	VERTEXFORMAT_POS_DIF,
	VERTEXFORMAT_POS,
	VERTEXFORMAT_POS_NOR,
};

template <EVertexFormat vertex_format> struct TVertexDataTraits
{
};

template <> struct TVertexDataTraits<VERTEXFORMAT_POS_NOR_TEX>
{
    enum
    {
        FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1,
    };
    struct TVertexData
	{
		TD3DVector3 position;
		TD3DVector3 normal;
		TD3DVector2 texPos;
    };
};

template <> struct TVertexDataTraits<VERTEXFORMAT_POS_DIF_TEX>
{
    enum
    {
        FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1,
    };
    struct TVertexData
	{
		TD3DVector3 position;
		TD3DVector4 color;
		TD3DVector2 texPos;
    };
};

template <> struct TVertexDataTraits<VERTEXFORMAT_POS_TEX>
{
    enum
    {
        FVF = D3DFVF_XYZ | D3DFVF_TEX1,
    };
    struct TVertexData
	{
		TD3DVector3 position;
		TD3DVector2 texPos;
    };
};

template <> struct TVertexDataTraits<VERTEXFORMAT_POS_DIF>
{
    enum
    {
        FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE,
    };
    struct TVertexData
	{
		TD3DVector3 position;
		TD3DVector4 color;
    };
};

template <> struct TVertexDataTraits<VERTEXFORMAT_POS>
{
    enum
    {
        FVF = D3DFVF_XYZ,
    };
    struct TVertexData
	{
		TD3DVector3 position;
    };
};

template <> struct TVertexDataTraits<VERTEXFORMAT_POS_NOR>
{
    enum
    {
        FVF = D3DFVF_XYZ | D3DFVF_NORMAL,
    };
    struct TVertexData
	{
		TD3DVector3 position;
		TD3DVector3 normal;
    };
};

template <EVertexFormat vertex_format>
struct TVertexBuffer
{
protected:
	IDirect3DDevice9* m_pD3DDevice;

public:
    typedef typename TVertexDataTraits<vertex_format>::TVertexData TVertexData;

    enum
    {
        FVF = TVertexDataTraits<vertex_format>::FVF
    };

	IDirect3DVertexBuffer9*  pVertexBuffer;
	iu::shared_ptr<CTexture> texture;
	TShaderSet               m_shaderSet;
	unsigned int             m_sizeVertex;
	unsigned int             m_countVertex;

	TVertexBuffer(IDirect3DDevice9* pD3DDevice, TVertexData* pData, unsigned int size, unsigned int count, iu::shared_ptr<CTexture> _texture = iu::shared_ptr<CTexture>((CTexture*)NULL))
		: m_pD3DDevice(pD3DDevice), pVertexBuffer(NULL), texture(_texture), m_sizeVertex(size), m_countVertex(count)
	{
		if (_SUCCEEDED(pD3DDevice->CreateVertexBuffer(m_sizeVertex*m_countVertex*sizeof(TVertexData), D3DUSAGE_WRITEONLY, FVF, D3DPOOL_MANAGED, &pVertexBuffer, 0)))
		{
			Assign(pData, 0, m_sizeVertex*m_countVertex);
		}
	}
	~TVertexBuffer()
	{
		if (pVertexBuffer)
		{
			pVertexBuffer->Release();
			pVertexBuffer = NULL;
		}
	}

	void SetShader(TShaderSet shaderSet)
	{
		m_shaderSet = shaderSet;
	}

	void DrawPrimitive(D3DPRIMITIVETYPE type, unsigned int ixStart, unsigned int count)
	{
		m_pD3DDevice->DrawPrimitive(type, ixStart, count);
	}

	void Assign(TVertexData* pData, unsigned int index, unsigned int num)
	{
		if (pVertexBuffer)
		{
			//?? 범위 체크 해야 함
			void* pDest = NULL;
			if (_SUCCEEDED(pVertexBuffer->Lock(index, num, &pDest, 0)))
			{
				memcpy(pDest, pData, num*sizeof(TVertexData));
				pVertexBuffer->Unlock();
			}
		}
	}

	virtual void Begin(void)
	{
		if (texture.get())
			m_pD3DDevice->SetTexture(0, texture->m_p_texture);
		else
			m_pD3DDevice->SetTexture(0, NULL);

		m_pD3DDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(TVertexData));

		if (m_shaderSet.vertex.get())
		{
			m_pD3DDevice->SetVertexDeclaration(m_shaderSet.vertex->pDeclation);
			m_pD3DDevice->SetVertexShader(m_shaderSet.vertex->pShader);
		}
		else
		{
			m_pD3DDevice->SetFVF(FVF);
		}

		if (m_shaderSet.pixel.get())
		{
			m_pD3DDevice->SetPixelShader(m_shaderSet.pixel->pShader);
		}

		{
			IDirect3DDevice9::TStatus::TStreamSource stream_source = m_pD3DDevice->m_status.stream_source[0];

			float* p_stream_data = (float*)stream_source.p_stream_data->m_p_buffer;
			int    stream_data_offset = 0;
			DWORD  fvf = stream_source.p_stream_data->m_fvf;

			if (fvf & D3DFVF_XYZ)
			{
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, stream_source.stride, p_stream_data + stream_data_offset);

				stream_data_offset += 3;
			}
			else
			{
				glDisableClientState(GL_VERTEX_ARRAY);
			}

			if (fvf & D3DFVF_DIFFUSE)
			{
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(4, GL_FLOAT, stream_source.stride, p_stream_data + stream_data_offset);

				stream_data_offset += 4;
			}
			else
			{
				glDisableClientState(GL_COLOR_ARRAY);
			}

			if (fvf & D3DFVF_NORMAL)
			{
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, stream_source.stride, p_stream_data + stream_data_offset);
				glEnable(GL_NORMALIZE);

				glEnable(GL_LIGHTING);

				stream_data_offset += 3;
			}
			else
			{
				glDisable(GL_LIGHTING);
				glDisableClientState(GL_NORMAL_ARRAY);
			}

			if (fvf & D3DFVF_TEX1)
			{
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, stream_source.stride, p_stream_data + stream_data_offset);

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0);

				if (texture.get())
				{
					GLuint h_texture = ((avej_lite::TSurfaceDesc*)texture->m_p_texture->m_p_surface->Handle())->hw_handle;

					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, h_texture);
					glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}

				stream_data_offset += 2;
			}
			else
			{
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_BLEND);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}
		}
	}

	virtual void End(void)
	{
		m_pD3DDevice->SetVertexShader(NULL);
		m_pD3DDevice->SetPixelShader(NULL);
	}
};

template <EVertexFormat vertex_format>
struct TVertexIndexBuffer: public TVertexBuffer<vertex_format>
{
	typedef typename TVertexBuffer<vertex_format>::TVertexData TVertexData;

public:
	IDirect3DIndexBuffer9* pIndexBuffer;
	//iu::vector<iu::pair<int, int> > ready_buffer;
	iu::vector<short> vertex_buffer;

	TVertexIndexBuffer(IDirect3DDevice9* pD3DDevice, TVertexData* pVertexData, unsigned int size, unsigned int count, short* pIndexData, unsigned int numIndex, iu::shared_ptr<CTexture> _texture)
		: TVertexBuffer<vertex_format>(pD3DDevice, pVertexData, size, count, _texture), pIndexBuffer(NULL)
	{
		if (_SUCCEEDED(pD3DDevice->CreateIndexBuffer(numIndex*sizeof(short), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pIndexBuffer, 0)))
		{
			void* pDest = NULL;
			if (_SUCCEEDED(pIndexBuffer->Lock(0, 0, &pDest, 0)))
			{
				memcpy(pDest, pIndexData, numIndex*sizeof(short));
				pIndexBuffer->Unlock();
			}
		}
	}
	~TVertexIndexBuffer()
	{
		if (pIndexBuffer)
		{
			pIndexBuffer->Release();
			pIndexBuffer = NULL;
		}
	}
	
	void DrawIndexedPrimitive(D3DPRIMITIVETYPE type, unsigned int ix_start, unsigned int count)
	{
#if 1
		this->m_pD3DDevice->DrawIndexedPrimitive(type, ix_start, 0, this->m_sizeVertex * this->m_countVertex, 0, count);
#else
		{
			short* p_src = (short*)this->m_pD3DDevice->m_status.p_index_buffer->m_p_buffer;

			for (int i = 0; i < count * 3; i++)
				vertex_buffer.push_back(*p_src++ + ix_start);

			int size = vertex_buffer.size();

			if (size > 2048)
			{
				glDrawElements(GL_TRIANGLES, vertex_buffer.size(), GL_UNSIGNED_SHORT, vertex_buffer.begin());
				vertex_buffer.clear();
			}
		}
#endif
	}

	virtual void Begin(void)
	{
		TVertexBuffer<vertex_format>::Begin();

		this->m_pD3DDevice->SetIndices(pIndexBuffer);

		vertex_buffer.clear();
	}
	
	virtual void End(void)
	{
		TVertexBuffer<vertex_format>::End();
	}

	virtual void Flush(void)
	{
#if 0
		glDrawElements(GL_TRIANGLES, vertex_buffer.size(), GL_UNSIGNED_SHORT, vertex_buffer.begin());
#endif
	}
};

#endif // #ifndef __IQB_CLASS_3D_VERTEX_H__
