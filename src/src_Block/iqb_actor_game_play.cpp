
////////////////////////////////////////////////////////////////////////////////
// uses

#include "iqb_z_config.h"
#include "iqb_actor.h"
#include "iqb_base_gfx.h"
#include "iqb_class_3d_vertext.h"
#include "iqb_class_3d_texture.h"
#include "iqb_class_3d_attrib.h"
#include "iqb_data_map.h"
#include <map>
#include <math.h>

using namespace avej_lite;

namespace
{
	using namespace erio;

	struct TBounds
	{
		float x1;
		float y1;
		float x2;
		float y2;

		TBounds()
		{
		}

		TBounds(float _x1, float _y1, float _x2, float _y2)
			: x1(_x1)
			, y1(_y1)
			, x2(_x2)
			, y2(_y2)
		{
		}
	};

	void ConvertIsometricToVertice(TVertexBuffer<VERTEXFORMAT_POS_NOR_TEX>::TVertexData vertices[], unsigned int count, TBounds srcRect, IDirect3DTexture9* pTexture)
	{
		const float MAGIC_NUMBER = 23.0f;
		const float readWidth  = sqrtf(powf(MAGIC_NUMBER,2.0f)+powf(srcRect.x2-srcRect.x1,2.0f)) / 2.0f;
		const float TALL = (srcRect.y2 - srcRect.y1 - MAGIC_NUMBER) / readWidth;

		// Vertex position 정보 만들기
		{
			vertices[ 4].position = D3DVECTOR3(-0.5f, 0.0f, 0.5f);
			vertices[ 5].position = D3DVECTOR3(-0.5f, TALL, 0.5f);
			vertices[ 6].position = D3DVECTOR3(-0.5f, 0.0f,-0.5f);
			vertices[ 7].position = D3DVECTOR3(-0.5f, TALL,-0.5f);
			vertices[ 8].position = D3DVECTOR3( 0.5f, 0.0f,-0.5f);
			vertices[ 9].position = D3DVECTOR3( 0.5f, TALL,-0.5f);
			vertices[10].position = D3DVECTOR3( 0.5f, 0.0f, 0.5f);
			vertices[11].position = D3DVECTOR3( 0.5f, TALL, 0.5f);
			vertices[12].position = vertices[ 4].position;
			vertices[13].position = vertices[ 5].position;
			//
			vertices[ 0].position = vertices[ 5].position;
			vertices[ 1].position = vertices[ 7].position;
			vertices[ 2].position = vertices[ 9].position;
			vertices[ 3].position = vertices[11].position;
		}

		// Vertex normal 정보 만들기
		{
			for (int i = 0; i < count; i++)
				D3DXVec3Normalize(&vertices[i].normal, &vertices[i].position);
		}

		// Vertex UV 정보 만들기
		{
			D3DSURFACE_DESC desc;
			pTexture->GetLevelDesc(0, &desc);

			float wTexture = float(desc.Width);
			float hTexture = float(desc.Height);

			float srcRect_x0 = (srcRect.x1 + srcRect.x2) / 2.0f;

			/*
				(x1,y2) -> (x1,y1) -> (x2,y2) -> (x2,y1) 순서
			*/
			vertices[ 4].tex_pos = TD3DVector2(srcRect.x1 + 0.5f, srcRect.y2 - MAGIC_NUMBER / 2.0f);
			vertices[ 5].tex_pos = TD3DVector2(srcRect.x1 + 0.5f, srcRect.y1 + MAGIC_NUMBER / 2.0f);
			vertices[ 6].tex_pos = TD3DVector2(srcRect_x0,        srcRect.y2 + 0.5f);
			vertices[ 7].tex_pos = TD3DVector2(srcRect_x0,        srcRect.y1 + MAGIC_NUMBER + 0.5f);
			vertices[ 8].tex_pos = TD3DVector2(srcRect.x2 - 0.5f, srcRect.y2 - MAGIC_NUMBER / 2.0f);
			vertices[ 9].tex_pos = TD3DVector2(srcRect.x2 - 0.5f, srcRect.y1 + MAGIC_NUMBER / 2.0f);
			// 반대편 기둥
			vertices[10].tex_pos = vertices[ 6].tex_pos;
			vertices[11].tex_pos = vertices[ 7].tex_pos;
			vertices[12].tex_pos = vertices[ 4].tex_pos;
			vertices[13].tex_pos = vertices[ 5].tex_pos;
			// 꼭대기
			vertices[ 0].tex_pos = vertices[ 5].tex_pos;
			vertices[ 1].tex_pos = vertices[ 7].tex_pos;
			vertices[ 2].tex_pos = TD3DVector2(srcRect.x2 - 0.5f, srcRect.y1 + MAGIC_NUMBER / 2.0f);
			vertices[ 3].tex_pos = TD3DVector2(srcRect_x0, srcRect.y1);

			for (int i = 0; i < count; i++)
			{
				vertices[i].tex_pos.x /= wTexture;
				vertices[i].tex_pos.y /= hTexture;
			}
		}
	}
}

