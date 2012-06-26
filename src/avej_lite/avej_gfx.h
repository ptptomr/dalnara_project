
#ifndef __AVEJ_GFX_H__
#define __AVEJ_GFX_H__

/*!
 * \file
 * \ingroup AVEJ library
 * \brief AVEJ library�� graphics interface
 * \author SMgal smgal@hitel.net
 * \date 2006-12-31
*/

#include "avej_base_type.h"

namespace avej_lite
{

struct TFixed16
{
	explicit TFixed16() {}
	explicit TFixed16(int data): m_data(data << 16) {}
	explicit TFixed16(float data): m_data(long(data * 65536.0f)) {}
	explicit TFixed16(double data): m_data(long(data * 65536.0)) {}

	TFixed16 operator+=(const TFixed16& ref)
	{
		this->m_data += ref.m_data;
		return *this;
	}

	TFixed16 operator-=(const TFixed16& ref)
	{
		this->m_data -= ref.m_data;
		return *this;
	}

	long m_data;
};

inline TFixed16 operator+(TFixed16 lhs, TFixed16 rhs)
{
	TFixed16 temp;
	temp.m_data = lhs.m_data + rhs.m_data;

	return temp;
}

inline TFixed16 operator-(TFixed16 lhs, TFixed16 rhs)
{
	TFixed16 temp;
	temp.m_data = lhs.m_data - rhs.m_data;

	return temp;
}

struct TLayerDesc
{
	enum
	{
		OPTION_NONE       = 0x00000000,
		OPTION_DEPTH_TEST = 0x00000001,
		OPTION_DWORD      = 0x7FFFFFFF,
	};
	enum
	{
		LAYER_ATTRIB_OPAQUE       = 0x00000000,
		LAYER_ATTRIB_ALPHA_BLEND  = 0x00000001,
		LAYER_ATTRIB_ALPHA_TEST   = 0x00000002,
		LAYER_ATTRIB_DWORD        = 0x7FFFFFFF,
	};
	unsigned long num_layer;
	unsigned long layer_attrib[MAX_LAYER_IN_SURFACE];
	unsigned long option_flag;
};

//! Graphics�� surface�� ���� interface
/*!
 * \ingroup AVEJ library graphics
*/
class IGfxSurface
{
public:
	//! Surface�� �޸� ��ũ����
	struct TLockDesc
    {
    	void* pMem;          //!< surface �޸��� ù �ּ�
        int   pitch;         //!< surface���� y��ǥ�� 1 �Ƚ��� ������Ű�� ���� �ȼ� ��
    };

	template <typename T>
	struct TPoint
    {
		T x, y;
    };

	struct TSpriteDesc
	{
		const IGfxSurface* ref_surface;
		int x_sour;
		int y_sour;
		int w_sour;
		int h_sour;
	};

	virtual ~IGfxSurface(void) {}

	void SetClipRect(int x, int y, int w, int h) {}

	//! ���� ����� ���̾ �����Ѵ�. 0�� ���� �ֻ����̰� ��ȣ�� �ö󰥼��� �Ʒ��ʿ� ���δ�.
	virtual void  SetActiveLayer(unsigned long layer) = 0;

	//! Surface�� ������ �簢 ������ ������ ������ ä���.
	virtual bool  FillRect(unsigned long color, TFixed16 x, TFixed16 y, TFixed16 w, TFixed16 h) = 0;
	virtual bool  FillRect(unsigned long color, int x, int y, int width, int height) = 0;
	virtual bool  FillRect(unsigned long color, TPoint<TFixed16> point[]) = 0;
	virtual bool  FillRect(unsigned long color, TPoint<int> point[]) = 0;

	//! Surface�� ���� �ߴ´�.
	virtual bool  DrawLine(unsigned long color, TFixed16 x1, TFixed16 y1, TFixed16 x2, TFixed16 y2) = 0;
	virtual bool  DrawGradLine(unsigned long color1, unsigned long color2, TFixed16 x1, TFixed16 y1, TFixed16 x2, TFixed16 y2) = 0;

	//! Surface�� �ٸ� surface�� ������ �簢 ������ �����Ѵ�.
	virtual bool  BitBlt(TFixed16 x, TFixed16 y, const IGfxSurface* ref_surface, TFixed16 x_sour, TFixed16 y_sour, TFixed16 w_sour, TFixed16 h_sour, const unsigned long color[] = 0) = 0;
	virtual bool  BitBlt(int x, int y, const IGfxSurface* ref_surface, int x_sour, int y_sour, int w_sour, int h_sour, const unsigned long color[] = 0) = 0;
	virtual bool  BitBlt(int x, int y, const TSpriteDesc& sprite_desc, const unsigned long color[] = 0) = 0;

