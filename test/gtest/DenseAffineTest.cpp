﻿
#include <stdio.h>
#include <iostream>

#include "gtest/gtest.h"
#include "bb/DenseAffine.h"


TEST(DenseAffineTest, testAffine)
{
	auto affine = bb::DenseAffine<>::Create(3);
    
    affine->SetInputShape({2});

    // forward

    bb::FrameBuffer x_buf(BB_TYPE_FP32, 1, 2);

	x_buf.SetFP32(0, 0, 1);
	x_buf.SetFP32(0, 1, 2);
	EXPECT_EQ(1, x_buf.GetFP32(0, 0));
	EXPECT_EQ(2, x_buf.GetFP32(0, 1));

    {
        auto W = affine->lock_W();
        auto b = affine->lock_b();
	    W(0, 0) = 1;
	    W(0, 1) = 2;
	    W(1, 0) = 10;
	    W(1, 1) = 20;
	    W(2, 0) = 100;
	    W(2, 1) = 200;
	    b(0) = 1000;
	    b(1) = 2000;
	    b(2) = 3000;
    }

    auto y_buf = affine->Forward(x_buf);

	EXPECT_EQ(1 *   1 + 2 *   2 + 1000, y_buf.GetFP32(0, 0));
	EXPECT_EQ(1 *  10 + 2 *  20 + 2000, y_buf.GetFP32(0, 1));
	EXPECT_EQ(1 * 100 + 2 * 200 + 3000, y_buf.GetFP32(0, 2));

    
    // backward
    
    bb::FrameBuffer dy_buf(BB_TYPE_FP32, 1, 2);

    dy_buf.SetFP32(0, 0, 998);
	dy_buf.SetFP32(0, 1, 2042);
	dy_buf.SetFP32(0, 2, 3491);

	auto dx_buf = affine->Backward(dy_buf);

	EXPECT_EQ(370518, dx_buf.GetFP32(0, 0));
	EXPECT_EQ(741036, dx_buf.GetFP32(0, 1));

    {
	    auto dW = affine->lock_dW_const();

	    EXPECT_EQ(998,  dW(0, 0));
	    EXPECT_EQ(2042, dW(1, 0));
	    EXPECT_EQ(3491, dW(2, 0));
	    EXPECT_EQ(1996, dW(0, 1));
	    EXPECT_EQ(4084, dW(1, 1));
	    EXPECT_EQ(6982, dW(2, 1));
    }
}

#if 0
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


#endif

