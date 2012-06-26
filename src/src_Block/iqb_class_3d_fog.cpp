
#include "iqb_class_3d_fog.h"
#include "iqb_class_3d_shader.h"

CSm3DFog::CSm3DFog(IDirect3DDevice9* pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;

	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, 0x00000000);
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, 1);

	m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
	
	m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *((DWORD*)(&FOG_START)));
	m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *((DWORD*)(&FOG_END)));

	m_pD3DDevice->SetRenderState(D3DRS_RANGEFOGENABLE, 1);

	shader::SetFog(m_pD3DDevice, FOG_START, FOG_END);

	m_Apply();
}

unsigned long CSm3DFog::Process(long refTime, ISmActor* pSender)
{
	return 0;
}

void CSm3DFog::m_Apply(void)
{
}
