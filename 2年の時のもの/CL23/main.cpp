//-----------------------------------------------
// main.cpp
// 制作日：2024/09/20
// 制作者：安田晴人
//-----------------------------------------------

#include <iostream>

struct Vector2
{
    float x = 0.0f, y = 0.0f;

    Vector2() {};
    Vector2(float x, float y) : x(x), y(y) {}
    ~Vector2() {};

    Vector2 operator+(const Vector2& r) const
    {
        return { x + r.x, y + r.y };
    }

    Vector2 operator-(const Vector2& r) const
    {
        return { x - r.x, y - r.y };
    }

    Vector2 operator*(float s) const
    {
        return { x * s, y * s };
    }
};

Vector2 operator* (float s, Vector2 a)
{
    return { s * a.x, s * a.y };
}

int main()
{
    Vector2 a{ 1.1f, 2.2f }, b{ 3.3f, 4.4f }, c;

    //c = a + b;
    //c = a - b;
    //c = a * 4.0f;
    c = 4.0f * a;

    std::cout << c.x << "," << c.y << std::endl;

    return 0;
}
