// WrappingDecorator.cpp : Defines the entry point for the console application.
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
			<< " % transparency";
		return oss.str();
	}
	~TransparentShape() override { }
};

int main()
{
	Circle circle{ 5 };
	cout << circle.str() << endl;

	ColouredShape red_circle{ circle, "red" };
	cout << red_circle.str() << endl;

	TransparentShape half_transparent_shape{ circle, 128 };
	cout << half_transparent_shape.str() << endl;

	TransparentShape half_tr_red_shape{ red_circle, 128 };
	cout << half_tr_red_shape.str() << endl;

	_getch();
    return 0;
}

