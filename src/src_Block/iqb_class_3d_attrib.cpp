
#include "iqb_class_3d_attrib.h"

CSm3DAttrib::CSm3DAttrib(IDirect3DDevice9* pD3DDevice, float screenRatio)
{
	// assign Direct3D device instance
	m_pD3DDevice = pD3DDevice;

	// initialize internal instances
	m_camera     = new CSm3DCamera(pD3DDevice);
	m_projection = new CSm3DProjection(pD3DDevice, screenRatio);
	m_light      = new CSm3DLight(pD3DDevice);
	m_fog        = new CSm3DFog(pD3DDevice);
}

CSm3DAttrib::~CSm3DAttrib(void)
{
	delete m_camera;
	delete m_projection;
	delete m_light;
	delete m_fog;
}

unsigned long CSm3DAttrib::Process(long refTime, ISmActor* pSender)
{
	m_camera->Process(refTime, pSender);
	m_projection->Process(refTime, pSender);

	return 0;
}
