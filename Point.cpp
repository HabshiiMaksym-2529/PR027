#include "Point.h"

#include <cmath>
#include <istream>
#include <ostream>
#include <sstream>
#include <stdexcept>

namespace
{
    void CheckCoordinate(double value)
    {
        if (!std::isfinite(value))
        {
            throw std::invalid_argument("Coordinates must be finite.");
        }
    }
}

Point::Point() = default;

Point::Point(double x, double y)
{
    SetX(x);
    SetY(y);
}

Point::Point(const Point& other) = default;

double Point::GetX() const
{
    return x;
}

double Point::GetY() const
{
    return y;
}

void Point::SetX(double value)
{
    CheckCoordinate(value);
    x = value;
}

void Point::SetY(double value)
{
    CheckCoordinate(value);
    y = value;
}

void Point::MoveX(double offset)
{
    SetX(x + offset);
}

void Point::MoveY(double offset)
{
    SetY(y + offset);
}

double Point::DistanceToOrigin() const
{
    return std::hypot(x, y);
}

double Point::DistanceTo(const Point& other) const
{
    return std::hypot(x - other.x, y - other.y);
}

std::pair<double, double> Point::ToPolar() const
{
    if (x == 0.0 && y == 0.0)
    {
        return { 0.0, 0.0 };
    }

    const double pi = std::acos(-1.0);
    const double angle = std::atan2(y, x) * (180.0 / pi);

    return { DistanceToOrigin(), angle };
}

Point& Point::operator=(const Point& other) = default;

Point& Point::operator++()
{
    *this = Point(x + 1.0, y + 1.0);
    return *this;
}

Point Point::operator++(int)
{
    Point previous(*this);
    ++(*this);
    return previous;
}

Point& Point::operator--()
{
    *this = Point(x - 1.0, y - 1.0);
    return *this;
}

Point Point::operator--(int)
{
    Point previous(*this);
    --(*this);
    return previous;
}

Point::operator std::string() const
{
    std::ostringstream output;
    output << *this;
    return output.str();
}

bool operator==(const Point& left, const Point& right)
{
    return left.x == right.x && left.y == right.y;
}

bool operator!=(const Point& left, const Point& right)
{
    return !(left == right);
}

std::istream& operator>>(std::istream& input, Point& point)
{
    double x{};
    double y{};

    if (input >> x >> y)
    {
        try
        {
            point = Point(x, y);
        }
        catch (const std::invalid_argument&)
        {
            input.setstate(std::ios::failbit);
        }
    }

    return input;
}

std::ostream& operator<<(std::ostream& output, const Point& point)
{
    return output << '(' << point.x << ", " << point.y << ')';
}