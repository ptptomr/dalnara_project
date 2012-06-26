
////////////////////////////////////////////////////////////////////////////////
// uses

#include "iqb_z_config.h"
#include "iqb_base_gfx.h"

using namespace avej_lite;

namespace erio
{

////////////////////////////////////////////////////////////////////////////////
// callback

namespace title
{
	bool OnCreate(void)
	{
		return true;
	}

	bool OnDestory(void)
	{
		return true;
	}

	bool OnProcess(void)
	{
		CInputDevice& input_device = avej_lite::singleton<CInputDevice>::get();
		input_device.UpdateInputState();

		if (input_device.WasKeyPressed(avej_lite::INPUT_KEY_SYS1))
		{
			g_ChangeState(STATE_EXIT);
			return false;
		}

		{
			IGfxDevice::TDeviceDesc screenDesc;
			g_p_gfx_device->GetDesc(screenDesc);

			g_p_gfx_device->BeginDraw();

			gfx::FillRect(0xFF204080, 50, 50, 100, 100); 
			gfx::BitBlt(0, 0, g_p_res_sprite, 0, 0, 512, 256);

			g_p_gfx_device->EndDraw();
			g_p_gfx_device->Flip();
		}

		return true;
	}

	AppCallback callback =
	{
		OnCreate,
		OnDestory,
		OnProcess
	};
}

} // namespace erio
