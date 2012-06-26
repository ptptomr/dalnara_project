
#include "iqb_class_3d_camera.h"
#include <math.h>

void CSm3DCamera::m_SetAngle(single angle)
{
	m_angle    = angle;
	m_modified = true;
}

void CSm3DCamera::m_SetHeight(single height)
{
	m_height   = height;
	m_modified = true;
}

void CSm3DCamera::m_SetRadius(single radius)
{
	m_radius   = radius;
	m_modified = true;
}

void CSm3DCamera::m_Apply(void)
{
	TD3DMatrix matView;

	m_vecEye = D3DVECTOR3(
		float(m_radius * sin(m_angle)), float(m_height), -float(m_radius * cos(m_angle)));
	D3DXMatrixLookAtLH(&matView, &m_vecEye, &m_vecLookAt, &m_vecUp);

	m_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
}

CSm3DCamera::CSm3DCamera(IDirect3DDevice9* pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;

	m_modified   = false;

	m_vecLookAt  = D3DVECTOR3(0.0, 1.0, 0.0);
	m_vecUp      = D3DVECTOR3(0.0, 1.0, 0.0);

	m_angle      = - 0.22f;
	m_height     = 8.0;
	m_radius     = 16.0;

	m_height     = 6.0;
	m_radius     = 12.0;

	m_Apply();
}

unsigned long CSm3DCamera::Process(long refTime, ISmActor* pSender)
{
	if (m_modified)
	{
		m_Apply();
		m_modified = false;
	}

	return 0;
}
