
#ifndef __IQB_CLASS_3D_PROJECTIOM_H__
#define __IQB_CLASS_3D_PROJECTIOM_H__

#include "iqb_base_type.h"
#include "iqb_class_3d.h"

class CSm3DProjection: public ISmActor
{
public:
	enum
	{
		LOW_INDEX  = 0,
		HIGH_INDEX = 100,
	};
	enum TProjectionMode
	{
		pmPerspecive,
		pmOrthogonal,
	};

private:
	IDirect3DDevice9* m_pD3DDevice;
	TD3DMatrix m_projectionMatrix[HIGH_INDEX+1];
	TProjectionMode m_projectionMode;
	int m_projectionStep;

	void m_InitiaizeProjectionMatrix(float screenRatio);
	void m_SetProjectionMode(TProjectionMode mode);
	void m_Apply(void);

public:
	CSm3DProjection(IDirect3DDevice9* pD3DDevice, float screenRatio);
	unsigned long Process(long refTime = 0, ISmActor* pSender = 0);

	TProjectionMode GetMode(void) { return m_projectionMode; }
	void SetMode(TProjectionMode mode) { m_SetProjectionMode(mode); }

};

#endif // #ifndef __IQB_CLASS_3D_PROJECTIOM_H__