namespace erio
{
	extern IDirect3DDevice9* g_p_d3d_device;

	const char* MAP_DATA[] =
	{
		"00000000000",
		"04111100300",
		"01000000330",
		"01022220300",
		"01000020000",
		"01000020000",
		"00000022222",
		"50000000200",
		"00300000200",
		"03333300000",
		"00300000000",
		NULL,
	};

	typedef iu::vector<CSmPlayer*> TPlayerList;

	TPlayerList playerList;

	CTileMap m_map(MAP_DATA, -5, -5);

	iu::shared_ptr<CTexture> sprite;
	std::map<EVertexFormat, TShaderSet> shaderSet;

	TVertexBuffer<VERTEXFORMAT_POS_TEX>* pVbCharacter = 0;
	TVertexIndexBuffer<VERTEXFORMAT_POS_NOR_TEX>* pVbTile = 0;
	TVertexBuffer<VERTEXFORMAT_POS_DIF>* pVbTemp = 0;

	TVertexBuffer<VERTEXFORMAT_POS_NOR_TEX>* pVbFixedMap = 0;

	CSmPlayer* GetMainPlayer(void)
	{
		return (playerList.empty()) ? NULL : playerList[1];
	}

	class CRenderMode
	{
	public:
		enum ERenderMode
		{
			SPRITE = 0,
			GHOST
		};

		CRenderMode(ERenderMode mode)
		{

			DWORD factor = 0xFF;

			switch (mode)
			{
			case SPRITE:
				factor = 0xD0;
				g_p_d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
				break;
			case GHOST:
				factor = 0x20;
				g_p_d3d_device->SetRenderState(D3DRS_ZENABLE, false);
				g_p_d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
				break;
			};
			g_p_d3d_device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(factor, 0xFF, 0xFF, 0xFF));
			g_p_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

			g_p_d3d_device->SetRenderState(D3DRS_LIGHTING, 0);

			shader::SetTexFactor(g_p_d3d_device, float(factor) / 255.0f);
		};
		~CRenderMode()
		{
			shader::SetTexFactor(g_p_d3d_device, 1.0f);

			g_p_d3d_device->SetRenderState(D3DRS_LIGHTING, 1);

			g_p_d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);

			g_p_d3d_device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(0xFF, 0xFF, 0xFF, 0xFF));
			g_p_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			g_p_d3d_device->SetRenderState(D3DRS_ZENABLE, true);
		};
	};


////////////////////////////////////////////////////////////////////////////////
// callback

namespace game_play
{
	CSm3DAttrib* p_d3d_attrib = 0;

	const int NUM_OF_VERTICES = 14;
	avej_lite::IGfxSurface* p_res_sprite = 0;

	void SetupMatrices(void)
	{
		TD3DMatrix matWorld;

		D3DXMatrixIdentity(&matWorld);
		g_p_d3d_device->SetTransform(D3DTS_WORLD, &matWorld);
	}

