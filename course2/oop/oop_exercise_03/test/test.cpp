//
// Created by Sin Denis on 2019-09-16.
//

#include <gtest/gtest.h>
#include "../src/point.h"
#include "../src/triangle.h"
#include "../src/square.h"
#include "../src/rectnagle.h"

TEST(ClassPointTest, AllMethodsTest) {

	Point a{12.32, 32.32};
	Point b{-23.43023, 23.43};
	Point c{};
	Point d{0, 0};

	EXPECT_DOUBLE_EQ(a.X(), 12.32);
	EXPECT_DOUBLE_EQ(a.Y(), 32.32);
	EXPECT_DOUBLE_EQ(b.X(), -23.43023);
	EXPECT_DOUBLE_EQ(b.Y(), 23.43);
	EXPECT_DOUBLE_EQ(c.X(), 0);
	EXPECT_DOUBLE_EQ(c.Y(), 0);
	EXPECT_DOUBLE_EQ(d.X(), 0);
	EXPECT_DOUBLE_EQ(d.Y(), 0);

}

TEST(ClassTriangleTest, AreaTest) {

	Triangle a{};
	Triangle b{Point(34.43, 9.34), Point(32, 43), Point(20, 43)};
	Triangle c{Point(0, 9.34), Point(-32, -43), Point(20, -90.23)};

	EXPECT_DOUBLE_EQ(b.get_area(), 201.96);
	EXPECT_DOUBLE_EQ(c.get_area(), 2116.52);
	EXPECT_DOUBLE_EQ(a.get_area(), 0);

}

TEST(ClassTriangleTest, CenterTest) {

	Triangle b{Point(34.43, 9.34), Point(32, 43), Point(20, 43)};
	Triangle c{Point(0, 9.34), Point(-32, -43), Point(20, -90.23)};

	EXPECT_DOUBLE_EQ(b.get_center().X(), 28.81);
	EXPECT_DOUBLE_EQ(b.get_center().Y(), 31.78);
	EXPECT_DOUBLE_EQ(c.get_center().X(), -4);
	EXPECT_DOUBLE_EQ(c.get_center().Y(), -41.29666666666667);

}

TEST(ClassTriangleTest, BadTriangle) {
	EXPECT_ANY_THROW(Triangle(Point(-1, 4), Point(5, 4), Point(-3242.234234, 4)));
}

TEST(ClassSquareTest, AreaTest) {

	Square a{Point(0, 10), Point(10, 0)};
	Square b{Point(-50.5, 100.5), Point(50, 0)};

	EXPECT_DOUBLE_EQ(a.get_area(), 100);
	EXPECT_DOUBLE_EQ(b.get_area(), 10100.25);
}

TEST(ClassSquareTest, CenterTest) {

	Square a{Point(0, 10), Point(10, 0)};
	Square b{Point(-50.5, 100.5), Point(50, 0)};

	EXPECT_DOUBLE_EQ(a.get_center().X(), 5);
	EXPECT_DOUBLE_EQ(a.get_center().Y(), 5);
	EXPECT_DOUBLE_EQ(b.get_center().X(), -0.25);
	EXPECT_DOUBLE_EQ(b.get_center().Y(), 50.25);

}

TEST(ClassRectangleTest, AreaTest) {

	Rectangle a{Point(-2342.243, 100.23234), Point(8242.24234, -0.2342)};
	Rectangle b{Point(0.232, 98275.82345), Point(3872.234234, -832.2348885043)};

	EXPECT_DOUBLE_EQ(a.get_area(), 1063386.6197905235);
	EXPECT_DOUBLE_EQ(b.get_area(), 383746623.294091);

}

TEST(ClassRectangleTest, CenterTest) {

	Rectangle a{Point(-2342.243, 100.23234), Point(8242.24234, -0.2342)};
	Rectangle b{Point(0.232, 98275.82345), Point(3872.234234, -832.2348885043)};

	EXPECT_DOUBLE_EQ(a.get_center().X(), 2949.99967);
	EXPECT_DOUBLE_EQ(a.get_center().Y(), 49.999069999999996);
	EXPECT_DOUBLE_EQ(b.get_center().X(), 1936.233117);
	EXPECT_DOUBLE_EQ(b.get_center().Y(), 48721.79428074785);

}

class FigureTest : public testing::Test {
protected:
	void SetUp() override {
		shapes.push_back(dynamic_cast<Figure*>(t1));
		shapes.push_back(dynamic_cast<Figure*>(t2));
		shapes.push_back(dynamic_cast<Figure*>(s1));
		shapes.push_back(dynamic_cast<Figure*>(s2));
		shapes.push_back(dynamic_cast<Figure*>(r1));
		shapes.push_back(dynamic_cast<Figure*>(r2));
	}

	void TearDown() override {
		for (int i = 0; i < shapes.size(); ++i) {
			delete shapes[i];
		}
	}

	std::vector<Figure*> shapes;

	Triangle* t1 = new Triangle(Point(34.43, 9.34), Point(32, 43), Point(20, 43));
	Triangle* t2 = new Triangle(Point(0, 9.34), Point(-32, -43), Point(20, -90.23));
	Square* s1 = new Square(Point(0, 10), Point(10, 0));
	Square* s2 = new Square(Point(-50.5, 100.5), Point(50, 0));
	Rectangle* r1 = new Rectangle(Point(-2342.243, 100.23234), Point(8242.24234, -0.2342));
	Rectangle* r2 = new Rectangle(Point(0.232, 98275.82345), Point(3872.234234, -832.2348885043));
};

TEST_F(FigureTest, VirtualMethodsTest) {

	EXPECT_DOUBLE_EQ(shapes[0]->get_center().X(), 28.81);
	EXPECT_DOUBLE_EQ(shapes[0]->get_center().Y(), 31.78);
	EXPECT_DOUBLE_EQ(shapes[1]->get_center().X(), -4);
	EXPECT_DOUBLE_EQ(shapes[1]->get_center().Y(), -41.29666666666667);
	EXPECT_DOUBLE_EQ(shapes[2]->get_center().X(), 5);
	EXPECT_DOUBLE_EQ(shapes[2]->get_center().Y(), 5);
	EXPECT_DOUBLE_EQ(shapes[3]->get_center().X(), -0.25);
	EXPECT_DOUBLE_EQ(shapes[3]->get_center().Y(), 50.25);
	EXPECT_DOUBLE_EQ(shapes[4]->get_center().X(), 2949.99967);
	EXPECT_DOUBLE_EQ(shapes[4]->get_center().Y(), 49.999069999999996);
	EXPECT_DOUBLE_EQ(shapes[5]->get_center().X(), 1936.233117);
	EXPECT_DOUBLE_EQ(shapes[5]->get_center().Y(), 48721.79428074785);

	EXPECT_DOUBLE_EQ(shapes[0]->get_area(), 201.96);
	EXPECT_DOUBLE_EQ(shapes[1]->get_area(), 2116.52);
	EXPECT_DOUBLE_EQ(shapes[2]->get_area(), 100);
	EXPECT_DOUBLE_EQ(shapes[3]->get_area(), 10100.25);
	EXPECT_DOUBLE_EQ(shapes[4]->get_area(), 1063386.6197905235);
	EXPECT_DOUBLE_EQ(shapes[5]->get_area(), 383746623.294091);

}
