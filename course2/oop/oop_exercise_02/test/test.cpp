//
// Created by Sin Denis on 2019-09-15.
//

#include <gtest/gtest.h>
#include "../src/complex.h"

TEST(ComplexClassTests, SumTest) {

	Complex a{12, 43.3};
	Complex b{};
	Complex c{0, 0};
	Complex d{32.434, 232.43};
	Complex e{-323.653, -892748.234};
	Complex res;

	res = a + c;
	EXPECT_FLOAT_EQ(res.Re(), 12);
	EXPECT_FLOAT_EQ(res.Im(), 43.3);
	res = a + 32.4_i;
	EXPECT_FLOAT_EQ(res.Re(), 12);
	EXPECT_FLOAT_EQ(res.Im(), 75.7);
	res = a + d;
	EXPECT_FLOAT_EQ(res.Re(), 44.434);
	EXPECT_FLOAT_EQ(res.Im(), 275.73);
	res = d + e;
	EXPECT_FLOAT_EQ(res.Re(), -291.2190);
	EXPECT_FLOAT_EQ(res.Im(), -892515.804);

}

TEST(ComplexClassTests, SubTest) {

	Complex a{14.3, 90.12323};
	Complex b{89.2332223, -98.23414};
	Complex c{-123123.123312, -9000.000000234239003};
	Complex e{0, 0};
	Complex res;

	res = a - b;
	EXPECT_FLOAT_EQ(res.Re(), -74.9332223);
	EXPECT_FLOAT_EQ(res.Im(), 188.35737);

	res = b - c;
	EXPECT_FLOAT_EQ(res.Re(), 123212.3565343);
	EXPECT_FLOAT_EQ(res.Im(), 8901.765860234238);

	res = c - a;
	EXPECT_FLOAT_EQ(res.Re(), -123137.423312);
	EXPECT_FLOAT_EQ(res.Im(), -9090.123230234238);

	res = e - b;
	EXPECT_FLOAT_EQ(res.Re(), -89.2332223);
	EXPECT_FLOAT_EQ(res.Im(), 98.23414);

}

TEST(ComplexClassTests, ProductTest) {

	Complex a{90900014.3, 987.12323};
	Complex b{345.2323, -9908.414};
	Complex c{-1123.123312, -9093500.000000234239003};
	Complex e{0, 0};
	Complex res;

	res = a * b;
	EXPECT_FLOAT_EQ(res.Re(), 31391401832.4537);
	EXPECT_FLOAT_EQ(res.Im(), -900674633503.497);

	res = b * c;
	EXPECT_FLOAT_EQ(res.Re(), -90102550447.4466);
	EXPECT_FLOAT_EQ(res.Im(), -3128241549.30174);

	res = c * a;
	EXPECT_FLOAT_EQ(res.Re(), -93115520029.4581);
	EXPECT_FLOAT_EQ(res.Im(), -826599281145733.0);

	res = e * b;
	EXPECT_FLOAT_EQ(res.Re(), 0);
	EXPECT_FLOAT_EQ(res.Im(), 0);

}

TEST(ComplexClassTests, DivideTest) {

	Complex a{-348942.3, 987.12323};
	Complex b{-344345.2323, -9908.414};
	Complex c{-6023.1203312, -9980.009003};
	Complex e{0, 0};
	Complex res;

	res = a / b;
	EXPECT_FLOAT_EQ(res.Re(), 1.01242941164702);
	EXPECT_FLOAT_EQ(res.Im(), -0.0319989706787328);

	res = b / c;
	EXPECT_FLOAT_EQ(res.Re(), 15.9916241307185);
	EXPECT_FLOAT_EQ(res.Im(), -24.8522577279043);

	res = c / a;
	EXPECT_FLOAT_EQ(res.Re(), 0.0171800320522364);
	EXPECT_FLOAT_EQ(res.Im(), 0.0286493434772759);

	res = e / b;
	EXPECT_FLOAT_EQ(res.Re(), 0);
	EXPECT_FLOAT_EQ(res.Im(), 0);

	EXPECT_ANY_THROW(a / e);

	EXPECT_ANY_THROW(b / 0.0_i);

}

TEST(ComplexClassTests, AssertTest) {

	Complex a{-348942.3, 987.12323};
	Complex b{-344345.2323, -9908.414};
	Complex q{-344345.2323, -9908.414};
	Complex c{-6023.1203312, -9980.009003};
	Complex t{-6023.1203312, -9980.009003};
	Complex e{0, 0};

	EXPECT_EQ(a + e, a);
	EXPECT_EQ(b, q);
	EXPECT_EQ(c - e, c);
	EXPECT_EQ(c, t);
	EXPECT_EQ(e, 0.0_i);

}

TEST(ComplexClassTests, ConjTest) {

	Complex a1{-348942.3, 987.12323};
	Complex a2{-348942.3, -987.12323};
	Complex b1{-344345.2323, -9908.414};
	Complex b2{-344345.2323, 9908.414};
	Complex c1{-6023.1203312, -9980.009003};
	Complex c2{-6023.1203312, 9980.009003};
	Complex e{0, 0};

	EXPECT_EQ(a1.conj(), a2);
	EXPECT_EQ(b1.conj(), b2);
	EXPECT_EQ(c1.conj(), c2);
	EXPECT_EQ(e.conj(), e);

}
