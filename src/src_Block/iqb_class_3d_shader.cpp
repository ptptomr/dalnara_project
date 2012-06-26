
#include "iqb_class_3d_shader.h"

iu::shared_ptr<TVertexShader> CreateVertexShader(IDirect3DDevice9* pD3DDevice, const char* szShaderCode, DWORD fvf)
{
	iu::shared_ptr<TVertexShader> vertexShader = iu::shared_ptr<TVertexShader>(new TVertexShader);
/*
	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(fvf, decl);

	if (FAILED(pD3DDevice->CreateVertexDeclaration(decl, &vertexShader->pDeclation)))
		return iu::shared_ptr<TVertexShader>();

	{
		LPD3DXBUFFER pCode;

		if (FAILED(D3DXAssembleShader(szShaderCode, strlen(szShaderCode), NULL, NULL, D3DXSHADER_DEBUG, &pCode, NULL)))
			return iu::shared_ptr<TVertexShader>();

		pD3DDevice->CreateVertexShader((DWORD*)pCode->GetBufferPointer(), &vertexShader->pShader);

		pCode->Release();
	}
*/
	return vertexShader;
}

iu::shared_ptr<TPixelShader> CreatePixelShader(IDirect3DDevice9* pD3DDevice, const char* szShaderCode)
{
	iu::shared_ptr<TPixelShader> pixelShader = iu::shared_ptr<TPixelShader>(new TPixelShader);
/*
	{
		LPD3DXBUFFER pCode;

		if (FAILED(D3DXAssembleShader(szShaderCode, strlen(szShaderCode), NULL, NULL, D3DXSHADER_DEBUG, &pCode, NULL)))
			return iu::shared_ptr<TPixelShader>();

		pD3DDevice->CreatePixelShader((DWORD*)pCode->GetBufferPointer(), &pixelShader->pShader);

		pCode->Release();
	}
*/	
	return pixelShader;
}

namespace shader
{
	void SetFog(IDirect3DDevice9* pD3DDevice, float fogStart, float fogEnd)
	{
		float fogData[4] = { fogStart, fogEnd, 1.0f / (fogEnd - fogStart), 0.0f };
		pD3DDevice->SetVertexShaderConstantF(3, fogData, 1);
	}
	void SetTexFactor(IDirect3DDevice9* pD3DDevice, float factor)
	{
		float textureFactor[4] = { 1.0f, 1.0f, 1.0f, factor };
		pD3DDevice->SetVertexShaderConstantF(23, textureFactor, 1);
	}
	void SetLightDir(IDirect3DDevice9* pD3DDevice, float xDir, float yDir, float zDir)
	{
		float direction[4] = { -xDir, -yDir, -zDir, 0.0f };
		pD3DDevice->SetVertexShaderConstantF(20, direction, 1);
	}
	void SetLightDiffuse(IDirect3DDevice9* pD3DDevice, float R, float G, float B)
	{
		float diffuse[4] = { R, G, B, 0.0f };
		pD3DDevice->SetVertexShaderConstantF(21, diffuse, 1);
	}
	void SetLightAmbient(IDirect3DDevice9* pD3DDevice, float R, float G, float B)
	{
		float ambient[4] = { R, G, B, 0.0f };
		pD3DDevice->SetVertexShaderConstantF(22, ambient, 1);
	}
}
