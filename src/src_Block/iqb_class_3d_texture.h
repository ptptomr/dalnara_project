
#ifndef __IQB_CLASS_3D_TEXTURE_H__
#define __IQB_CLASS_3D_TEXTURE_H__

#include "iqb_class_3d.h"

struct CTexture
{
	IDirect3DDevice9*  m_p_ref_d3d_device;
	IDirect3DTexture9* m_p_texture;

	CTexture(IDirect3DDevice9* pD3DDevice, const char* szFileName)
		: m_p_ref_d3d_device(pD3DDevice), m_p_texture(NULL)
	{
		D3DXCreateTextureFromFileA(pD3DDevice, szFileName, &m_p_texture);
	}
	~CTexture()
	{
		if (m_p_texture)
			m_p_texture->Release();
	}
};

#endif // #ifndef __IQB_CLASS_3D_TEXTURE_H__
