
#ifndef __IQB_ACTOR_H__
#define __IQB_ACTOR_H__

#include "iqb_base_type.h"

class CSmPlayer: public ISmActor
{
private:
	//ISmActor* m_pAction;

public:
	struct TAttribute
	{
		TSm_Point pos;
		int face;
		int face_inc;
		int face_inc_count;
		TAttribute(): face(0), face_inc(0), face_inc_count(0) {};
	} attribute;

	CSmPlayer();
	~CSmPlayer();

	void Move(float dx, float dy, bool turnFace = true);
	void Warp(float x, float y);
	unsigned long Process(long refTime, ISmActor* pSender = 0);
};

CSmPlayer* CreateCharacter(int type, float x, float y);

#endif // #ifndef __IQB_ACTOR_H__