	void Test(int mode)
	{
		const int MAX_FACE_INC = 8;

		CRenderMode state((CRenderMode::ERenderMode)mode);
		
		CSmPlayer* pMainPlayer = GetMainPlayer();

		{
			single angle = p_d3d_attrib->GetCamera()->GetAngle();

			pVbCharacter->Begin();

			TD3DMatrix matView;
			TD3DMatrix matProj;
			TD3DMatrix matWV;
			TD3DMatrix matWVP;
			TD3DMatrix matVP;

			g_p_d3d_device->GetTransform(D3DTS_VIEW, &matView);
			g_p_d3d_device->GetTransform(D3DTS_PROJECTION, &matProj);

			D3DXMatrixMultiply(&matVP, &matView, &matProj);
			D3DXMatrixTranspose(&matVP, &matVP);

			g_p_d3d_device->SetVertexShaderConstantF(16, (float*)&matVP, 4);

			for (TPlayerList::iterator obj = playerList.begin(); obj < playerList.end(); ++obj)
			{
				{
					TD3DMatrix matWorld, temp;
					D3DXMatrixIdentity(&matWorld);
					D3DXMatrixScaling(&matWorld, 1.0f, 1.8f, 1.0f);
					D3DXMatrixRotationY(&temp, angle);
					D3DXMatrixMultiply(&matWorld, &matWorld, &temp);
					D3DXMatrixTranslation(&temp, 1.0f * (*obj)->attribute.pos.x - pMainPlayer->attribute.pos.x, 0.26f, 1.0f * (*obj)->attribute.pos.y - pMainPlayer->attribute.pos.y);
					D3DXMatrixMultiply(&matWorld, &matWorld, &temp);

					g_p_d3d_device->SetTransform(D3DTS_WORLD, &matWorld);

					{
						D3DXMatrixMultiply(&matWV, &matWorld, &matView);
						D3DXMatrixMultiply(&matWVP, &matWV, &matProj);

						D3DXMatrixTranspose(&matWVP, &matWVP);
						g_p_d3d_device->SetVertexShaderConstantF(4, (float*)&matWVP, 4);

						D3DXMatrixTranspose(&matWV, &matWV);
						g_p_d3d_device->SetVertexShaderConstantF(8, (float*)&matWV, 4);

						D3DXMatrixTranspose(&matWorld, &matWorld);
						g_p_d3d_device->SetVertexShaderConstantF(12, (float*)&matWorld, 4);
					}
				}

				/*
				빌보드를 위한 행렬은 미리 계산되어서 singleton이 되어야...

				캐릭터에는 vertex buffer와 그 buffer를 해석하는(character의 동작 방식에 맞게) 방법이
				같이 전달되어야 한다 그래서 아래와 같은 계산식도 일반화될 수 있어야 한다.

				그 vertex buffer의 몇번째 인덱스 부터인지,
				8방향에 대한 frame을 꺼내올 수 있어야 하고, frame의 진행이 repeat인지 reverse인지 내부적으로 설정되어야 한다.
				*/

				int ixFace = (*obj)->attribute.face * 2 + (*obj)->attribute.face_inc;
				assert(ixFace >= 0 && ixFace < MAX_FACE_INC);

				pVbCharacter->DrawPrimitive(D3DPT_TRIANGLESTRIP, ixFace*4, 2);
			}

			pVbCharacter->End();
		}
	}

