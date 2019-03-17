﻿#include <stdio.h>
#include <iostream>
#include <fstream>
#include "gtest/gtest.h"

#include "opencv2/opencv.hpp"

#include "bb/NeuralNetGroup.h"
#include "bb/NeuralNetBinaryLutN.h"
#include "bb/NeuralNetBinaryLut6.h"
#include "bb/NeuralNetBinaryFilter.h"


#if BB_NEURALNET_BUFFER_USE_ROI


inline void testSetupLayerBuffer(bb::NeuralNetLayer<>& net)
{
	net.SetInputSignalBuffer (net.CreateInputSignalBuffer());
	net.SetInputErrorBuffer (net.CreateInputErrorBuffer());
	net.SetOutputSignalBuffer(net.CreateOutputSignalBuffer());
	net.SetOutputErrorBuffer(net.CreateOutputErrorBuffer());
}


TEST(NeuralNetBinaryFilterTest, testNeuralNetBinaryFilter)
{
	size_t batch_size = 3;
	size_t input_c_size = 3;
	size_t input_h_size = 16;
	size_t input_w_size = 24;
	size_t output_c_size = 3;
	size_t filter_h_size = 7;
	size_t filter_w_size = 5;
	size_t y_step = 2;
	size_t x_step = 3;
	size_t mux_size = 7;
	size_t output_h_size = ((input_h_size - filter_h_size + 1) + (y_step - 1)) / y_step;
	size_t output_w_size = ((input_w_size - filter_w_size + 1) + (x_step - 1)) / x_step;


	// フィルタ層生成
	size_t filter_input_node_size  = input_c_size * input_h_size * input_w_size;
	size_t filter_layer0_node_size = output_c_size * 6;
	size_t filter_output_node_size = output_c_size;
	bb::NeuralNetGroup<> filter_net;
	bb::NeuralNetBinaryLut6<> filter_lut0(filter_input_node_size, filter_layer0_node_size, 1);
	bb::NeuralNetBinaryLut6<> filter_lut1(filter_layer0_node_size, filter_output_node_size, 1);
	filter_net.AddLayer(&filter_lut0);
	filter_net.AddLayer(&filter_lut1);
	
	// 畳み込み層構成
	bb::NeuralNetBinaryFilter<> net(&filter_net, input_c_size, input_h_size, input_w_size, output_c_size, filter_h_size, filter_w_size, y_step, x_step);
	testSetupLayerBuffer(net);

	net.SetMuxSize(mux_size);
	net.SetBatchSize(batch_size);

	net.Forward();

	EXPECT_EQ(true, true);


	// シリアライズ確認
	std::string fname("testNeuralNetBinaryFilterSerialize.json");
	// save
	{
		std::ofstream ofs(fname);
		cereal::JSONOutputArchive o_archive(ofs);
		//		o_archive(cereal::make_nvp("layer_lut", lut0));
		net.Save(o_archive);
	}

	std::vector<double> loss(batch_size*mux_size);
	net.Feedback(loss);
}


static void image_show(std::string name, bb::NeuralNetBuffer<> buf, size_t f, size_t h, size_t w)
{
	cv::Mat img((int)h, (int)w, CV_8U);
	for (size_t y = 0; y < h; y++) {
		for (size_t x = 0; x < w; x++) {
			img.at<uchar>((int)y, (int)x) = buf.GetBinary(f, y*w + x) ? 255 : 0;
		}
	}
	cv::imshow(name, img);
}



// LUTを入力0をそのままスルーするように設定
void SetLutThrough0(bb::NeuralNetBinaryLut<true>& net, bool inv)
{
	size_t node_size = net.GetOutputNodeSize();
	int lut_table_size = net.GetLutTableSize();

	for (size_t node = 0; node < node_size; ++node) {
		for (int bit = 0; bit < lut_table_size; ++bit) {
			net.SetLutTable(node, bit, (bit & 1) ? !inv : inv);
		}
	}
}



