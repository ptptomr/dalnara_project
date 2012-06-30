
#include "iqb_z_config.h"
#include "iqb_base_gfx.h"

#include "iqb_class_3d_texture.h"

using namespace avej_lite;

namespace erio
{
	extern IDirect3DDevice9* g_p_d3d_device;

namespace team_logo
{
	float alpha_constant = 0.2f;
	iu::shared_ptr<CTexture> sprite;

	bool OnCreate(unsigned long param)
	{
		gfx::SetFlatMode();

		sprite = iu::shared_ptr<CTexture>(new CTexture(g_p_d3d_device, "team_logo2.tga"));
		if (sprite->m_p_texture == 0)
		{
			sprite = iu::shared_ptr<CTexture>(new CTexture(g_p_d3d_device, "res_Block/team_logo2.tga"));
		}

		return true;
	}

	bool OnDestory(void)
	{
		sprite.setNull();

		return true;
	}

	bool OnProcess(void)
	{
		static long startTick = util::GetTicks();
		static bool increase = true;
		long currTick = util::GetTicks();

		CInputDevice& input_device = avej_lite::singleton<CInputDevice>::get();
		input_device.UpdateInputState();

		if (input_device.WasKeyPressed(avej_lite::INPUT_KEY_SYS1) ||
			//currTick - startTick >= 2000)
			alpha_constant <= 0.f)
		{
			g_ChangeState(STATE_TITLE, 0x00010001);
			return false;
		}
		
		{
			IGfxDevice::TDeviceDesc screenDesc;
			g_p_gfx_device->GetDesc(screenDesc);

			g_p_gfx_device->BeginDraw();
			gfx::BlendBlt(800/2 - 256/2, 480/2 - 256/2, sprite->m_p_texture->m_p_surface, 0, 0, 256, 256, alpha_constant);
			g_p_gfx_device->EndDraw();
			g_p_gfx_device->Flip();

			if (alpha_constant < 1.0f && increase)
			{
				alpha_constant += 0.005f;
				if (alpha_constant >= 1.f)
				{
					increase = false;
				}
			}
			else if (currTick - startTick >= 2500)
			{
				alpha_constant -= 0.005f;
			}
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
