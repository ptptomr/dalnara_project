
#include "iqb_z_config.h"
#include "iqb_z_util.h"

#pragma warning(disable: 4786)
#include <vector>

void erio::util::ClearKeyBuffer(void)
{
	avej_lite::CInputDevice& input_device = avej_lite::singleton<avej_lite::CInputDevice>::get();
	input_device.UpdateInputState();

	// 키버퍼 소거
	for (avej_lite::TInputKey key = avej_lite::INPUT_KEY_BEGIN; key < avej_lite::INPUT_KEY_END; inc_(key))
		input_device.WasKeyPressed(key);
}

struct erio::util::CTextFileFromRes::TImpl
{
public:
	TImpl(const tchar* sz_text_stream, int size)
	{
		const tchar* p_stream_beg = sz_text_stream;
		const tchar* p_stream_end = sz_text_stream + size;

		const tchar* p_stream = sz_text_stream;

		while (p_stream < p_stream_end)
		{
			if ((*p_stream != 0x0D) && (*p_stream != 0x0A))
			{
				++p_stream;
				continue;
			}

			m_paragraph_list.push_back(std::make_pair(p_stream_beg, p_stream));

			++p_stream;

			// <CR>로 종료
			if (p_stream >= p_stream_end)
				break;

			if (*p_stream == 0x0A)
				++p_stream;

			p_stream_beg = p_stream;
		}

		m_current_paragraph = m_paragraph_list.begin();
	}

	bool IsValid(void)
	{
		if (m_paragraph_list.size() == 0)
			return false;

		return (m_current_paragraph != m_paragraph_list.end());
	}

	bool GetString(tchar* out_str, int str_len)
	{
		if (m_current_paragraph == m_paragraph_list.end())
			return false;

		int sour_len = m_current_paragraph->second - m_current_paragraph->first;
		int copy_len = min(str_len, sour_len);

		STRNCPY(out_str, m_current_paragraph->first, copy_len);

		if (copy_len < str_len)
			out_str[copy_len] = 0;

		++m_current_paragraph;

		return true;
	}

private:
	typedef std::pair<const tchar*, const tchar*> TParagraph;

	std::vector<TParagraph>           m_paragraph_list;
	std::vector<TParagraph>::iterator m_current_paragraph;

};

erio::util::CTextFileFromRes::CTextFileFromRes(const tchar* sz_text_stream, int size)
	: m_p_impl(new TImpl(sz_text_stream, size))
{
}

erio::util::CTextFileFromRes::~CTextFileFromRes()
{
	delete m_p_impl;
}

bool erio::util::CTextFileFromRes::IsValid(void)
{
	return m_p_impl->IsValid();
}

bool erio::util::CTextFileFromRes::GetString(tchar* out_str, int str_len)
{
	return m_p_impl->GetString(out_str, str_len);
}

////////////////////////////////////////////////////////////////////////////////
// ComposeString()

namespace
{
	void s_Int2Str(tchar** pp_buffer, int value)
	{
		if (value == 0)
		{
			*((*pp_buffer)++) = TCHAR('0');
		}
		else
		{
			if (value < 0)
			{
				*((*pp_buffer)++) = TCHAR('-');
				value = -value;
			}

			int count = 0;
			int temp = value;

			while (temp)
			{
				temp = temp / 10;
				++count;
			}

			int num_loop = count;

			while (num_loop-- > 0)
			{
				(*pp_buffer)[num_loop] = (value % 10) + TCHAR('0');
				value = value / 10;
			}

			*pp_buffer += count;
		}
	}
}

void erio::util::ComposeString(tchar p_buffer[], const tchar* sz_format, const int data ...)
{
	if (p_buffer == 0)
		return;

	const int* p_data = &data;

	while (*sz_format)
	{
		if (*sz_format != TCHAR('@'))
		{
			*p_buffer++ = *sz_format++;
			continue;
		}
		++sz_format;
		s_Int2Str(&p_buffer, *p_data++);
	}

	*p_buffer = 0;
}

int erio::util::tstrlen(const tchar* str1)
{
	const tchar* str2 = str1;

	while (*str2++)
		;

	return str2-str1-1;
}

tchar* erio::util::tstrcpy(tchar* dest, const tchar* src)
{
	return util::tstrncpy(dest, src, util::tstrlen(src)+1);
}

tchar* erio::util::tstrncpy(tchar* dest, const tchar* src, size_t n)
{
	size_t i;

	for (i = 0; i < n && src[i]; i++)
		dest[i] = src[i];

	for (; i < n; i++)
		dest[i] = TCHAR('\0');

	return dest;
}

tchar* erio::util::tstrcat(tchar* dest, const tchar* src)
{
	return util::tstrncat(dest, src, util::tstrlen(src)+1);
}

tchar* erio::util::tstrncat(tchar* dest, const tchar* src, size_t n)
{
   size_t dest_len = util::tstrlen(dest);
   size_t i;

   for (i = 0 ; i < n && src[i]; i++)
	   dest[dest_len + i] = src[i];

   dest[dest_len + i] = TCHAR('\0');

   return dest;
}

int erio::util::tstrncmp(const tchar* s1, const tchar* s2, size_t n)
{
	if (n == 0)
		return 0;

	while (*s1 == *s2 && n-- > 0)
	{
		if (*s1 == 0 || n == 0)
			return 0;
		s1++;
		s2++;
	}

	if (n <= 0)
		return 0;

	return (*s1 > *s2) ? 1 : -1;
}

int erio::util::tnumchar(const tchar* str1)
{
	if (sizeof(tchar) == 1)
	{
		return STRLEN(str1);
	}
	else
	{
		int count = 0;

		while (*str1)
		{
			count += (*str1++ < 256) ? 1 : 2;
		}

		return count;
	}
}
