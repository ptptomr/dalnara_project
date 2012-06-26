
#ifndef __AVEJ_PIXEL_FORMAT_H__
#define __AVEJ_PIXEL_FORMAT_H__

/*!
 * \file
 * \ingroup base type
 * \brief pixel format�� �����ϱ� ���� �⺻ Ÿ�԰� �׿� ���� ��ƿ��Ƽ
 */

#define DUFFS_DEVICE(operation, count) \
	switch (count & 0x07)              \
	{                                  \
		do {                           \
	case 0 :                           \
		operation;                     \
	case 7 :                           \
		operation;                     \
	case 6 :                           \
		operation;                     \
	case 5 :                           \
		operation;                     \
	case 4 :                           \
		operation;                     \
	case 3 :                           \
		operation;                     \
	case 2 :                           \
		operation;                     \
	case 1 :                           \
		operation;                     \
		} while ((count -= 8) > 0);    \
	}

namespace avej_lite
{
	//! ���� ������ �ȼ� ������ ��� 
	/*!
	 * \ingroup gfx2d
	 */
	enum TPixelFormat
	{
		PIXELFORMAT_ABGR1555,  //!< ABGR�� �� ��Ұ� 1:5:5:5�� ���·� ��Ʈ�� �������� �ִ� �ȼ� ����. 16��Ʈ
		PIXELFORMAT_RGBA5551,  //!< RGBA�� �� ��Ұ� 5:5:5:1�� ���·� ��Ʈ�� �������� �ִ� �ȼ� ����. 16��Ʈ
		PIXELFORMAT_RGB555,    //!< RGB�� �� ��Ұ� 5:5:5�� ���·� ��Ʈ�� �������� �ִ� �ȼ� ����. 16��Ʈ
		PIXELFORMAT_RGB565,    //!< RGB�� �� ��Ұ� 5:6:5�� ���·� ��Ʈ�� �������� �ִ� �ȼ� ����. 16��Ʈ
		PIXELFORMAT_RGBA4444,  //!< ARGB�� �� ��Ұ� 4:4:4:4�� ���·� ��Ʈ�� �������� �ִ� �ȼ� ����. 16��Ʈ
		PIXELFORMAT_ARGB8888,  //!< ARGB�� �� ��Ұ� 8:8:8:8�� ���·� ��Ʈ�� �������� �ִ� �ȼ� ����. 32��Ʈ
		PIXELFORMAT_UNKNOWN    //!< ���� ��Ͽ� ���ǵ��� ���� �ȼ� ����
	};

	template <TPixelFormat pixel_format>
	struct TPixelFormatTraits
	{
		typedef unsigned long TPixel;
	};

	template <>
	struct TPixelFormatTraits<PIXELFORMAT_RGBA4444>
	{
		typedef unsigned short TPixel;
		static TPixel ConvertColor(unsigned long color32bits)
		{
			return TPixel((color32bits & 0x00F00000) >>  8 |
			              (color32bits & 0x0000F000) >>  4 |
			              (color32bits & 0x000000F0) >>  0 |
			              (color32bits & 0xF0000000) >> 28);
		}
	};

	template <TPixelFormat pixel_format>
	struct TPixelInfo
	{
		typedef typename TPixelFormatTraits<pixel_format>::TPixel TPixel;
		static TPixel ConvertColor(unsigned long color32bits)
		{
			return TPixelFormatTraits<pixel_format>::ConvertColor(color32bits);
		}
	};

	////////////////////////////////////////////////////////////////////////////
	// enum TStreamPixelFormat

	enum TStreamPixelFormat
	{
		SOURCE_ARGB8888,
		SOURCE_BGRA8888,
		SOURCE_XBGR8888,
		SOURCE_BGR888,
		SOURCE_PALETTE256,
		SOURCE_MONOCHROME,
		DESTINATION_RGB555,
		DESTINATION_RGB565,
		DESTINATION_RGBA4444,
		DESTINATION_ABGR1555,
		DESTINATION_RGBA5551
	};

	template <TStreamPixelFormat pixel_format>
	struct TStreamPixelFormatTraits
	{
	};
	
	template <>
	struct TStreamPixelFormatTraits<SOURCE_ARGB8888>
	{
		typedef unsigned long TPixel;
	};

	template <>
	struct TStreamPixelFormatTraits<DESTINATION_RGBA4444>
	{
		typedef unsigned short TPixel;
	};

	////////////////////////////////////////////////////////////////////////
	// ConvertColorSpace

	template <TStreamPixelFormat dest_pf, TStreamPixelFormat sour_pf>
	struct ConvertColorSpace
	{
		typedef typename TStreamPixelFormatTraits<dest_pf>::TPixel TDestPixel;
		typedef typename TStreamPixelFormatTraits<sour_pf>::TPixel TSourPixel;

		ConvertColorSpace(TDestPixel* dest_buffer, TSourPixel* sour_buffer, int num)
		{
			DUFFS_DEVICE(ConvertColorSpaceSub(dest_buffer, sour_buffer), num);
		};

	private:
		inline void ConvertColorSpaceSub(TDestPixel*& dest_buffer, TSourPixel*& sour_buffer);
	};

	template <>
	inline void ConvertColorSpace<DESTINATION_RGBA4444, SOURCE_ARGB8888>::ConvertColorSpaceSub(ConvertColorSpace::TDestPixel*& dest_buffer, ConvertColorSpace::TSourPixel*& sour_buffer)
	{
		ConvertColorSpace::TDestPixel temp = ConvertColorSpace::TDestPixel((*sour_buffer & 0xF0000000) >> 28);
		temp |= ConvertColorSpace::TDestPixel((*sour_buffer & 0x00F00000) >>  8);
		temp |= ConvertColorSpace::TDestPixel((*sour_buffer & 0x0000F000) >>  4);
		temp |= ConvertColorSpace::TDestPixel((*sour_buffer & 0x000000F0) >>  0);
		
		*dest_buffer++ = temp;
		++sour_buffer;
	};

	////////////////////////////////////////////////////////////////////////
	// TToStreamFormat

	template <TPixelFormat pixel_format>
	struct TToStreamFormat
	{
	};

	template <>
	struct TToStreamFormat<PIXELFORMAT_RGBA4444>
	{
		enum { stream_format = DESTINATION_RGBA4444 };
	};

} // namespace avej_lite

#undef DUFFS_DEVICE

#endif // #ifndef __AVEJ_PIXEL_FORMAT_H__
