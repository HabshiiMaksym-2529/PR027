#pragma once

#include <iosfwd>
#include <string>
#include <utility>

#ifdef GEOMETRY_PACKED
#pragma pack(push, 1)
#endif

class Point
{
private:
    double x{};
    double y{};

public:
    Point();
    Point(double x, double y);
    Point(const Point& other);

    double GetX() const;
    double GetY() const;
    void SetX(double value);
    void SetY(double value);

    void MoveX(double offset);
    void MoveY(double offset);

    double DistanceToOrigin() const;
    double DistanceTo(const Point& other) const;

    // Returns the radius and the angle in degrees.
    std::pair<double, double> ToPolar() const;

    Point& operator=(const Point& other);
    Point& operator++();
    Point operator++(int);
    Point& operator--();
    Point operator--(int);

    explicit operator std::string() const;

    friend bool operator==(const Point& left, const Point& right);
    friend bool operator!=(const Point& left, const Point& right);

    friend std::istream& operator>>(std::istream& input, Point& point);
    friend std::ostream& operator<<(std::ostream& output, const Point& point);
};

#ifdef GEOMETRY_PACKED
#pragma pack(pop)
#endif