
#include "iqb_class_3d_light.h"
#include "iqb_class_3d_shader.h"

CSm3DLight::CSm3DLight(IDirect3DDevice9* pD3DDevice)
{
	TD3DVector3 vecDirection;

	m_pD3DDevice = pD3DDevice;

	memset(&m_light, 0, sizeof(m_light));

	m_light.Type      = D3DLIGHT_DIRECTIONAL;
	m_light.Diffuse.r = 0.6f;
	m_light.Diffuse.g = 0.6f;
	m_light.Diffuse.b = 0.6f;
	m_light.Range     = 1000.0;

	vecDirection      = D3DVECTOR3(-5.0, -5.0, 5.0);
	D3DXVec3Normalize((TD3DVector3*)&m_light.Direction, &vecDirection);

	m_pD3DDevice->LightEnable(0, true);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, 1);

	m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00606060);

	{
		shader::SetLightDiffuse(m_pD3DDevice, 0.6f, 0.6f, 0.6f);
		shader::SetLightAmbient(m_pD3DDevice, float(0x60) / 255.0f, float(0x60) / 255.0f, float(0x60) / 255.0f);
	}

	m_Apply();
}

unsigned long CSm3DLight::Process(long refTime, ISmActor* pSender)
{
	return 0;
}

void CSm3DLight::m_Apply(void)
{
	m_pD3DDevice->SetLight(0, &m_light);

	shader::SetLightDir(m_pD3DDevice, m_light.Direction.x, m_light.Direction.y, m_light.Direction.z);
}
