
#include "iqb_actor.h"
#include <assert.h>

CSmPlayer::CSmPlayer()
{
}

CSmPlayer::~CSmPlayer()
{
}

template <class T>
inline int sgn(T data)
{
	return (data == 0) ? 0 : ((data > 0) ? 1 : -1);
}

void CSmPlayer::Move(float dx, float dy, bool turnFace)
{
	attribute.pos.x += dx;
	attribute.pos.y += dy;

	if (turnFace)
	{
		if (dx == 0 && dy == 0)
			return;

		int index = (2-(sgn(dy)+1)) * 3 + (sgn(dx)+1);

		const int FACE_DATA[9] =
		{
			0, 0, 1,
			3, 0, 1,
			3, 2, 2
		};

		assert(index >= 0 && index < 9);

		attribute.face = FACE_DATA[index];
	}

	++attribute.face_inc_count;
	if (attribute.face_inc_count > 10)
	{
		attribute.face_inc_count = 0;
		attribute.face_inc = (attribute.face_inc+1) % 2;
	}
}

void CSmPlayer::Warp(float x, float y)
{
	attribute.pos.x = x;
	attribute.pos.y = y;
}

unsigned long CSmPlayer::Process(long refTime, ISmActor* pSender)
{
	return 0;
}

CSmPlayer* CreateCharacter(int type, float x, float y)
{
	CSmPlayer* pPlayer = new CSmPlayer;
	pPlayer->Warp(x, y);
	
	return pPlayer;
}
