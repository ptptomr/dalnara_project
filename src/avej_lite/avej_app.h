
#ifndef __AVEJ_APP_H
#define __AVEJ_APP_H

/*!
 * \file
 * \ingroup AVEJ library
 * \brief AVEJ library�� graphics interface
 * \author SMgal smgal@hitel.net
 * \date 2006-12-31
*/

#define THIS_CLASS_IS_A_SINGLETON(class_name)      \
	friend class ::avej_lite::singleton<class_name>;    \
	friend class ::avej_lite::auto_deletor<class_name>; \
	class_name();                                  \
	class_name(const class_name&);                 \
	~class_name();                                 \
	class_name& operator=(const class_name&)

#define THIS_STRUCT_IS_A_SINGLETON(struct_name)    \
	private:                                       \
	THIS_CLASS_IS_A_SINGLETON(struct_name);        \
	public:                                        \


namespace avej_lite
{
	template <typename TBaseClass>
	class auto_deletor
	{
	public:
		auto_deletor(TBaseClass* object = 0)
			: m_object(object)
		{
		}
		~auto_deletor(void)
		{
			delete m_object;
		}
		void set(TBaseClass* object)
		{
			m_object = object;
		}

	private:
		TBaseClass* m_object;
	};

	//! �̱��� ��ü�� ����� �ֱ� ���� ���ø� (single thread ����)
	/*!
	 * \ingroup gfx2d
	 */
	template <typename TBaseClass, int kind = 0>
	class singleton
	{
	public:
		static TBaseClass& get(int param = 0)
		{
			if (m_instance == 0)
			{
				m_instance = new TBaseClass;
				m_deletor.set(m_instance);
			}

			return *m_instance;
		}
		// ������� ���� ������ �ʿ��� �� ����Ѵ�.
		static void release(int param = 0)
		{
			if (m_instance)
			{
				delete m_instance;
				m_instance = 0;
				m_deletor.set(m_instance);
			}
		}
	private:
		singleton(void);

		static TBaseClass* m_instance;
		static auto_deletor<TBaseClass> m_deletor;
	};

	template <class TBaseClass, int kind> TBaseClass* singleton<TBaseClass, kind>::m_instance = 0;
	template <class TBaseClass, int kind> auto_deletor<TBaseClass> singleton<TBaseClass, kind>::m_deletor;
} // namespace avej_lite

namespace avej_lite
{
	////////////////////////////////////////////////////////////////////////////
	// class CInputDevice

	//! �Է� ����̽��� Ű ����
	enum TInputKey
	{
		INPUT_KEY_BEGIN = 0,  //!< Ű ������ ������ ���� ���� ��
		INPUT_KEY_UP    = 0,  //!< ���� ���� Ű
		INPUT_KEY_DOWN,       //!< �Ʒ��� ���� Ű
		INPUT_KEY_LEFT,       //!< ���� ���� Ű
		INPUT_KEY_RIGHT,      //!< ������ ���� Ű
		INPUT_KEY_A,          //!< �Ϲ� ���� ��ư A
		INPUT_KEY_B,          //!< �Ϲ� ���� ��ư B
		INPUT_KEY_C,          //!< �Ϲ� ���� ��ư C
		INPUT_KEY_D,          //!< �Ϲ� ���� ��ư D
		INPUT_KEY_L1,         //!< ���� Ʈ���� ��ư 1
		INPUT_KEY_R1,         //!< ������ Ʈ���� ��ư 1
		INPUT_KEY_SYS1,       //!< �ý��� ��ư 1 (start ��)
		INPUT_KEY_SYS2,       //!< �ý��� ��ư 2 (select ��)
		INPUT_KEY_SYS3,       //!< �ý��� ��ư 3 (vol up ��)
		INPUT_KEY_SYS4,       //!< �ý��� ��ư 4 (vol down ��)
		INPUT_KEY_END,        //!< Ű ������ ������ ���� ������ �� (��ȿ���� ���� ��)
		INPUT_KEY_MAX = INPUT_KEY_END - INPUT_KEY_BEGIN
	};

	struct TMultiTouchInfo
	{
		int x, y;

		TMultiTouchInfo()
			: x(-1), y(-1)
		{
		}
	};

	enum { MAX_MULTI_TOUCH = 10 };

	typedef TMultiTouchInfo TMultiTouchInfoArray[MAX_MULTI_TOUCH];
	/*!
	 * \ingroup gfx2d
	 */
	class CInputDevice
	{
		friend class singleton<CInputDevice>;
		friend class auto_deletor<CInputDevice>;

	public:
		//! �Է� ����̽� ���¸� ���� ���·� ������
		void UpdateInputState();
		//! ������ Ű�� ���� ������ �ִ� �������� �Ǻ�
		bool IsKeyHeldDown(TInputKey key) const;
		//! ������ Ű�� ���� ���� �ִ� �������� �Ǻ�
		bool WasKeyPressed(TInputKey key) const;
		//!
		bool GetMultiTouchInfo(TMultiTouchInfoArray& multi_touch_info_array) const;

	private:
		CInputDevice();
		~CInputDevice();
	};

} // namespace avej_lite


namespace avej_lite
{

//! Application�� call-back �Լ� ���Ǹ� ���� structure. Event-driven ����� �����ϰ� �Ѵ�.
/*!
 * \ingroup AVEJ library application
*/
struct AppCallback
{
	//! Application�� ������ �� �Ҹ���.
	bool (*OnCreate)(void);
	//! Application�� ����� �� �Ҹ���.
	bool (*OnDestory)(void);
	//! ������ ���� ���� ������ ���� ��� �Ҹ���.
	bool (*OnProcess)(void);
};

//! Application�� ���� interface
/*!
 * \ingroup AVEJ library application
*/
class IAvejApp
{
public:
	virtual          ~IAvejApp(void) {};
	virtual bool      Process(void) = 0;
	static  void      ProcessMessages(void);
	static  IAvejApp* GetInstance(const AppCallback& callBack);
    static  int       Release(void);
};

class IAvejAppTarget: IAvejApp
{
public:
	IAvejAppTarget(void);
	~IAvejAppTarget(void);

	void  Create(void);
	void  Destory(void);
	bool  Process(void);
};

}

#endif // #ifndef __AVEJ_APP_H
