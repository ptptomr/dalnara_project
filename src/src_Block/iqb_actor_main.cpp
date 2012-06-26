
////////////////////////////////////////////////////////////////////////////////
// uses

#include "iqb_z_config.h"
#include "iqb_z_util.h"
#include "iqb_base_gfx.h"
#include "iqb_class_3d.h"
//#include "iqb_actor.h"
#include "iqb_class_3d_vertext.h"
#include "iqb_class_3d_attrib.h"

using namespace avej_lite;

namespace erio
{

////////////////////////////////////////////////////////////////////////////////
// static variable

static TLayerDesc s_layer_option =
{
	1,
	{
		avej_lite::TLayerDesc::LAYER_ATTRIB_ALPHA_TEST | avej_lite::TLayerDesc::LAYER_ATTRIB_ALPHA_BLEND
	},
	avej_lite::TLayerDesc::OPTION_NONE
};

static bool   s_state_changing = false;
static TState s_required_state = STATE_EXIT;

////////////////////////////////////////////////////////////////////////////////
// global function

void g_ChangeState(TState state)
{
	s_required_state = state;
	s_state_changing = true;
}

////////////////////////////////////////////////////////////////////////////////
// main

namespace title     { extern AppCallback callback; }
namespace game_play { extern AppCallback callback; }

const AppCallback* g_app_callback_list[STATE_EXIT+1] =
{
	// 	STATE_TITLE
	&title::callback,
	// 	STATE_GAME_PLAY
	&game_play::callback,
	//  STATE_EXIT
	NULL,
};


IDirect3DDevice9* g_pD3DDevice = 0;
CSm3DAttrib*      g_pD3DAttrib = 0;


class CSystem
{
	enum TProcess
	{
		PROCESS_READY,
		PROCESS_IN
	};

	TProcess  m_process;
	TState    m_current_state;
	IAvejApp* m_p_app;

protected:
	void _Run(TState state)
	{
		while (g_app_callback_list[state])
		{
			util::ClearKeyBuffer();

			IAvejApp* p_app = IAvejApp::GetInstance(*g_app_callback_list[state]);
			SF_ASSERT(p_app);
			
			p_app->Process();
			p_app->Release();

			if (s_state_changing)
			{
				state = s_required_state;
				s_state_changing = false;
			}

			if (s_required_state == STATE_EXIT)
				break;
		}
	}

public:
	CSystem(TState state)
	: m_current_state(state), m_process(PROCESS_READY), m_p_app(0)
	{
		// initialize the graphics system
		g_p_gfx_device = IGfxDevice::GetInstance();
		
		gfx::Init();

		g_p_gfx_device->SetLayerDesc(s_layer_option);
		g_p_gfx_device->GetSurface(&g_p_back_buffer);

		g_pD3DDevice = new IDirect3DDevice9(g_p_gfx_device);
		g_pD3DAttrib = new CSm3DAttrib(g_pD3DDevice, 800.0f / 480.0f);
	}
	~CSystem()
	{
		delete g_pD3DAttrib;
		g_pD3DAttrib = NULL;

		if (g_pD3DDevice)
		{
			g_pD3DDevice->Release();
			g_pD3DDevice = NULL;
		}

		g_p_gfx_device->Release();
	}
	bool Process(void)
	{
		switch (m_process)
		{
		case PROCESS_READY:
			{
				if (g_app_callback_list[m_current_state] == 0)
					return false;

				util::ClearKeyBuffer();

				assert(m_p_app == 0);
				m_p_app = IAvejApp::GetInstance(*g_app_callback_list[m_current_state]);
				assert(m_p_app);

				m_process = PROCESS_IN;
			}
			// pass through
		case PROCESS_IN:
			{
				if (!m_p_app->Process())
				{
					m_p_app->Release();
					m_p_app = 0;

					if (s_state_changing)
					{
						m_current_state = s_required_state;
						s_state_changing = false;
					}

					if (s_required_state == STATE_EXIT)
						return false;

					m_process = PROCESS_READY;
				}
			}
			break;
		}

		return true;
	}
};

} // namespace erio

////////////////////////////////////////////////////////////////////////////////
// main

using namespace erio;

static CSystem* p_system = 0;

static void avej_init()
{
	p_system = new CSystem(STATE_GAME_PLAY);
//	p_system = new CSystem(STATE_TITLE);
}

static void avej_done()
{
	delete p_system;
}

static bool avej_process()
{
	return p_system->Process();
}

#include "avejapp_register.h"

static bool s_Initialize(void* h_window)
{
#if (TARGET_DEVICE == TARGET_BADA)
	avej_lite::util::SetResourcePath("res_Block");
#else
	avej_lite::util::SetResourcePath("../../Res/res_Block");
#endif

//	p_system = new CSystem(STATE_TITLE);
	p_system = new CSystem(STATE_GAME_PLAY);
	return (p_system != 0);
}

static void s_Finalize(void)
{
	delete p_system;
}

static bool s_Process(void)
{
	return p_system->Process();
}

static bool GetAppCallback(TAppCallback& out_callback)
{
	out_callback.Initialize = s_Initialize;
	out_callback.Finalize   = s_Finalize;
	out_callback.Process    = s_Process;

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// main

bool g_Game_Initialize(void)
{
	return s_Initialize(0);
}

void g_Game_Finalize(void)
{
	s_Finalize();
}

bool g_Game_Process(void)
{
	return s_Process();
}

////////////////////////////////////////////////////////////////////////////////
//

REGISTER_APP_1("IQ Block", GetAppCallback);
