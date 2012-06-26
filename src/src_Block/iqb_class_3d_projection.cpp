
#include "iqb_class_3d_projection.h"
#include <math.h>

void CSm3DProjection::m_InitiaizeProjectionMatrix(float screenRatio)
{
	TD3DMatrix matProj1, matProj2;
	single factor;
	int i, j, k;

	if (screenRatio == 0.0f)
		screenRatio = 4.0f / 3.0f;

	D3DXMatrixPerspectiveFovLH(&matProj1, D3DX_PI / 6.0f, screenRatio, 1.0f, 100.0f);
	D3DXMatrixOrthoOffCenterLH(&matProj2, -6.0f, 6.0f, -6.0f / screenRatio, 6.0f / screenRatio, 1.0f, 100.0f);

	for (k = 0; k <= HIGH_INDEX; k++)
	{
		factor = k * 1.0f + 0.1f;
		factor = float(log(1.0 / factor) - 0.0);
		for (j = 0; j < 4; j++)
		{
			for (i = 0; i < 4; i++)
			{
				m_projectionMatrix[k].m[i][j] = float(matProj1.m[i][j] + (matProj2.m[i][j] - matProj1.m[i][j]) * factor / (log(1.0/HIGH_INDEX)-0.0));
			}
		}
	}

	m_projectionMatrix[LOW_INDEX]  = matProj1;
	m_projectionMatrix[HIGH_INDEX] = matProj2;
}

void CSm3DProjection::m_SetProjectionMode(TProjectionMode mode)
{
	if (m_projectionMode == mode)
		return;

	++m_projectionStep;
}

void CSm3DProjection::m_Apply(void)
{
	TD3DMatrix matProject;

	if (m_projectionStep > 0)
	{
		if (m_projectionMode == pmPerspecive)
		{
			matProject = m_projectionMatrix[m_projectionStep];
		}
		else
		{
			matProject = m_projectionMatrix[HIGH_INDEX-m_projectionStep];
		}

		++m_projectionStep;
		if (m_projectionStep >= HIGH_INDEX)
		{
			m_projectionStep = LOW_INDEX;
			if (m_projectionMode == pmPerspecive)
				m_projectionMode = pmOrthogonal;
			else
				m_projectionMode = pmPerspecive;
		}
	}
	else
	{
		if (m_projectionMode == pmPerspecive)
			matProject = m_projectionMatrix[LOW_INDEX];
		else
			matProject = m_projectionMatrix[HIGH_INDEX];
	}

	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProject);
}

CSm3DProjection::CSm3DProjection(IDirect3DDevice9* pD3DDevice, float screenRatio)
{
	m_pD3DDevice = pD3DDevice;

	m_projectionMode = pmPerspecive;
	m_projectionStep = LOW_INDEX;

	m_InitiaizeProjectionMatrix(screenRatio);
	m_Apply();
}

unsigned long CSm3DProjection::Process(long refTime, ISmActor* pSender)
{
	if (m_projectionStep > 0)
		m_Apply();

	return 0;
}
