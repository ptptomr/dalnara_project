
#ifndef __IQB_CLASS_3D_ATTRIB_H__
#define __IQB_CLASS_3D_ATTRIB_H__

#include "iqb_base_type.h"
#include "iqb_class_3d.h"
#include "iqb_class_3d_camera.h"
#include "iqb_class_3d_projection.h"
#include "iqb_class_3d_light.h"
#include "iqb_class_3d_fog.h"


class CSm3DAttrib: public ISmActor
{
private:
	IDirect3DDevice9* m_pD3DDevice;
	CSm3DCamera* m_camera;
	CSm3DProjection* m_projection;
	CSm3DLight* m_light;
	CSm3DFog* m_fog;

public:
	CSm3DAttrib(IDirect3DDevice9* pD3DDevice, float screenRatio);
	~CSm3DAttrib(void);

	unsigned long Process(long refTime = 0, ISmActor* pSender = 0);

	inline CSm3DCamera* GetCamera(void) { return m_camera; }
	inline CSm3DProjection* GetProjection(void) { return m_projection; }

};

#endif // #ifndef __IQB_CLASS_3D_ATTRIB_H__
