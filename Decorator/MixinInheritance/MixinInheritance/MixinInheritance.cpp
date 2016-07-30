// MixinInheritance.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

struct Shape
{
	virtual string str() const = 0;
	virtual ~Shape() { }
};

struct Circle : Shape
{
	float radius;
	Circle() { }
	explicit Circle(const float radius)
		: radius(radius)
	{
	}

	string str() const override
	{
		ostringstream oss;
		oss << "A circle of radius : " << radius;
		return oss.str();
	}

	~Circle() override { }
};

struct ColouredShape : Shape
{
	Shape& shape;
	string color;

	ColouredShape(Shape& shape, const string& color)
		: shape(shape),
		color(color)
	{
	}

	string str() const override
	{
		ostringstream oss;
		oss << shape.str() << " has the color " << color;
		return oss.str();
	}

	~ColouredShape() override { }
};

struct TransparentShape : Shape
{
	Shape& shape;
	uint8_t transparency;

	TransparentShape(Shape& shape, uint8_t transparency)
		: shape(shape),
		transparency(transparency)
	{
	}

	string str() const override
	{
		ostringstream oss;
		oss << shape.str() << " has "
			<< static_cast<float>(transparency) / 255.f * 100.f
			<< " % tranpanrency";
		return oss.str();
	}
	~TransparentShape() override { }
};

template<typename T> struct ColouredShape2 : T
{
	static_assert(is_base_of<Shape, T>::value, "Template must be shape");

	string color;

	template<typename... Args>
	explicit ColouredShape2(const string& color, Args... args)
		: T::T(args...), color(color)
	{
	}

	string str() const override
	{
		ostringstream oss;
		oss << T::str() << " has the color " << color;
		return oss.str();
	}
};

template<typename T> struct TransparentShape2 : T
{
	static_assert(is_base_of<Shape, T>::value, "Template must be shape");

	uint8_t transparency;

	template<typename... Args>
	explicit TransparentShape2(uint8_t transparency, Args... args)
		: T::T(args...), transparency(transparency)
	{
	}

	string str() const override
	{
		ostringstream oss;
		oss << T::str() << " has "
			<< static_cast<float>(transparency) / 255.f * 100.f
			<< " % transparency";
		return oss.str();
	}
};

int main()
{
	ColouredShape2<Circle> red_circle{ "red" };
	red_circle.radius = 5;
	cout << red_circle.str() << endl;

	TransparentShape2<ColouredShape2<Circle>> red_half_transparent_circle{ 127, "red", 100.f };
	cout << red_half_transparent_circle.str() << endl;

	_getch();
    return 0;
}
