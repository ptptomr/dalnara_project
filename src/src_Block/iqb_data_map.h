
#ifndef __IQB_DATA_MAP_H__
#define __IQB_DATA_MAP_H__

#include "iqb_base_type.h"

struct TMapData
{
	int tile;
	int wall;
};

class CTileMap
{
private:
	int m_xMapSize;
	int m_yMapSize;
	int m_xMapPitch;
	int m_xOffset;
	int m_yOffset;

	TMapData* m_pMapData;

public:
	CTileMap(const char* mapData[], int xOffset = 0, int yOffset = 0)
		: m_xMapSize(0), m_yMapSize(0), m_xMapPitch(0), m_pMapData(0), m_xOffset(xOffset), m_yOffset(yOffset)
	{
		Init(mapData);
	}

	~CTileMap(void)
	{
		delete[] m_pMapData;
	}

	void Init(const char* mapData[])
	{
		int i, j;
		TMapData* pMap;

		m_xMapSize = strlen(mapData[0]);
		m_yMapSize = 0;
		while (mapData[m_yMapSize])
			++m_yMapSize;
		m_xMapPitch = m_xMapSize;

		m_pMapData = new TMapData[m_xMapPitch * m_yMapSize];

		for (j = 0; j < m_yMapSize; j++)
		{
			pMap = m_pMapData;
			pMap += j * m_xMapPitch;
			for (i = 0; i < m_xMapSize; i++)
			{
				int data = mapData[m_yMapSize-1-j][i] - '0';

				if (data < 1)
				{
					pMap->tile = data;
					pMap->wall = 0;
				}
				else
				{
					pMap->tile = 0;
					pMap->wall = data;
				}

				++pMap;
			}
		}
	}

	TMapData* m_GetMapData(int& x, int& y) const
	{
		x -= m_xOffset;
		y -= m_yOffset;

		if ((x < 0) || (x >= m_xMapSize) || (y < 0) || (y >= m_yMapSize))
			return NULL;

		if (m_pMapData == NULL)
			return NULL;

		TMapData* pMap = m_pMapData;
		pMap += y * m_xMapPitch + x;

		return pMap;
	}


	bool IsWallMap(int x, int y) const
	{
		TMapData* pMap = m_GetMapData(x, y);

		if (pMap)
			return (pMap->wall > 0);
		else
			return false;
	}

	bool IsMoveableMap(int x, int y) const
	{
		TMapData* pMap = m_GetMapData(x, y);

		if (pMap)
			return (pMap->wall == 0);
		else
			return true;
	}

	bool IsMoveableMapRect(float fx, float fy, float radius) const
	{
		int xTest, yTest;

		xTest = int(fx       +10000.f)-10000;
		yTest = int(fy       +10000.f)-10000;
		if (!IsMoveableMap(xTest, yTest))
			return false;

		xTest = int(fx-radius+10000.f)-10000;
		yTest = int(fy       +10000.f)-10000;
		if (!IsMoveableMap(xTest, yTest))
			return false;

		xTest = int(fx+radius+10000.f)-10000;
		yTest = int(fy       +10000.f)-10000;
		if (!IsMoveableMap(xTest, yTest))
			return false;

		xTest = int(fx       +10000.f)-10000;
		yTest = int(fy-radius+10000.f)-10000;
		if (!IsMoveableMap(xTest, yTest))
			return false;

		xTest = int(fx       +10000.f)-10000;
		yTest = int(fy+radius+10000.f)-10000;
		if (!IsMoveableMap(xTest, yTest))
			return false;

		xTest = int(fx-radius+10000.f)-10000;
		yTest = int(fy-radius+10000.f)-10000;
		if (!IsMoveableMap(xTest, yTest))
			return false;

		xTest = int(fx+radius+10000.f)-10000;
		yTest = int(fy-radius+10000.f)-10000;
		if (!IsMoveableMap(xTest, yTest))
			return false;

		xTest = int(fx-radius+10000.f)-10000;
		yTest = int(fy+radius+10000.f)-10000;
		if (!IsMoveableMap(xTest, yTest))
			return false;

		xTest = int(fx+radius+10000.f)-10000;
		yTest = int(fy+radius+10000.f)-10000;
		if (!IsMoveableMap(xTest, yTest))
			return false;

		return true;
	}

	int  GetTile(int x, int y) const
	{
		TMapData* pMap = m_GetMapData(x, y);

		if (pMap)
			return pMap->tile;
		else
			return 0;
	}

	int  GetWall(int x, int y) const
	{
		TMapData* pMap = m_GetMapData(x, y);

		if (pMap)
			return pMap->wall;
		else
			return 0;
	}

	int  XMapSize(void) const
	{
		return m_xMapSize;
	}

	int  YMapSize(void) const
	{
		return m_yMapSize;
	}

};

#endif // #ifndef __IQB_DATA_MAP_H__
