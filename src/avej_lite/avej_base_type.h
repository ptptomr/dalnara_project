
#ifndef __AVEJ_BASE_TYPE_H__
#define __AVEJ_BASE_TYPE_H__

#include "avej_pixel_format.h"

namespace avej_lite
{
	//! ���۸� ǥ���ϱ� ���� �⺻ ����
	/*!
	 * \ingroup gfx2d
	 */
	struct TBufferDesc
	{
		TPixelFormat  pixel_format;    //!< �ȼ� ����
		int           width;           //!< ������ ���� ������ ũ��
		int           height;          //!< ������ ���� ������ ũ��
		int           depth;           //!< �ȼ� ����. �ϳ��� �ȼ��� �����ϱ� ���� ��Ʈ ��
		bool          has_color_key;   //!< ���� ������ �÷�Ű�� ���� �Ǿ� �ִ����� ����
		unsigned long color_key;       //!< ���� ������ �÷�Ű. \c has_color_key�� true�� �� �� ���� ��ȿ��
	};

	//! ���ۿ� ���� �ϱ� ���� �⺻ ����
	/*!
	 * \ingroup gfx2d
	 */
	struct TLockDesc
	{
		void*         p_buffer;        //!< ������ ��ȿ�� ������ ���� �ּ�
		long          bytes_per_line;  //!< (x,y+1)�� (x,y)�� ���� �ּ� ����. 'addr(x,y+1) - addr(x, y)'�� ��
	};

	//! ���ǽ��� �����ϱ� ���� �⺻ ����
	/*!
	 * \ingroup gfx2d
	 */
	struct TSurfaceDesc
	{
		TBufferDesc    buffer_desc;    //!< ���ǽ��� ���� ����
		TLockDesc      lock_desc;      //!< ���ǽ��� ���� ���� �Ӽ� ����
		unsigned long  hw_handle;      //!< ���ǽ��� ���õ� H/W �Ӽ�. �� ���� 0�̸� H/W �Ӽ��� ����
		bool           has_ownership;  //!< ���ǽ� ���ۿ� ���� �������� �ִ����� ����. �������� �ִٸ� ���ǽ� ���� �� ���� �޸𸮵� ���� ����

		//! ���ǽ� ���� �� ȣ�� �Ǵ� �Լ��� ����. �̰��� ���ǵ��� ������ �̸� ���ǵ� ����Ʈ ������ ����
		void          (*Destroy)(TSurfaceDesc& native_surface);
		//! ���ǽ� ���� �� ȣ�� �Ǵ� �Լ��� ����. �̰��� ���ǵ��� ������ \c lock_desc �� ���� ���
		bool          (*Lock)(TSurfaceDesc& native_surface);
		//! ���ǽ� ���� �Ϸ� �� ȣ�� �Ǵ� �Լ��� ����.
		bool          (*Unlock)(TSurfaceDesc& native_surface);
		//! ARGB8888 ������ �÷��� ���ǽ� ���ۿ� �´� �ȼ� �������� ��ȯ�ϴ� �Լ��� ����.
		unsigned long (*ToTargetColor)(unsigned long color_32bits);
		//! �簢 ������ Ư�� �÷��� ä��� ���� ��ü���� �Լ��� ����
		bool          (*FillRect)(int x, int y, unsigned int width, unsigned int height);
		//! ���ǽ����� �簢 ������ �����ϱ� ���� ��ü���� �Լ��� ����
		bool          (*CopyRect)(int x, int y, TSurfaceDesc& src_native_surface, int src_x, int src_y, unsigned int src_width, unsigned int src_height);
	};
}

#endif // #ifndef __AVEJ_BASE_TYPE_H__