TEST(NeuralNetBinaryFilterTest, testNeuralNetBinaryFilterRoi1)
{
	size_t mux_size = 1;
	size_t batch_size = 1;
	size_t input_c_size = 1;
	size_t input_h_size = 16;
	size_t input_w_size = 16;
	size_t filter_h_size = 5;
	size_t filter_w_size = 5;
	size_t y_step = 1;
	size_t x_step = 1;
	size_t output_c_size = 1;
	size_t output_h_size = ((input_h_size - filter_h_size + 1) + (y_step - 1)) / y_step;
	size_t output_w_size = ((input_w_size - filter_w_size + 1) + (x_step - 1)) / x_step;

	size_t input_node_size = input_c_size * input_h_size * input_w_size;
	size_t output_node_size = output_c_size * output_h_size * output_w_size;
	size_t frame_size = batch_size * mux_size;

	// テストデータ作成
	std::mt19937_64 mt(1);
	std::vector< std::vector<bool> > train_images(frame_size);
	for (size_t frame = 0; frame < frame_size; ++frame) {
		train_images[frame].resize(input_node_size);
		for (size_t y = 0; y < input_h_size; y++) {
			for (size_t x = 0; x < input_w_size; x++) {
				train_images[frame][y*input_w_size + x] = (mt() % 2 == 0);
			}
		}
	}

	// フィルタ層生成
	size_t filter_input_node_size = input_c_size * filter_h_size * filter_w_size;
	size_t filter_layer0_node_size = output_c_size * 6;
	size_t filter_output_node_size = output_c_size;
	bb::NeuralNetGroup<> filter_net;
	bb::NeuralNetBinaryLutN<6, true> filter_lut0(filter_input_node_size, filter_layer0_node_size, 1);
	bb::NeuralNetBinaryLutN<6, true> filter_lut1(filter_layer0_node_size, filter_output_node_size, 1);
	filter_net.AddLayer(&filter_lut0);
	filter_net.AddLayer(&filter_lut1);

	// LUTを入力0をそのままスルー or 反転するように設定
	SetLutThrough0(filter_lut0, false);
	SetLutThrough0(filter_lut1, true);

	filter_lut0.SetLutInput(0, 0, 0);
//	filter_lut0.SetLutInput(1, 0, 0);
	filter_lut1.SetLutInput(0, 0, 0);
//	filter_lut1.SetLutInput(1, 0, 1);

	// 畳み込み層構成
	bb::NeuralNetBinaryFilter<> net(&filter_net, input_c_size, input_h_size, input_w_size, output_c_size, filter_h_size, filter_w_size, y_step, x_step);
	net.SetMuxSize(mux_size);
	net.SetBatchSize(batch_size);

	// バッファ準備
	testSetupLayerBuffer(net);

	// 入力データ
	auto in_val = net.GetInputSignalBuffer();
	auto out_val = net.GetOutputSignalBuffer();
	for (size_t frame = 0; frame < batch_size*mux_size; ++frame) {
		for (size_t node = 0; node < input_node_size; ++node) {
			in_val.SetBinary(frame, node, train_images[frame][node]);
		}
	}

	net.Forward();

	// 出力データ
	for (size_t frame = 0; frame < batch_size*mux_size; ++frame) {
		for (size_t y = 0; y < output_h_size; y++) {
			for (size_t x = 0; x < output_w_size; x++) {
				EXPECT_EQ(!train_images[frame][y*input_w_size + x], out_val.GetBinary(frame, y*output_w_size + x));
			}
		}
	}

//	image_show("in0", in_val, 0, input_h_size, input_w_size);
//	image_show("out0", out_val, 0, output_h_size, output_w_size);
//	cv::waitKey();
}


