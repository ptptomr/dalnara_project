
#ifndef __IQB_CLASS_3D_SHADER_H__
#define __IQB_CLASS_3D_SHADER_H__

#include "iqb_class_3d.h"

struct TVertexShader
{
	IDirect3DVertexDeclaration9* pDeclation;
	IDirect3DVertexShader9*      pShader;

	TVertexShader()
		: pDeclation(NULL), pShader(NULL)
	{
	}
	TVertexShader(IDirect3DVertexDeclaration9* _pDeclation, IDirect3DVertexShader9* _pShader)
		: pDeclation(_pDeclation), pShader(_pShader)
	{
	}
	~TVertexShader()
	{
		if (pDeclation)
			pDeclation->Release();
		if (pShader)
			pShader->Release();
	}
};

struct TPixelShader
{
	IDirect3DPixelShader9*       pShader;

	TPixelShader()
		: pShader(NULL)
	{
	}
	TPixelShader(IDirect3DPixelShader9* _pShader)
		: pShader(_pShader)
	{
	}
	~TPixelShader()
	{
		if (pShader)
			pShader->Release();
	}
};

struct TShaderSet
{
	iu::shared_ptr<TVertexShader> vertex;
	iu::shared_ptr<TPixelShader>  pixel;

	TShaderSet()
	{
	}
	TShaderSet(iu::shared_ptr<TVertexShader> _vertex, iu::shared_ptr<TPixelShader> _pixel)
		: vertex(_vertex), pixel(_pixel)
	{
	}
};

iu::shared_ptr<TPixelShader> CreatePixelShader(IDirect3DDevice9* pD3DDevice, const char* szShaderCode);
iu::shared_ptr<TVertexShader> CreateVertexShader(IDirect3DDevice9* pD3DDevice, const char* szShaderCode, DWORD fvf);

namespace shader
{
	void SetFog(IDirect3DDevice9* pD3DDevice, float fogStart, float fogEnd);
	void SetTexFactor(IDirect3DDevice9* pD3DDevice, float factor);
	void SetLightDir(IDirect3DDevice9* pD3DDevice, float xDir, float yDir, float zDir);
	void SetLightDiffuse(IDirect3DDevice9* pD3DDevice, float R, float G, float B);
	void SetLightAmbient(IDirect3DDevice9* pD3DDevice, float R, float G, float B);
}

#endif // #ifndef __IQB_CLASS_3D_SHADER_H__
