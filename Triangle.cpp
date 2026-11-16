#include "Triangle.h"

#include <algorithm>
#include <cmath>
#include <istream>
#include <ostream>
#include <sstream>
#include <stdexcept>

namespace
{
    struct Vector
    {
        long double x;
        long double y;
    };

    Vector Between(const Point& from, const Point& to)
    {
        return {
            static_cast<long double>(to.GetX()) - from.GetX(),
            static_cast<long double>(to.GetY()) - from.GetY()
        };
    }

    long double Cross(const Vector& left, const Vector& right)
    {
        return left.x * right.y - left.y * right.x;
    }

    Vector UnitDirection(const Point& from, const Point& to)
    {
        const Vector vector = Between(from, to);
        const long double length = std::hypot(vector.x, vector.y);

        if (length == 0.0L || !std::isfinite(length))
        {
            throw std::invalid_argument(
                "Vertices must be distinct and side lengths must be finite."
            );
        }

        return { vector.x / length, vector.y / length };
    }

    double AngleAt(const Point& vertex, const Point& first, const Point& second)
    {
        const Vector left = UnitDirection(vertex, first);
        const Vector right = UnitDirection(vertex, second);

        const long double dot = left.x * right.x + left.y * right.y;
        const long double radians = std::atan2(std::abs(Cross(left, right)), dot);

        return static_cast<double>(radians * (180.0L / std::acos(-1.0L)));
    }
}

Triangle::Triangle()
    : Triangle(Point(0.0, 0.0), Point(1.0, 0.0), Point(0.0, 1.0))
{
}

Triangle::Triangle(const Point& p1, const Point& p2, const Point& p3)
    : p1(p1), p2(p2), p3(p3)
{
    const Vector first = UnitDirection(p1, p2);
    const Vector second = UnitDirection(p1, p3);

    if (Cross(first, second) == 0.0L)
    {
        throw std::invalid_argument("The vertices must form a triangle.");
    }

    if (!std::isfinite(get_a()) ||
        !std::isfinite(get_b()) ||
        !std::isfinite(get_c()))
    {
        throw std::invalid_argument("Side lengths must be finite.");
    }
}

Triangle::Triangle(const Triangle& other) = default;

Point Triangle::GetVertex(int number) const
{
    switch (number)
    {
    case 1:
        return p1;
    case 2:
        return p2;
    case 3:
        return p3;
    default:
        throw std::out_of_range("Choose vertex 1, 2 or 3.");
    }
}

void Triangle::SetVertex(int number, const Point& value)
{
    switch (number)
    {
    case 1:
        *this = Triangle(value, p2, p3);
        break;
    case 2:
        *this = Triangle(p1, value, p3);
        break;
    case 3:
        *this = Triangle(p1, p2, value);
        break;
    default:
        throw std::out_of_range("Choose vertex 1, 2 or 3.");
    }
}

double Triangle::get_a() const
{
    return p2.DistanceTo(p3);
}

double Triangle::get_b() const
{
    return p1.DistanceTo(p3);
}

double Triangle::get_c() const
{
    return p1.DistanceTo(p2);
}

double Triangle::get_A() const
{
    return AngleAt(p1, p2, p3);
}

double Triangle::get_B() const
{
    return AngleAt(p2, p1, p3);
}

double Triangle::get_C() const
{
    return AngleAt(p3, p1, p2);
}

double Triangle::Area() const
{
    const Vector first = Between(p1, p2);
    const Vector second = Between(p1, p3);
    const Vector halfFirst{ first.x * 0.5L, first.y * 0.5L };

    return static_cast<double>(std::abs(Cross(halfFirst, second)));
}

double Triangle::Perimeter() const
{
    return get_a() + get_b() + get_c();
}

std::array<double, 3> Triangle::Heights() const
{
    const long double sinA = std::abs(
        Cross(UnitDirection(p1, p2), UnitDirection(p1, p3))
    );

    const long double sinB = std::abs(
        Cross(UnitDirection(p2, p1), UnitDirection(p2, p3))
    );

    return {
        static_cast<double>(get_c() * sinB),
        static_cast<double>(get_c() * sinA),
        static_cast<double>(get_b() * sinA)
    };
}

std::string Triangle::Type() const
{
    std::array<double, 3> sides{ get_a(), get_b(), get_c() };
    std::sort(sides.begin(), sides.end());

    const double shortest = sides[0] / sides[2];
    const double middle = sides[1] / sides[2];
    constexpr double tolerance = 1e-9;

    const bool equilateral = std::abs(shortest - 1.0) <= tolerance;
    const bool isosceles = std::abs(shortest - middle) <= tolerance ||
        std::abs(middle - 1.0) <= tolerance;
    const bool right = std::abs(shortest * shortest +
        middle * middle - 1.0) <= tolerance;

    if (equilateral)
    {
        return "Equilateral";
    }
    if (isosceles && right)
    {
        return "Isosceles and right-angled";
    }
    if (isosceles)
    {
        return "Isosceles";
    }
    if (right)
    {
        return "Right-angled";
    }

    return "Scalene";
}

Triangle& Triangle::operator=(const Triangle& other) = default;

Triangle& Triangle::operator++()
{
    Point first = p1;
    Point second = p2;
    Point third = p3;

    ++first;
    ++second;
    ++third;

    *this = Triangle(first, second, third);
    return *this;
}

Triangle Triangle::operator++(int)
{
    Triangle previous(*this);
    ++(*this);
    return previous;
}

Triangle& Triangle::operator--()
{
    Point first = p1;
    Point second = p2;
    Point third = p3;

    --first;
    --second;
    --third;

    *this = Triangle(first, second, third);
    return *this;
}

Triangle Triangle::operator--(int)
{
    Triangle previous(*this);
    --(*this);
    return previous;
}

Triangle::operator std::string() const
{
    std::ostringstream output;
    output << *this;
    return output.str();
}

std::istream& operator>>(std::istream& input, Triangle& triangle)
{
    Point first;
    Point second;
    Point third;

    if (input >> first >> second >> third)
    {
        try
        {
            triangle = Triangle(first, second, third);
        }
        catch (const std::invalid_argument&)
        {
            input.setstate(std::ios::failbit);
        }
    }

    return input;
}

std::ostream& operator<<(std::ostream& output, const Triangle& triangle)
{
    return output << "P1 = " << triangle.p1 << ", "
        << "P2 = " << triangle.p2 << ", "
        << "P3 = " << triangle.p3;
}