#if 0
TEST(NeuralNetBinaryFilterTest, testNeuralNetBinaryFilterRoi)
{
	size_t batch_size = 4;
	size_t input_c_size = 1;
	size_t input_h_size = 16;
	size_t input_w_size = 16;
	size_t output_c_size = 2;
	size_t filter_h_size = 5;
	size_t filter_w_size = 5;
	size_t y_step = 1;
	size_t x_step = 1;
	size_t mux_size = 1;
	size_t output_h_size = ((input_h_size - filter_h_size + 1) + (y_step - 1)) / y_step;
	size_t output_w_size = ((input_w_size - filter_w_size + 1) + (x_step - 1)) / x_step;

	size_t input_node_size = input_c_size * input_h_size * input_w_size;
	size_t output_node_size = output_c_size * output_h_size * output_w_size;
	size_t frame_size = batch_size * mux_size;

	// テストデータ作成
	std::mt19937_64 mt(1);
	std::vector< std::vector<bool> > train_images(frame_size);
	for (size_t frame = 0; frame < frame_size; ++frame) {
		train_images[frame].resize(input_node_size);
		for (size_t y = 0; y < input_h_size; y++) {
			for (size_t x = 0; x < input_w_size; x++) {
				train_images[frame][y*input_w_size + x] = (mt() % 2 == 0);
			}
		}
	}
	
	// フィルタ層生成
	size_t filter_input_node_size = input_c_size * input_h_size * input_w_size;
	size_t filter_layer0_node_size = output_c_size*6;
	size_t filter_output_node_size = output_c_size;
	bb::NeuralNetGroup<> filter_net;
	bb::NeuralNetBinaryLutN<6, true> filter_lut0(filter_input_node_size, filter_layer0_node_size, 1);
	bb::NeuralNetBinaryLutN<6, true> filter_lut1(filter_layer0_node_size, filter_output_node_size, 1);
	filter_net.AddLayer(&filter_lut0);
	filter_net.AddLayer(&filter_lut1);

	// LUTを入力0をそのままスルー or 反転するように設定
	SetLutThrough0(filter_lut0, false);
	SetLutThrough0(filter_lut1, true);

	filter_lut0.SetLutInput(0, 0, 0);
	filter_lut0.SetLutInput(1, 0, 0);
	filter_lut1.SetLutInput(0, 0, 0);
	filter_lut1.SetLutInput(1, 0, 1);
	
	// 畳み込み層構成
	bb::NeuralNetBinaryFilter<> net(&filter_net, input_c_size, input_h_size, input_w_size, output_c_size, filter_h_size, filter_w_size, y_step, x_step, mux_size);
	net.SetMuxSize(mux_size);
	net.SetBatchSize(batch_size);

	// バッファ準備
	testSetupLayerBuffer(net);
	
	// 入力データ
	auto in_val = net.GetInputSignalBuffer();
	for (size_t frame = 0; frame < batch_size*mux_size; ++frame) {
		for (size_t node = 0; node < input_node_size; ++node) {
			in_val.SetBinary(frame, node, train_images[frame][node]);
		}
	}

	net.Forward();

	// 出力データ
	auto out_val = net.GetOutputSignalBuffer();
	for (size_t frame = 0; frame < batch_size*mux_size; ++frame) {
		for (size_t node = 0; node < input_node_size; ++node) {
//			EXPECT_EQ(!train_images[frame][node], out_val.GetBinary(frame, node));
		}
	}

	image_show("in0", in_val, 0, input_h_size, input_w_size);
//	image_show("in1", in_val, 1, input_h_size, input_w_size);
//	image_show("in2", in_val, 2, input_h_size, input_w_size);
//	image_show("in3", in_val, 3, input_h_size, input_w_size);
	image_show("out0", out_val, 0, output_h_size, output_w_size);
//	image_show("out1", out_val, 1, output_h_size, output_w_size);
//	image_show("out2", out_val, 2, output_h_size, output_w_size);
//	image_show("out3", out_val, 3, output_h_size, output_w_size);
	cv::waitKey();
}
#endif



