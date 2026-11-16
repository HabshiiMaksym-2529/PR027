#include "pch.h"
#include "CppUnitTest.h"
#include "E:\Документи\навчання\2 курс ООП\PR2.x\PR2.7\Point.h"
#include "E:\Документи\навчання\2 курс ООП\PR2.x\PR2.7\Point.cpp"
#include "E:\Документи\навчання\2 курс ООП\PR2.x\PR2.7\Triangle.h"
#include "E:\Документи\навчання\2 курс ООП\PR2.x\PR2.7\Triangle.cpp"
#include "E:\Документи\навчання\2 курс ООП\PR2.x\PR2.7\PR2.7.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TriangleTests
{
    TEST_CLASS(TriangleTests)
    {
    public:
        TEST_METHOD(Area_UsesVertexCoordinates)
        {
            const Triangle triangle(
                Point(1.0, 2.0),
                Point(4.0, 6.0),
                Point(5.0, 2.0)
            );

            const double actual = triangle.Area();

            Assert::AreEqual(8.0, actual, 1e-12);
        }
    };
}