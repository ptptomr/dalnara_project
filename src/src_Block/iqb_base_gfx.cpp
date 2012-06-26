
#include "iqb_base_gfx.h"
#include "gfx3d_config.h"

using namespace avej_lite;

////////////////////////////////////////////////////////////////////////////////
// global variable

namespace erio
{
	IGfxDevice*  g_p_gfx_device  = NULL;
	IGfxSurface* g_p_back_buffer = NULL;
	IGfxSurface* g_p_res_sprite  = NULL;
}

////////////////////////////////////////////////////////////////////////////////
// static

namespace
{
	inline avej_lite::TFixed16 _ToF16(int a)
	{
		avej_lite::TFixed16 temp(a);

		return temp;
	}

	inline int _ToInt(avej_lite::TFixed16 a)
	{
		return ((a.m_data) >> 16);
	}
}

namespace erio
{

////////////////////////////////////////////////////////////////////////////////
// public (gfx)

void gfx::Init(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustumx(0, 800 << 16, 480 << 16, 0, 1000, 1000*100);
	glMatrixMode(GL_MODELVIEW);
}

void gfx::FillRect(unsigned long real_color, int x, int y, int width, int height)
{
	g_p_back_buffer->FillRect(real_color, _ToF16(x), _ToF16(y), _ToF16(width), _ToF16(height));
}

void gfx::BitBlt(int x_dest, int y_dest, IGfxSurface* p_surface, int x_sour, int y_sour, int w_sour, int h_sour)
{
	g_p_back_buffer->BitBlt(_ToF16(x_dest), _ToF16(y_dest), p_surface, _ToF16(x_sour), _ToF16(y_sour), _ToF16(w_sour), _ToF16(h_sour));
}

void gfx::BlendBlt(int x_dest, int y_dest, IGfxSurface* p_surface, int x_sour, int y_sour, int w_sour, int h_sour, float opacity)
{
	if (opacity > 0.0f)
	{
		if (opacity >= 1.0f)
		{
			g_p_back_buffer->BitBlt(_ToF16(x_dest), _ToF16(y_dest), p_surface, _ToF16(x_sour), _ToF16(y_sour), _ToF16(w_sour), _ToF16(h_sour));
		}
		else
		{
			unsigned long color = ((unsigned long)(opacity * 255)) << 24 | 0xFFFFFF;
			unsigned long color_list[4] = { color, color, color, color };

			g_p_back_buffer->BitBlt(_ToF16(x_dest), _ToF16(y_dest), p_surface, _ToF16(x_sour), _ToF16(y_sour), _ToF16(w_sour), _ToF16(h_sour), color_list);
		}
	}
}

} // namespace erio

