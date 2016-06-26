#pragma once

#include "Geometry.h"

// AdapterExample.cpp : Defines the entry point for the application.
//

struct LineToPointAdapter
{
	using Points = std::vector<Point>;

	explicit LineToPointAdapter(Line& line)
	{
		auto left = min(line.start.x, line.end.x);
		auto right = max(line.start.x, line.end.x);
		auto top = min(line.start.y, line.end.y);
		auto bottom = max(line.start.y, line.end.y);

		auto dx = right - left;
		auto dy = line.end.y - line.start.y;

		if (dx == 0)
		{
			//vertical
			for (int y = top; y <= bottom; ++y)
			{
				points.emplace_back(Point{ left, y });
			}
		}
		else if (dy == 0)
		{
			for (int x = left; x <= right; ++x)
			{
				points.emplace_back(Point{ x, top });
			}
		}
	}

	Points::const_iterator begin() const
	{
		return points.cbegin();
	}

	Points::const_iterator end() const
	{
		return points.cend();
	}

private:
	Points points;
};

struct LineToPointCachingAdapter
{
	using Points = std::vector<Point>;

	explicit LineToPointCachingAdapter(Line& line)
	{
		boost::hash<Line> hash;
		line_hash = hash(line);
		if (cache.find(line_hash) != cache.end())
			return;

		Points points;

		auto left = min(line.start.x, line.end.x);
		auto right = max(line.start.x, line.end.x);
		auto top = min(line.start.y, line.end.y);
		auto bottom = max(line.start.y, line.end.y);

		auto dx = right - left;
		auto dy = line.end.y - line.start.y;

		if (dx == 0)
		{
			//vertical
			for (int y = top; y <= bottom; ++y)
			{
				points.emplace_back(Point{ left, y });
			}
		}
		else if (dy == 0)
		{
			for (int x = left; x <= right; ++x)
			{
				points.emplace_back(Point{ x, top });
			}
		}

		cache[line_hash] = points;
	}

	Points::const_iterator begin() const
	{
		return cache[line_hash].cbegin();
	}

	Points::const_iterator end() const
	{
		return cache[line_hash].cend();
	}
private:
	size_t line_hash;
	static std::map<size_t, Points> cache;
};
std::map<size_t, std::vector<Point>> LineToPointCachingAdapter::cache;

std::vector<std::shared_ptr<VectorObject>> vectorObjects{
	std::make_shared<VectorRectangle>(10, 10, 100,100),
	std::make_shared<VectorRectangle>(30, 30, 60, 60)
};

inline void DrawPoint(HDC _hdc, std::vector<Point>::const_iterator begin, std::vector<Point>::const_iterator end)
{
	for (auto i = begin; i != end; ++i)
		SetPixel(_hdc, i->x, i->y, 0);
}

inline void DrawVectorObjects(HDC _hdc)
{
	for (auto& o : vectorObjects)
	{
		for (auto& l : *o)
		{
			LineToPointCachingAdapter lpo{ l };
			DrawPoint(_hdc, lpo.begin(), lpo.end());
		}
	}
}

inline void AdapterExample(HDC _hdc)
{
	DrawVectorObjects(_hdc);
}
