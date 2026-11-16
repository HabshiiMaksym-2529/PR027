#pragma once

#include "Point.h"

#include <array>
#include <iosfwd>
#include <string>

#ifdef GEOMETRY_PACKED
#pragma pack(push, 1)
#endif

class Triangle
{
private:
    Point p1;
    Point p2;
    Point p3;

public:
    Triangle();
    Triangle(const Point& p1, const Point& p2, const Point& p3);
    Triangle(const Triangle& other);

    Point GetVertex(int number) const;
    void SetVertex(int number, const Point& value);

    double get_a() const;
    double get_b() const;
    double get_c() const;

    double get_A() const;
    double get_B() const;
    double get_C() const;

    double Area() const;
    double Perimeter() const;
    std::array<double, 3> Heights() const;
    std::string Type() const;

    Triangle& operator=(const Triangle& other);
    Triangle& operator++();
    Triangle operator++(int);
    Triangle& operator--();
    Triangle operator--(int);

    explicit operator std::string() const;

    friend std::istream& operator>>(std::istream& input, Triangle& triangle);
    friend std::ostream& operator<<(std::ostream& output,
        const Triangle& triangle);
};

#ifdef GEOMETRY_PACKED
#pragma pack(pop)
#endif