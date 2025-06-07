#include <iostream>
#include <memory>
#include <string>
using namespace std;

class Shape
{
    public:
    virtual void draw() = 0;
    virtual ~Shape() {};
};

class Circle : public Shape
{
    public:
    void draw()
    {
        cout << "draw a circle" << endl;
    }
};

class Rectangle : public Shape
{
    public:
    void draw()
    {
        cout << "draw a rectangle" << endl;
    }
};

class ShapeFactory
{
public:
    static unique_ptr<Shape> createShape(const string &type)
    {
        if (type.compare("circle") == 0)
            return make_unique<Circle>();
        else if (type.compare("rectangle") == 0)
            return make_unique<Rectangle>();
        else
            throw invalid_argument("Unknown shape type");
    }
};

int main()
{
    auto shape = ShapeFactory::createShape("rectangle");
    shape->draw();
    return 0;
}