	//! 
	virtual bool  StretchBlt(TFixed16 x_dest, TFixed16 y_dest, TFixed16 w_dest, TFixed16 h_dest, const IGfxSurface* ref_surface, int x_sour, int y_sour, int w_sour, int h_sour, const unsigned long color[] = 0) = 0;
	virtual bool  StretchBlt(int x_dest, int y_dest, int w_dest, int h_dest, const IGfxSurface* ref_surface, int x_sour, int y_sour, int w_sour, int h_sour, const unsigned long color[] = 0) = 0;
	virtual bool  StretchBlt(int x_dest, int y_dest, int w_dest, int h_dest, const TSpriteDesc& sprite_desc, const unsigned long color[] = 0) = 0;

	//! Surface�� �ڵ��� ��´�.
	virtual void* Handle(void) const = 0;
	//! ARGB8888 ������ �÷��� ���� surface�� �ش� �÷� ���� ��´�.
	virtual unsigned long Color(unsigned char a, unsigned char r, unsigned char g, unsigned char b) = 0;
	//! ARGB8888 ������ �÷��� ���� surface�� �ش� �÷� ���� ��´�.
	virtual unsigned long Color(unsigned long absColor) = 0;
};

//! Graphics�� device�� ���� interface
/*!
 * \ingroup AVEJ library graphics
*/
class IGfxDevice
{
public:
	//! Device�� �Ӽ��� ��Ÿ���� ���� ��ũ����
	struct TDeviceDesc
    {
    	int  width;    //!< Device ������ ���� ũ��
    	int  height;   //!< Device ������ ���� ũ��
    	int  pitch;    //!< Device�� ���ۿ��� y��ǥ�� 1 �ȼ��� ������Ű�� ���� �ȼ� ��
    	int  depth;    //!< Device ������ �� �ȼ��� �����ϴ� ��Ʈ ��
    	int  format;   //!< Device ������ �ȼ� ����
    };

	virtual ~IGfxDevice(void) {}

	//! Device�� �׸��� �׸� �� �ֵ��� �غ��Ѵ�.
    virtual bool  BeginDraw(bool clear_flag = true) = 0;
	//! Device�� �׸��� �� �׷����� �˷� �ش�.
    virtual bool  EndDraw(void) = 0;
	//! Device�� �׷��� �׸��� ȭ������ ��Ÿ����.
    virtual bool  Flip(void) = 0;
	//! Device�� ����۸� ���� �´�.
    virtual bool  GetSurface(IGfxSurface** ppSurface) = 0;

	//! Device�� ��� ������ surface�� descriptor�� ���� �����Ѵ�.
	virtual bool  CreateSurface(const TSurfaceDesc& surface_desc, IGfxSurface** pp_surface) = 0;

	//! Device�� ��� ������ surface�� �̹��� ���Ϸκ��� �����Ѵ�.
    virtual bool  CreateSurfaceFrom(const char* szFileName, IGfxSurface** ppSurface) = 0;
	virtual bool  CreateSurfaceFrom(const unsigned char* sz_stream, unsigned long stream_size, IGfxSurface** pp_surface) = 0;
	//! Device�� �Ӽ��� ��´�.
    virtual bool  GetDesc(TDeviceDesc& desc) = 0;
	//! 
    virtual void  SetLayerDesc(TLayerDesc& layer_desc) = 0;
	//! 
    virtual void  GetLayerDesc(TLayerDesc& layer_desc) = 0;

	//! Device�� �ν��Ͻ��� ��´�.
    static IGfxDevice* GetInstance(void);
	//! Device�� �ν��Ͻ��� ����� �������� �˷� �ش�.
    static int Release(void);
};

/*!
\fn bool IGfxSurface::AssignType(EType type, unsigned long color = 0);

\param type [in]
	Surface�� ��� ����
\param color [in]
	Surface�� �÷� Ű. \p type�� \p TYPE_COLORKEY�϶��� ��ȿ�ϴ�.

\return �Լ��� �����ϸ� \c true �� �����ְ� �����ϸ� \c false �� �����ش�.

\remark
\p TYPE_NONE�� �����Ǹ� �� surface�� �׻� ���� ���� ����� �ȴ�. \n
\p TYPE_COLORKEY�� �����Ǹ� \c color���� ������ ���� �÷�Ű�� �����ؼ� ����Ѵ�. \n
\p TYPE_COLORKEY_AUTO�� �����Ǹ� (0,0)�� �ȼ� ���� �÷�Ű�� �����ؼ� ����Ѵ�. \n
\p TYPE_ALPHA�� �����Ǹ� �� �ȼ��� ���ǵǾ� �ִ� ���� �������� ���� �ؼ� ����Ѵ�.

\par ����:
\code
IGfxDevice*  pGfxDevice;
IGfxSurface* pSprite;

pGfxDevice = IGfxDevice::GetInstance();

pGfxDevice->CreateSurfaceFrom("./tile.bmp", &pSprite);
pSprite->AssignType(IGfxSurface::TYPE_COLORKEY_AUTO);
\endcode

\image html exmaple.png "������ ���"

\see IGfxSurface::BitBlt()
*/

} // namespace avej_lite

#endif // #ifndef __AVEJ_GFX_H__
