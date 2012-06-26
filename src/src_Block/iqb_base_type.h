
#ifndef __IQB_BASE_TYPE_H__
#define __IQB_BASE_TYPE_H__

#define single float

struct TSmPoint
{
	long x;
	long y;
};

struct TSm_Point
{
	float x;
	float y;
	TSm_Point(): x(0.0f), y(0.0f) {};
	TSm_Point(float _x, float _y): x(_x), y(_y) {};
};

struct TSmRect
{
	long  x, y, w, h;
	long  dx, dy;
};

class ISmActor
{
public:
	virtual ~ISmActor() {};
	virtual unsigned long Process(long refTime = 0, ISmActor* pSender = 0) = 0;
};

#endif // #ifndef __IQB_BASE_TYPE_H__