#if 0
TEST(NeuralNetBinaryFilterTest, testNeuralNetBinaryFilterFeedBack)
{
	size_t batch_size = 4;
	size_t input_c_size = 1;
	size_t input_h_size = 16;
	size_t input_w_size = 16;
	size_t output_c_size = 1;
	size_t filter_h_size = 5;
	size_t filter_w_size = 5;
	size_t y_step = 1;
	size_t x_step = 1;
	size_t mux_size = 1;
	size_t output_h_size = ((input_h_size - filter_h_size + 1) + (y_step - 1)) / y_step;
	size_t output_w_size = ((input_w_size - filter_w_size + 1) + (x_step - 1)) / x_step;
	
	size_t input_node_size = input_c_size * input_h_size * input_w_size;
	size_t output_node_size = output_c_size * output_h_size * output_w_size;

	// テストデータ作成
	std::mt19937_64 mt(1);
	std::vector< std::vector<bool> > train_images(batch_size);
	train_images[0].resize(input_node_size);
	train_images[1].resize(input_node_size);
	train_images[2].resize(input_node_size);
	train_images[3].resize(input_node_size);
//	train_images[4].resize(input_node_size);
	for (size_t y = 0; y < input_h_size; y++) {
		for (size_t x = 0; x < input_w_size; x++) {
			train_images[0][y*input_w_size + x] = true;
			train_images[1][y*input_w_size + x] = false;
			train_images[2][y*input_w_size + x] = (y % 2 == x % 2);
			train_images[3][y*input_w_size + x] = (y % 2 != x % 2);
//			train_images[4][y*input_w_size + x] = (mt()%2 == 0);
		}
	}
	
	std::vector<bool> train_labels = { false, false, true, true };


	// フィルタ層生成
	size_t filter_input_node_size = input_c_size * input_h_size * input_w_size;
	size_t filter_layer0_node_size = output_c_size * 6;
	size_t filter_output_node_size = output_c_size;
	bb::NeuralNetGroup<> filter_net;
	bb::NeuralNetBinaryLut6<true> filter_lut0(filter_input_node_size, filter_layer0_node_size, mux_size);
	bb::NeuralNetBinaryLut6<true> filter_lut1(filter_layer0_node_size, filter_output_node_size, mux_size);
	filter_net.AddLayer(&filter_lut0);
	filter_net.AddLayer(&filter_lut1);

	// 畳み込み層構成
	bb::NeuralNetBinaryFilter<> net(&filter_net, input_c_size, input_h_size, input_w_size, output_c_size, filter_h_size, filter_w_size, y_step, x_step, mux_size);
	net.SetMuxSize(mux_size);
	net.SetBatchSize(batch_size);
	
	// バッファ準備
	testSetupLayerBuffer(net);

	auto in_val = net.GetInputSignalBuffer();
	for (size_t frame = 0; frame < batch_size*mux_size; ++frame) {
		for (size_t node = 0; node < input_node_size; ++node) {
			in_val.SetBinary(frame, node, train_images[frame][node]);
		}
	}

	// 学習
	double total_loss = 0;
	for (int loop = 0; loop < 1; loop++) {
		// 予測
		net.Forward();

		auto out_val = net.GetOutputSignalBuffer();

#if 1
		image_show("in0", in_val, 0, input_h_size, input_w_size);
		image_show("in1", in_val, 1, input_h_size, input_w_size);
		image_show("in2", in_val, 2, input_h_size, input_w_size);
		image_show("in3", in_val, 3, input_h_size, input_w_size);
//		image_show("in4", in_val, 4, input_h_size, input_w_size);
		image_show("out0", out_val, 0, output_h_size, output_w_size);
		image_show("out1", out_val, 1, output_h_size, output_w_size);
		image_show("out2", out_val, 2, output_h_size, output_w_size);
		image_show("out3", out_val, 3, output_h_size, output_w_size);
//		image_show("out4", out_val, 4, output_h_size, output_w_size);
		cv::waitKey();
#endif

		total_loss = 0;
		for (size_t frame = 0; frame < batch_size*mux_size; ++frame) {
			for (size_t node = 0; node < output_node_size; ++node) {
				total_loss += out_val.GetBinary(frame, node) != train_labels[frame] ? 1 : 0;
			}
		}

		std::vector<double> loss(batch_size*mux_size);
		int n = 0;
		do {
			for (size_t frame = 0; frame < batch_size*mux_size; ++frame) {
				loss[frame] = 0;
				for (size_t node = 0; node < output_node_size; ++node) {
					loss[frame] += out_val.GetBinary(frame , node) != train_labels[frame] ? 1 : 0;
				}
			}
			n++;
		} while (net.Feedback(loss));

		std::cout << "loss : " << n << " : " << total_loss << std::endl;
	}
	
	EXPECT_EQ(true, true);
}
#endif