	bool OnCreate(void)
	{
		p_d3d_attrib = new CSm3DAttrib(g_p_d3d_device, 800.0f / 480.0f);

		playerList.push_back(CreateCharacter(0, 0.0f, 0.0f));
		playerList.push_back(CreateCharacter(1, 2.0f, -4.0f));

		sprite = iu::shared_ptr<CTexture>(new CTexture(g_p_d3d_device, "NewNeto1_512_256.tga"));
		if (sprite->m_p_texture == 0)
		{
			sprite = iu::shared_ptr<CTexture>(new CTexture(g_p_d3d_device, "res_Block/NewNeto1_512_256.tga"));
		}

		p_res_sprite = sprite->m_p_texture->m_p_surface;

		const int MAX_FACE_INC = 8;
		{
			struct TFRect
			{
				float x1, y1, x2, y2;
			};

			const TFRect srcRect[MAX_FACE_INC] =
			{
				// ^
				{261.0f / 512.0f,  59.0f / 256.0f, 298.0f / 512.0f, 116.0f / 256.0f},
				{299.0f / 512.0f,  59.0f / 256.0f, 336.0f / 512.0f, 116.0f / 256.0f},
				// _
				{298.0f / 512.0f,  59.0f / 256.0f, 261.0f / 512.0f, 116.0f / 256.0f},
				{336.0f / 512.0f,  59.0f / 256.0f, 299.0f / 512.0f, 116.0f / 256.0f},
				// ->
				{298.0f / 512.0f,   1.0f / 256.0f, 261.0f / 512.0f,  58.0f / 256.0f},
				{336.0f / 512.0f,   1.0f / 256.0f, 299.0f / 512.0f,  58.0f / 256.0f},
				// <-
				{261.0f / 512.0f,   1.0f / 256.0f, 298.0f / 512.0f,  58.0f / 256.0f},
				{299.0f / 512.0f,   1.0f / 256.0f, 336.0f / 512.0f,  58.0f / 256.0f},
			};

			TVertexBuffer<VERTEXFORMAT_POS_TEX>::TVertexData vertices[MAX_FACE_INC][4];

			for (int i = 0; i < MAX_FACE_INC; i++)
			{
				vertices[i][0].position = D3DVECTOR3(-0.5f, 1.0f, 0.0f);
				vertices[i][0].tex_pos.x = srcRect[i].x1;
				vertices[i][0].tex_pos.y = srcRect[i].y1;
				vertices[i][1].position = D3DVECTOR3( 0.5f, 1.0f, 0.0f);
				vertices[i][1].tex_pos.x = srcRect[i].x2;
				vertices[i][1].tex_pos.y = srcRect[i].y1;
				vertices[i][2].position = D3DVECTOR3(vertices[i][0].position.x, 0.0f, vertices[i][0].position.z);
				vertices[i][2].tex_pos.x = srcRect[i].x1;
				vertices[i][2].tex_pos.y = srcRect[i].y2;
				vertices[i][3].position = D3DVECTOR3(vertices[i][1].position.x, 0.0f, vertices[i][1].position.z);
				vertices[i][3].tex_pos.x = srcRect[i].x2;
				vertices[i][3].tex_pos.y = srcRect[i].y2;
			}

			pVbCharacter = new TVertexBuffer<VERTEXFORMAT_POS_TEX>(g_p_d3d_device, &vertices[0][0], MAX_FACE_INC, 4, sprite);
			pVbCharacter->SetShader(shaderSet[VERTEXFORMAT_POS_TEX]);
		}

		{
			const unsigned int NUM_OF_VERTICES = 14;

			TVertexBuffer<VERTEXFORMAT_POS_NOR_TEX>::TVertexData vertices[7][NUM_OF_VERTICES];

			ConvertIsometricToVertice(vertices[0], NUM_OF_VERTICES, TBounds( 66.0f, 105.0f, 130.0f, 138.0f), sprite.get()->m_p_texture);
			ConvertIsometricToVertice(vertices[1], NUM_OF_VERTICES, TBounds( 66.0f, 139.0f, 130.0f, 174.0f), sprite.get()->m_p_texture);
			ConvertIsometricToVertice(vertices[2], NUM_OF_VERTICES, TBounds( 66.0f,   1.0f, 130.0f, 104.0f), sprite.get()->m_p_texture);
			ConvertIsometricToVertice(vertices[3], NUM_OF_VERTICES, TBounds(131.0f,   1.0f, 195.0f, 104.0f), sprite.get()->m_p_texture);
			ConvertIsometricToVertice(vertices[4], NUM_OF_VERTICES, TBounds(199.0f,   1.0f, 257.0f,  57.0f), sprite.get()->m_p_texture);
			ConvertIsometricToVertice(vertices[5], NUM_OF_VERTICES, TBounds(199.0f,  58.0f, 257.0f, 111.0f), sprite.get()->m_p_texture);

			short indice[10][3] =
			{
				{ 1, 0, 2}, { 2, 0, 3},
				{ 4, 5, 6}, { 6, 5, 7},
				{ 6, 7, 8}, { 8, 7, 9},
				{ 8, 9,10}, {10, 9,11},
				{10,11,12}, {12,11,13},
			};
			
			pVbTile = new TVertexIndexBuffer<VERTEXFORMAT_POS_NOR_TEX>(g_p_d3d_device, &vertices[0][0], 7, NUM_OF_VERTICES, &indice[0][0], 30, sprite);
			pVbTile->SetShader(shaderSet[VERTEXFORMAT_POS_NOR_TEX]);

			{
				const int RADIUS_WIDTH  = 14;
				const int RADIUS_HEIGHT = 14;
				TVertexBuffer<VERTEXFORMAT_POS_NOR_TEX>::TVertexData vertices_fm[2*RADIUS_WIDTH+1][2*RADIUS_HEIGHT+1][2*3];

				for (int y = -RADIUS_HEIGHT; y <= RADIUS_HEIGHT; y++)
				for (int x = -RADIUS_WIDTH; x <= RADIUS_WIDTH; x++)
				{
					float x_base = 1.0f * x;
					float y_base = 1.0f * y;

					TVertexBuffer<VERTEXFORMAT_POS_NOR_TEX>::TVertexData* p_vertext_data = &vertices_fm[x+RADIUS_WIDTH][y+RADIUS_HEIGHT][0];

					for (int i = 0; i < 2; i++)
					{
						int tile = m_map.GetTile(x, y);

						p_vertext_data[3*i+0] = vertices[tile][indice[i][0]];
						p_vertext_data[3*i+1] = vertices[tile][indice[i][1]];
						p_vertext_data[3*i+2] = vertices[tile][indice[i][2]];

						p_vertext_data[3*i+0].position.x += x_base;
						p_vertext_data[3*i+0].position.z += y_base;
						p_vertext_data[3*i+1].position.x += x_base;
						p_vertext_data[3*i+1].position.z += y_base;
						p_vertext_data[3*i+2].position.x += x_base;
						p_vertext_data[3*i+2].position.z += y_base;
					}
				}

				pVbFixedMap = new TVertexBuffer<VERTEXFORMAT_POS_NOR_TEX>(g_p_d3d_device, &vertices_fm[0][0][0], (2*RADIUS_WIDTH+1) * (2*RADIUS_HEIGHT+1), 2*3, sprite);
				pVbFixedMap->SetShader(shaderSet[VERTEXFORMAT_POS_NOR_TEX]);
			}
		}

		{
			TVertexBuffer<VERTEXFORMAT_POS_DIF>::TVertexData vertices[2][3] =
			{
				{
					{ D3DVECTOR3(0.0f, 0.0f, 0.0f), { 1.0f, 0.0f, 0.0f, 1.0f } },
					{ D3DVECTOR3(0.0f, 0.0f, 1.0f), { 0.0f, 1.0f, 0.0f, 1.0f } },
					{ D3DVECTOR3(1.0f, 0.0f, 0.0f), { 0.0f, 0.0f, 1.0f, 1.0f } }
				},
				{
					{ D3DVECTOR3(1.0f, 0.0f, 0.0f), { 0.0f, 0.0f, 1.0f, 1.0f } },
					{ D3DVECTOR3(0.0f, 0.0f, 1.0f), { 0.0f, 1.0f, 0.0f, 1.0f } },
					{ D3DVECTOR3(1.0f, 0.0f, 1.0f), { 1.0f, 1.0f, 0.0f, 1.0f } }
				}
			};

			pVbTemp = new TVertexBuffer<VERTEXFORMAT_POS_DIF>(g_p_d3d_device, &vertices[0][0], 2, 3, sprite);
			pVbTemp->SetShader(shaderSet[VERTEXFORMAT_POS_DIF]);
		}

#if 1
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_COLOR_MATERIAL);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_ALPHA_TEST);
		glAlphaFuncx(GL_GREATER, 0.0f);
