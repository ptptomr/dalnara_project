
#ifndef __IQB_CLASS_3D_LIGHT_H__
#define __IQB_CLASS_3D_LIGHT_H__

#include "iqb_base_type.h"
#include "iqb_class_3d.h"

class CSm3DLight: public ISmActor
{
private:
	IDirect3DDevice9* m_pD3DDevice;
	TD3DLight9 m_light;

	void m_Apply(void);

public:
	CSm3DLight(IDirect3DDevice9* pD3DDevice);

	unsigned long Process(long refTime = 0, ISmActor* pSender = 0);
};

#endif // #ifndef __IQB_CLASS_3D_LIGHT_H__
