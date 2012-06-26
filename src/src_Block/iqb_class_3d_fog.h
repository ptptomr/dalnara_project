
#ifndef __IQB_CLASS_3D_FOG_H__
#define __IQB_CLASS_3D_FOG_H__

#include "iqb_base_type.h"
#include "iqb_class_3d.h"

const float FOG_START = 18.0f * 1.0f;
const float FOG_END   = 30.0f * 1.0f;

class CSm3DFog: public ISmActor
{
private:
	IDirect3DDevice9* m_pD3DDevice;
	single m_fogStart;
	single m_fogEnd;

	void m_Apply(void);

public:
	CSm3DFog(IDirect3DDevice9* pD3DDevice);
	unsigned long Process(long refTime = 0, ISmActor* pSender = 0);

};

#endif // #ifndef __IQB_CLASS_3D_FOG_H__
