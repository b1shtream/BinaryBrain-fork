﻿#include <stdio.h>
#include <iostream>
#include "gtest/gtest.h"
#include "bb/NeuralNetDenseAffine.h"
#include "bb/NeuralNetOptimizerSgd.h"


inline void testSetupLayerBuffer(bb::NeuralNetLayer<>& net)
{
	net.SetInputSignalBuffer (net.CreateInputSignalBuffer());
	net.SetInputErrorBuffer (net.CreateInputErrorBuffer());
	net.SetOutputSignalBuffer(net.CreateOutputSignalBuffer());
	net.SetOutputErrorBuffer(net.CreateOutputErrorBuffer());
}


TEST(NeuralNetDenseAffineTest, testAffine)
{
	bb::NeuralNetDenseAffine<> affine(2, 3);
	testSetupLayerBuffer(affine);

	auto in_val = affine.GetInputSignalBuffer();
	auto out_val = affine.GetOutputSignalBuffer();

	affine.SetOptimizer(&bb::NeuralNetOptimizerSgd<>(0.01f));

	in_val.SetReal(0, 0, 1);
	in_val.SetReal(0, 1, 2);
	EXPECT_EQ(1, in_val.GetReal(0, 0));
	EXPECT_EQ(2, in_val.GetReal(0, 1));

	affine.W(0, 0) = 1;
	affine.W(0, 1) = 2;
	affine.W(1, 0) = 10;
	affine.W(1, 1) = 20;
	affine.W(2, 0) = 100;
	affine.W(2, 1) = 200;
	affine.b(0) = 1000;
	affine.b(1) = 2000;
	affine.b(2) = 3000;
	affine.Forward();
	EXPECT_EQ(1 * 1 + 2 * 2 + 1000, out_val.GetReal(0, 0));
	EXPECT_EQ(1 * 10 + 2 * 20 + 2000, out_val.GetReal(0, 1));
	EXPECT_EQ(1 * 100 + 2 * 200 + 3000, out_val.GetReal(0, 2));

	auto in_err = affine.GetInputErrorBuffer();
	auto out_err = affine.GetOutputErrorBuffer();
	out_err.SetReal(0, 0, 998);
	out_err.SetReal(0, 1, 2042);
	out_err.SetReal(0, 2, 3491);

	affine.Backward();
	EXPECT_EQ(370518, in_err.GetReal(0, 0));
	EXPECT_EQ(741036, in_err.GetReal(0, 1));

	EXPECT_EQ(998,  affine.dW(0, 0));
	EXPECT_EQ(2042, affine.dW(1, 0));
	EXPECT_EQ(3491, affine.dW(2, 0));
	EXPECT_EQ(1996, affine.dW(0, 1));
	EXPECT_EQ(4084, affine.dW(1, 1));
	EXPECT_EQ(6982, affine.dW(2, 1));
	
	affine.Update();
}


TEST(NeuralNetDenseAffineTest, testAffineBatch)
{
	bb::NeuralNetDenseAffine<> affine(2, 3);
	testSetupLayerBuffer(affine);

	affine.SetBatchSize(2);

	auto in_val = affine.GetInputSignalBuffer();
	auto out_val = affine.GetOutputSignalBuffer();
	in_val.SetReal(0, 0, 1);
	in_val.SetReal(0, 1, 2);
	in_val.SetReal(1, 0, 3);
	in_val.SetReal(1, 1, 4);


	affine.W(0, 0) = 1;
	affine.W(0, 1) = 2;
	affine.W(1, 0) = 10;
	affine.W(1, 1) = 20;
	affine.W(2, 0) = 100;
	affine.W(2, 1) = 200;
	affine.b(0) = 1000;
	affine.b(1) = 2000;
	affine.b(2) = 3000;
	affine.Forward();
		
	EXPECT_EQ(1 * 1 + 2 * 2 + 1000, out_val.GetReal(0, 0));
	EXPECT_EQ(1 * 10 + 2 * 20 + 2000, out_val.GetReal(0, 1));
	EXPECT_EQ(1 * 100 + 2 * 200 + 3000, out_val.GetReal(0, 2));
	EXPECT_EQ(3 * 1 + 4 * 2 + 1000, out_val.GetReal(1, 0));
	EXPECT_EQ(3 * 10 + 4 * 20 + 2000, out_val.GetReal(1, 1));
	EXPECT_EQ(3 * 100 + 4 * 200 + 3000, out_val.GetReal(1, 2));
	

	auto out_err = affine.GetOutputErrorBuffer();
	auto in_err = affine.GetInputErrorBuffer();
	out_err.SetReal(0, 0, 998);
	out_err.SetReal(1, 0, 1004);
	out_err.SetReal(0, 1, 2042);
	out_err.SetReal(1, 1, 2102);
	out_err.SetReal(0, 2, 3491);
	out_err.SetReal(1, 2, 4091);

	affine.Backward();

	EXPECT_EQ(370518, in_err.GetReal(0, 0));
	EXPECT_EQ(741036, in_err.GetReal(0, 1));
	EXPECT_EQ(431124, in_err.GetReal(1, 0));
	EXPECT_EQ(862248, in_err.GetReal(1, 1));
}


