
#ifndef __IQB_CLASS_3D_CAMERA_H__
#define __IQB_CLASS_3D_CAMERA_H__

#include "iqb_base_type.h"
#include "iqb_class_3d.h"

class CSm3DCamera: public ISmActor
{
private:
	IDirect3DDevice9* m_pD3DDevice;
	bool        m_modified;
	TD3DVector3 m_vecEye;
	TD3DVector3 m_vecLookAt;
	TD3DVector3 m_vecUp;
	single      m_angle;
	single      m_height;
	single      m_radius;

	void m_SetAngle(single angle);
	void m_SetHeight(single height);
	void m_SetRadius(single radius);
	void m_Apply(void);

public:
	CSm3DCamera(IDirect3DDevice9* pD3DDevice);

	unsigned long Process(long refTime = 0, ISmActor* pSender = 0);

	single GetAngle(void)  { return m_angle; }
	void   SetAngle(single angle)   { m_SetAngle(angle); }
	single GetHeight(void) { return m_height; }
	void   SetHeight(single height) { m_SetHeight(height); }
	single GetRadius(void) { return m_radius; }
	void   SetRadius(single radius) { m_SetRadius(radius); }

};

#endif // #ifndef __IQB_CLASS_3D_CAMERA_H__
