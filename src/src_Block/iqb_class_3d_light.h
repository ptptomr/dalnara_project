
#ifndef __IQB_CLASS_3D_LIGHT_H__
#define __IQB_CLASS_3D_LIGHT_H__

#include "iqb_base_type.h"
#include "iqb_class_3d.h"

namespace erio
{
	class CSm3DLight: public ISmActor
	{
	public:
		CSm3DLight(IDirect3DDevice9* p_d3d_device);

		unsigned long Process(long ref_time = 0, ISmActor* p_sender = 0);

	private:
		IDirect3DDevice9* m_p_d3d_device;
		TD3DLight9        m_light;

		void m_Apply(void);
	};

} // namespace erio

#endif // #ifndef __IQB_CLASS_3D_LIGHT_H__
