#include <iostream>
using namespace std;
class Shape//named
{
public:
    Shape() { cout << "IShape::Ctor" << endl; }
    ~Shape() { cout << "IShape::Dtor" << endl; }
    virtual void DisplayShapeInfo() = 0;
    void SetShape(int& shapeinfo) { _ShapeInfo = shapeinfo; }
    //int SetShape(int shapeinfo) { _ShapeInfo = shapeinfo; }
    int GetShape() { return _ShapeInfo; }
private:
    int _ShapeInfo = 0;//0 - circle, 1 - rect, 2 - triangle
};

class ShapeRect : public Shape
{
public:
    ShapeRect() { cout << "ShapeRect::Ctor" << endl; }
    ~ShapeRect() { cout << "ShapeRect::Dtor" << endl; }
    void DisplayShapeInfo() override { cout << "This is Rectangle" << endl; }
};

int main()
{
    {
        ShapeRect sRect;//instance
        Shape* shape = nullptr;//object-pointer = nullptr
        shape = dynamic_cast<Shape*>(&sRect);
        int shapeInfo = 100;
        shape->SetShape(shapeInfo);
        shapeInfo = shape->GetShape();
        shape->DisplayShapeInfo();
    }
 
    return 1;
}