#else
		g_p_d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_p_d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		g_p_d3d_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

		g_p_d3d_device->SetRenderState(D3DRS_ALPHATESTENABLE, 1);
		g_p_d3d_device->SetRenderState(D3DRS_ALPHAREF, 0x00);
		g_p_d3d_device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		g_p_d3d_device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		g_p_d3d_device->SetTextureStageState(0, D3DTSS_COLORARG1,     D3DTA_TEXTURE);
		g_p_d3d_device->SetTextureStageState(0, D3DTSS_COLORARG2,     D3DTA_DIFFUSE);
		g_p_d3d_device->SetTextureStageState(0, D3DTSS_COLOROP,       D3DTOP_MODULATE);
		g_p_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG1,     D3DTA_TEXTURE);
		g_p_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG2,     D3DTA_DIFFUSE);
		g_p_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAOP,       D3DTOP_MODULATE);
#endif
		glViewport(0, 0, 800, 480);

		return true;
	}

	bool OnDestory(void)
	{
		for (TPlayerList::iterator obj = playerList.begin(); obj < playerList.end(); ++obj)
		{
			delete *obj;
		}

		playerList.clear();

		sprite.setNull();

		delete pVbCharacter;
		pVbCharacter = 0;

		delete pVbTile;
		pVbTile = 0;

		delete pVbFixedMap;
		pVbFixedMap = 0;

		delete pVbTemp;
		pVbTemp = 0;

		delete p_d3d_attrib;
		p_d3d_attrib = 0;

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
			const float MOVE = 0.075f;
			float dx = 0.0f; 
			float dy = 0.0f; 

			if (input_device.IsKeyHeldDown(avej_lite::INPUT_KEY_LEFT))
				dx = -MOVE;
			if (input_device.IsKeyHeldDown(avej_lite::INPUT_KEY_RIGHT))
				dx = +MOVE;
			if (input_device.IsKeyHeldDown(avej_lite::INPUT_KEY_UP))
				dy = +MOVE;
			if (input_device.IsKeyHeldDown(avej_lite::INPUT_KEY_DOWN))
				dy = -MOVE;

			if ((dx != 0.0f) || (dy != 0.0f))
			{
				const float RADIUS = 0.4f;

				float xCenter  = GetMainPlayer()->attribute.pos.x + dx + 0.5f;
				float yCenter  = GetMainPlayer()->attribute.pos.y + dy + 0.5f;

				bool  walkable = m_map.IsMoveableMapRect(xCenter, yCenter, RADIUS);

				if (walkable)
				{
					GetMainPlayer()->Move(dx, dy);
				}
				else
				{
					walkable = m_map.IsMoveableMapRect(xCenter, GetMainPlayer()->attribute.pos.y + 0.5f, RADIUS);
					if (walkable && (dx != 0.0f))
					{
						GetMainPlayer()->Move(dx, 0.0f);
					}
					else
					{
						walkable = m_map.IsMoveableMapRect(GetMainPlayer()->attribute.pos.x + 0.5f, yCenter, RADIUS);
						if (walkable && (dy != 0.0f))
							GetMainPlayer()->Move(0.0f, dy);
					}
				}
			}
		}

		p_d3d_attrib->Process();

		g_p_d3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
						  D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

		if (_SUCCEEDED(g_p_d3d_device->BeginScene()))
		{
			SetupMatrices();

			{
				CSmPlayer* pMainPlayer = GetMainPlayer();

				{
					const int MAP_RANGE = 14;

					TD3DMatrix matView;
					TD3DMatrix matProj;
					TD3DMatrix matWV;
					TD3DMatrix matWVP;
					TD3DMatrix matVP;

					g_p_d3d_device->GetTransform(D3DTS_VIEW, &matView);
					g_p_d3d_device->GetTransform(D3DTS_PROJECTION, &matProj);

					D3DXMatrixMultiply(&matVP, &matView, &matProj);
					D3DXMatrixTranspose(&matVP, &matVP);

					g_p_d3d_device->SetVertexShaderConstantF(16, (float*)&matVP, 4);
#if 1
					pVbFixedMap->Begin();
					{
						float x_offset = pMainPlayer->attribute.pos.x - floorf(pMainPlayer->attribute.pos.x);
						float y_offset = pMainPlayer->attribute.pos.y - floorf(pMainPlayer->attribute.pos.y);

						TD3DMatrix matWorld;
						D3DXMatrixIdentity(&matWorld);
						D3DXMatrixTranslation(&matWorld, - pMainPlayer->attribute.pos.x, 0.0f, - pMainPlayer->attribute.pos.y);
						g_p_d3d_device->SetTransform(D3DTS_WORLD, &matWorld);

						pVbFixedMap->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2*29*29);
					}
					pVbFixedMap->End();
#else
					pVbTile->Begin();

					// 바닥 그리기
					{
						float x_offset = pMainPlayer->attribute.pos.x - floorf(pMainPlayer->attribute.pos.x);
						float y_offset = pMainPlayer->attribute.pos.y - floorf(pMainPlayer->attribute.pos.y);

						for (int y = -MAP_RANGE; y <= MAP_RANGE; y++)
						for (int x = -MAP_RANGE; x <= MAP_RANGE; x++)
						{
							int tile = m_map.GetTile(x, y);

							TD3DMatrix matWorld;
							D3DXMatrixIdentity(&matWorld);
							D3DXMatrixTranslation(&matWorld, 1.0f * x - x_offset, 0.0f, 1.0f * y - y_offset);
							g_p_d3d_device->SetTransform(D3DTS_WORLD, &matWorld);

							{
								D3DXMatrixMultiply(&matWV, &matWorld, &matView);
								D3DXMatrixMultiply(&matWVP, &matWV, &matProj);

								D3DXMatrixTranspose(&matWVP, &matWVP);
								g_p_d3d_device->SetVertexShaderConstantF(4, (float*)&matWVP, 4);

								D3DXMatrixTranspose(&matWV, &matWV);
								g_p_d3d_device->SetVertexShaderConstantF(8, (float*)&matWV, 4);

								D3DXMatrixTranspose(&matWorld, &matWorld);
								g_p_d3d_device->SetVertexShaderConstantF(12, (float*)&matWorld, 4);
							}

							pVbTile->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, tile*NUM_OF_VERTICES, 10);
							//pVbTemp->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
						}
					}
					pVbTile->End();
#endif
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

					pVbTile->Begin();

					// 기둥 그리기
					{
						for (int y = -MAP_RANGE; y <= MAP_RANGE; y++)
						for (int x = -MAP_RANGE; x <= MAP_RANGE; x++)
						{
							int wall = m_map.GetWall(x, y);
							if (wall == 0)
								continue;

							TD3DMatrix matWorld;
							D3DXMatrixIdentity(&matWorld);
							D3DXMatrixTranslation(&matWorld, 1.0f * x - pMainPlayer->attribute.pos.x, 0.26f, 1.0f * y - pMainPlayer->attribute.pos.y);
							g_p_d3d_device->SetTransform(D3DTS_WORLD, &matWorld);

							{
								D3DXMatrixMultiply(&matWV, &matWorld, &matView);
								D3DXMatrixMultiply(&matWVP, &matWV, &matProj);

								D3DXMatrixTranspose(&matWVP, &matWVP);
								g_p_d3d_device->SetVertexShaderConstantF(4, (float*)&matWVP, 4);

								D3DXMatrixTranspose(&matWV, &matWV);
								g_p_d3d_device->SetVertexShaderConstantF(8, (float*)&matWV, 4);

								D3DXMatrixTranspose(&matWorld, &matWorld);
								g_p_d3d_device->SetVertexShaderConstantF(12, (float*)&matWorld, 4);
							}

							pVbTile->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, wall*NUM_OF_VERTICES, 10);
						}
					}
pVbTile->Flush();
					pVbTile->End();
				}

				SetupMatrices();
			}

			Test(0);
			Test(1);

			g_p_d3d_device->EndScene();
		}

		g_p_d3d_device->Present(NULL, NULL, 0, NULL);

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
