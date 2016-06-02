#include <string>
#include <iostream>
#include <stdio.h>

class ShapeAbstractClass
{
public:
	virtual ~ShapeAbstractClass()
	{

	}
public:
	void SetWidth(int width)
	{
		_width = width;
	}
	void SetHeight(int height)
	{
		_height = height;
	}
	virtual int getArea() = 0;

public:
	int _width;
	int _height;
};

class ShapeInterfaceClass
{
public:
	virtual ~ShapeInterfaceClass()
	{

	}
public:
	virtual void SetWidth(int width) = 0;
	virtual void SetHeight(int height) = 0;
	virtual int getArea() = 0;

public:
	int _width;
	int _height;
};


class Rectangle : public ShapeAbstractClass
{
public:
	Rectangle(std::string name)
		: _name (name)
	{
	}
	virtual ~Rectangle()
	{

	}

	virtual int getArea()
	{
		return _width * _height;
	}
public:
	std::string _name;
};



class Triangle : public ShapeAbstractClass
{
public:
	Triangle(std::string name)
		: _name (name)
	{

	}
	virtual ~Triangle()
	{

	}

	virtual int getArea()
	{
		return (_width * _height) / 2;
	}

public:
	std::string _name;
};
