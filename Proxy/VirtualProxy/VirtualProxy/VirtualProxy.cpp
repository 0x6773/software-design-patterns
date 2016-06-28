// VirtualProxy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

struct Image
{
	virtual ~Image() = default;
	virtual void draw() = 0;
};

struct Bitmap : Image
{
	Bitmap(const string& filename)
	{
		cout << "Loading a file from " << filename << endl;
	}

	~Bitmap() override = default;
	void draw() override
	{
		cout << "Drawing Image" << endl;
	}
};

struct LazyBitmap : Image
{
	explicit LazyBitmap(const string& filename)
		: filename(filename),
		bmp(nullptr)
	{
	}
	
	void draw() override
	{
		if (!bmp)
			bmp = new Bitmap{ filename };
		bmp->draw();
	}
	~LazyBitmap() override
	{
		if(bmp)
			delete bmp;
	}
private:
	string filename;
	Bitmap *bmp;
};

void draw_image(Image& img)
{
	cout << "About to draw Image " << endl;
	img.draw();
	cout << "Done drawing the image " << endl;
}

void virtual_proxy()
{
	LazyBitmap bmp{ "pockemon.png" };
	draw_image(bmp);
	draw_image(bmp);
}

int main()
{
	virtual_proxy();

	_getch();
    return 0;
}