/*
TEST(NeuralNetBinaryFilterTest, testNeuralNetBinaryFilterFeedBack2)
{
	size_t batch_size = 4;
	size_t input_c_size = 1;
	size_t input_h_size = 16;
	size_t input_w_size = 16;
	size_t output_c_size = 1;
	size_t filter_h_size = 5;
	size_t filter_w_size = 5;
	size_t y_step = 1;
	size_t x_step = 1;
	size_t mux_size = 1;
	size_t output_h_size = ((input_h_size - filter_h_size + 1) + (y_step - 1)) / y_step;
	size_t output_w_size = ((input_w_size - filter_w_size + 1) + (x_step - 1)) / x_step;

	size_t input_node_size = input_c_size * input_h_size * input_w_size;
	size_t output_node_size = output_c_size * output_h_size * output_w_size;

	// テストデータ作成
	std::mt19937_64 mt(1);
	std::vector< std::vector<bool> > train_images(batch_size);
	train_images[0].resize(input_node_size);
	train_images[1].resize(input_node_size);
	train_images[2].resize(input_node_size);
	train_images[3].resize(input_node_size);
	//	train_images[4].resize(input_node_size);
	for (size_t y = 0; y < input_h_size; y++) {
		for (size_t x = 0; x < input_w_size; x++) {
			train_images[0][y*input_w_size + x] = (x < 8);
			train_images[1][y*input_w_size + x] = (y > 8);
			train_images[2][y*input_w_size + x] = (y % 2 == x % 2);
			train_images[3][y*input_w_size + x] = (mt()%2 == 0);
		}
	}

	std::vector<bool> train_labels = { false, false, true, true };


	// フィルタ層生成
	size_t filter_input_node_size = input_c_size * input_h_size * input_w_size;
	size_t filter_layer0_node_size = output_c_size * 6;
	size_t filter_output_node_size = output_c_size;
	bb::NeuralNetGroup<> filter_net;
	bb::NeuralNetBinaryLutN<6, true> filter_lut0(filter_input_node_size, filter_layer0_node_size, 1);
	bb::NeuralNetBinaryLutN<6, true> filter_lut1(filter_layer0_node_size, filter_output_node_size, 2);
	filter_net.AddLayer(&filter_lut0);
	filter_net.AddLayer(&filter_lut1);

	// 畳み込み層構成
	bb::NeuralNetBinaryFilter<> net(&filter_net, input_c_size, input_h_size, input_w_size, output_c_size, filter_h_size, filter_w_size, y_step, x_step, mux_size, batch_size);
	net.SetMuxSize(mux_size);
	net.SetBatchSize(batch_size);

	// バッファ準備
	testSetupLayerBuffer(net);

	auto in_val = net.GetInputSignalBuffer();
//	auto out_val = net.GetOutputSignalBuffer();

	for (size_t frame = 0; frame < batch_size*mux_size; ++frame) {
		for (size_t node = 0; node < input_node_size; ++node) {
			in_val.SetBinary(frame, node, train_images[frame][node]);
		}
	}

	// 学習
	double total_loss = 0;
	for (int loop = 0; loop < 10; loop++) {
		std::cout << "loop :" << loop << std::endl;

		// debug
	//	SetLutThrough0(filter_lut0, false);
	//	SetLutThrough0(filter_lut1, true);
	//	filter_lut0.SetLutInput(0, 0, 0);
	//	filter_lut1.SetLutInput(0, 0, 0);


		// 予測
//		auto out_val1 = net.GetOutputSignalBuffer();
		net.Forward();
		auto out_val = net.GetOutputSignalBuffer();

		std::cout << "Forward()" << loop << std::endl;


#if 1
		image_show("in0", in_val, 0, input_h_size, input_w_size);
		image_show("in1", in_val, 1, input_h_size, input_w_size);
		image_show("in2", in_val, 2, input_h_size, input_w_size);
		image_show("in3", in_val, 3, input_h_size, input_w_size);
		//		image_show("in4", in_val, 4, input_h_size, input_w_size);
		image_show("out0", out_val, 0, output_h_size, output_w_size);
		image_show("out1", out_val, 1, output_h_size, output_w_size);
		image_show("out2", out_val, 2, output_h_size, output_w_size);
		image_show("out3", out_val, 3, output_h_size, output_w_size);
		//		image_show("out4", out_val, 4, output_h_size, output_w_size);
		cv::waitKey();
#endif
		std::cout << "loss()" << loop << std::endl;

		total_loss = 0;
		for (size_t frame = 0; frame < batch_size*mux_size; ++frame) {
			for (size_t node = 0; node < output_node_size; ++node) {
				total_loss += out_val.GetBinary(frame, node) != train_labels[frame] ? 1 : 0;
			}
		}

		std::vector<double> loss(batch_size*mux_size);
		int n = 0;
		do {
			for (size_t frame = 0; frame < batch_size*mux_size; ++frame) {
				loss[frame] = 0;
				for (size_t node = 0; node < output_node_size; ++node) {
					loss[frame] += out_val.GetBinary(frame, node) != train_labels[frame] ? 1 : 0;
				}
			}
			n++;
		} while (net.Feedback(loss));

		std::cout << "loss : " << n << " : " << total_loss << std::endl;
	}

	EXPECT_EQ(true, true);
}

*/


#endif

