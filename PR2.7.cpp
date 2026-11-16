#include "Triangle.h"

#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace
{
    std::string ReadLine(const std::string& prompt)
    {
        std::cout << prompt;

        std::string line;
        if (!std::getline(std::cin, line))
        {
            throw std::runtime_error("Input ended.");
        }

        return line;
    }

    template <typename T>
    bool ParseValue(const std::string& line, T& value)
    {
        std::istringstream input(line);
        char extra{};

        return (input >> value) && !(input >> extra);
    }

    template <typename T>
    T ReadValue(const std::string& prompt)
    {
        while (true)
        {
            T value{};

            if (ParseValue(ReadLine(prompt), value))
            {
                return value;
            }

            std::cout << "Enter one valid number and press Enter.\n";
        }
    }

    std::string ReadCoordinate(const std::string& prompt)
    {
        while (true)
        {
            const std::string line = ReadLine(prompt);
            double value{};

            if (ParseValue(line, value))
            {
                return line;
            }

            std::cout << "Enter one valid coordinate and press Enter.\n";
        }
    }

    std::string ReadCoordinates(const std::string& name)
    {
        const std::string x = ReadCoordinate(name + " X: ");
        const std::string y = ReadCoordinate(name + " Y: ");
        return x + ' ' + y;
    }

    Point ReadPoint(const std::string& name)
    {
        std::istringstream input(ReadCoordinates(name));
        Point point;

        if (!(input >> point))
        {
            throw std::invalid_argument("Invalid point.");
        }

        return point;
    }

    void EnterTriangle(Triangle& triangle)
    {
        const std::string first = ReadCoordinates("P1");
        const std::string second = ReadCoordinates("P2");
        const std::string third = ReadCoordinates("P3");

        std::istringstream input(first + ' ' + second + ' ' + third);

        if (!(input >> triangle))
        {
            throw std::invalid_argument(
                "Invalid triangle. The previous vertices were kept."
            );
        }

        std::cout << "Saved: " << triangle << '\n';
    }

    void ShowTriangle(const Triangle& triangle)
    {
        const auto heights = triangle.Heights();

        std::cout << '\n' << triangle << '\n'
            << "Sides a, b, c: " << triangle.get_a() << ' '
            << triangle.get_b() << ' ' << triangle.get_c() << '\n'
            << "Angles A, B, C (degrees): " << triangle.get_A() << ' '
            << triangle.get_B() << ' ' << triangle.get_C() << '\n'
            << "Heights to a, b, c: " << heights[0] << ' '
            << heights[1] << ' ' << heights[2] << '\n'
            << "Area: " << triangle.Area() << '\n'
            << "Perimeter: " << triangle.Perimeter() << '\n'
            << "Type: " << triangle.Type() << '\n';
    }

    template <typename T>
    bool ChangeByOne(T& object)
    {
        std::cout << "\n1. Prefix increment (++x)\n"
            << "2. Postfix increment (x++)\n"
            << "3. Prefix decrement (--x)\n"
            << "4. Postfix decrement (x--)\n"
            << "0. Back\n";

        switch (ReadValue<int>("Choose an operation: "))
        {
        case 0:
            return false;
        case 1:
            std::cout << "Returned value: " << ++object << '\n';
            break;
        case 2:
            std::cout << "Returned value: " << object++ << '\n';
            break;
        case 3:
            std::cout << "Returned value: " << --object << '\n';
            break;
        case 4:
            std::cout << "Returned value: " << object-- << '\n';
            break;
        default:
            throw std::invalid_argument("Choose a number from 0 to 4.");
        }

        return true;
    }

    void VertexOperations(Triangle& triangle)
    {
        const int number = ReadValue<int>("Choose a vertex (1-3): ");
        Point vertex = triangle.GetVertex(number);

        std::cout << "Selected vertex: " << vertex << '\n'
            << "\n1. Replace coordinates\n"
            << "2. Move along X\n"
            << "3. Move along Y\n"
            << "4. Show distance to origin and polar coordinates\n"
            << "5. Compare with another point and show distance\n"
            << "6. Increment or decrement\n"
            << "0. Back\n";

        switch (ReadValue<int>("Choose an operation: "))
        {
        case 0:
            return;

        case 1:
            vertex = ReadPoint("New point");
            break;

        case 2:
            vertex.MoveX(ReadValue<double>("X offset: "));
            break;

        case 3:
            vertex.MoveY(ReadValue<double>("Y offset: "));
            break;

        case 4:
        {
            const auto polar = vertex.ToPolar();

            std::cout << "Distance to origin: " << vertex.DistanceToOrigin()
                << "\nPolar radius: " << polar.first
                << "\nPolar angle (degrees): " << polar.second << '\n';
            return;
        }

        case 5:
        {
            const Point other = ReadPoint("Other point");

            std::cout << std::boolalpha
                << "Equal: " << (vertex == other) << '\n'
                << "Not equal: " << (vertex != other) << '\n'
                << "Distance between points: "
                << vertex.DistanceTo(other) << '\n';
            return;
        }

        case 6:
            if (!ChangeByOne(vertex))
            {
                return;
            }
            break;

        default:
            throw std::invalid_argument("Choose a number from 0 to 6.");
        }

        triangle.SetVertex(number, vertex);
        std::cout << "Saved: " << triangle << '\n';
    }

    void DemonstrateClasses(const Triangle& triangle)
    {
        Point point(3.0, 4.0);
        Point copy(point);
        copy.SetX(5.0);
        copy.SetY(6.0);

        const Point points[] = { Point(), point, copy };
        const Triangle example(
            Point(0.0, 0.0), Point(3.0, 0.0), Point(0.0, 4.0)
        );
        const Triangle triangles[] = { Triangle(), example, triangle };

        std::cout << "\nArray of points:\n";
        for (const Point& item : points)
        {
            std::cout << static_cast<std::string>(item) << '\n';
        }

        std::cout << "\nArray of triangles:\n";
        for (const Triangle& item : triangles)
        {
            std::cout << static_cast<std::string>(item) << '\n';
        }

#ifdef GEOMETRY_PACKED
        std::cout << "\nSizes with #pragma pack(1):\n";
#else
        std::cout << "\nSizes with default alignment:\n";
#endif

        std::cout << "sizeof(Point): " << sizeof(Point) << " bytes\n"
            << "sizeof(Triangle): " << sizeof(Triangle) << " bytes\n";
    }
}

int main()
{
    Triangle triangle;

    while (true)
    {
        std::cout << "\n1. Enter triangle\n"
            << "2. Show triangle calculations\n"
            << "3. Vertex operations\n"
            << "4. Increment or decrement the triangle\n"
            << "5. Class demonstration\n"
            << "0. Exit\n";

        try
        {
            switch (ReadValue<int>("Choose an option: "))
            {
            case 0:
                return 0;

            case 1:
                EnterTriangle(triangle);
                break;

            case 2:
                ShowTriangle(triangle);
                break;

            case 3:
                VertexOperations(triangle);
                break;

            case 4:
                if (ChangeByOne(triangle))
                {
                    std::cout << "Current triangle: " << triangle << '\n';
                }
                break;

            case 5:
                DemonstrateClasses(triangle);
                break;

            default:
                std::cout << "Choose a number from 0 to 5.\n";
                break;
            }
        }
        catch (const std::exception& error)
        {
            if (!std::cin)
            {
                return 0;
            }

            std::cout << "Error: " << error.what() << '\n';
        }
    }
}