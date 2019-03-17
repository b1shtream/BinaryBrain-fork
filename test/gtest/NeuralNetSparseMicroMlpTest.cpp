﻿#include <stdio.h>
#include <iostream>
#include "gtest/gtest.h"
#include "bb/NeuralNetSparseMicroMlp.h"
#include "bb/NeuralNetSparseMicroMlpDiscrete.h"
#include "bb/NeuralNetOptimizerSgd.h"


inline void testSetupLayerBuffer(bb::NeuralNetLayer<>& net)
{
	net.SetInputSignalBuffer (net.CreateInputSignalBuffer());
	net.SetInputErrorBuffer (net.CreateInputErrorBuffer());
	net.SetOutputSignalBuffer(net.CreateOutputSignalBuffer());
	net.SetOutputErrorBuffer(net.CreateOutputErrorBuffer());
}


inline void PrintBuffer(bb::NeuralNetBuffer<>& buf)
{
	int frame_size = (int)buf.GetFrameSize();
	int node_size  = (int)buf.GetNodeSize();
	frame_size = 8;// std::min(frame_size, 2);
	node_size  = std::min(node_size, 32);
	for (int frame = 0; frame < frame_size; ++frame) {
		std::cout << "[";
		for (int node = 0; node < node_size; ++node) {
			std::cout << buf.GetReal(frame, node) << ", ";
		}
		std::cout << "]\n";
	}
}

inline void PrintLayerBuffer(bb::NeuralNetLayer<>& layer, std::string name)
{
	auto in_sig_buf = layer.GetInputSignalBuffer();
	auto out_sig_buf = layer.GetOutputSignalBuffer();
	auto in_err_buf = layer.GetInputErrorBuffer();
	auto out_err_buf = layer.GetOutputErrorBuffer();

	std::cout << "(" << name << ")" << std::endl;
	std::cout << "in_sig" << std::endl;
	PrintBuffer(in_sig_buf);

	std::cout << "out_sig" << std::endl;
	PrintBuffer(out_sig_buf);

	std::cout << "in_err" << std::endl;
	PrintBuffer(in_err_buf);

	std::cout << "out_err" << std::endl;
	PrintBuffer(out_err_buf);
}


TEST(NeuralNetSparseMicroMlpTest, testLut)
{
	const int input_node_size = 6;
	const int output_node_size = 3;
	const int frame_size = 3;
	bb::NeuralNetSparseMicroMlp<6, 6> smm(input_node_size, output_node_size);
	bb::NeuralNetSparseMicroMlpDiscrete<6, 6> smmd(input_node_size, output_node_size);

	smm.SetOptimizer(&bb::NeuralNetOptimizerSgd<>(0.01f));

	smm.SetBatchSize(frame_size);
	testSetupLayerBuffer(smm);

	auto in_sig_buf = smm.GetInputSignalBuffer();
	in_sig_buf.SetReal(0, 0, 0.1f);
	in_sig_buf.SetReal(0, 1, 0.2f);
	in_sig_buf.SetReal(0, 2, 0.3f);
	in_sig_buf.SetReal(0, 3, 0.4f);
	in_sig_buf.SetReal(0, 4, 0.5f);
	in_sig_buf.SetReal(0, 5, 0.6f);

	in_sig_buf.SetReal(1, 0, 0.9f);
	in_sig_buf.SetReal(1, 1, 0.8f);
	in_sig_buf.SetReal(1, 2, 0.7f);
	in_sig_buf.SetReal(1, 3, 0.6f);
	in_sig_buf.SetReal(1, 4, 0.5f);
	in_sig_buf.SetReal(1, 5, 0.3f);

	in_sig_buf.SetReal(2, 0, 0.1f);
	in_sig_buf.SetReal(2, 1, 0.9f);
	in_sig_buf.SetReal(2, 2, 0.3f);
	in_sig_buf.SetReal(2, 3, 0.2f);
	in_sig_buf.SetReal(2, 4, 0.7f);
	in_sig_buf.SetReal(2, 5, 0.4f);

//	PrintLayerBuffer(lut.m_batch_norm, "m_batch_norm");

	smm.Forward();

	auto out_sig_buf = smm.GetOutputSignalBuffer();
#if 0
	std::cout << "out(0, 0) : " << out_sig_buf.GetReal(0, 0) << std::endl;
	std::cout << "out(0, 1) : " << out_sig_buf.GetReal(0, 1) << std::endl;
	std::cout << "out(0, 2) : " << out_sig_buf.GetReal(0, 2) << std::endl;
#endif

	auto out_err_buf = smm.GetOutputErrorBuffer();
	out_err_buf.SetReal(0, 0, 0.5f);
	out_err_buf.SetReal(0, 1, -0.7f);
	out_err_buf.SetReal(0, 2, 0.9f);
	out_err_buf.SetReal(1, 0, -0.3f);
	out_err_buf.SetReal(1, 1, 0.1f);
	out_err_buf.SetReal(1, 2, 0.4f);
	out_err_buf.SetReal(2, 0, 0.2f);
	out_err_buf.SetReal(2, 1, -0.3f);
	out_err_buf.SetReal(2, 2, -0.4f);


	smm.Backward();

	auto in_err_buf = smm.GetInputErrorBuffer();
#if 0
	std::cout << "in_err(0, 0) : " << out_sig_buf.GetReal(0, 0) << std::endl;
	std::cout << "in_err(0, 1) : " << out_sig_buf.GetReal(0, 1) << std::endl;
	std::cout << "in_err(0, 2) : " << out_sig_buf.GetReal(0, 2) << std::endl;
	std::cout << "in_err(0, 3) : " << out_sig_buf.GetReal(0, 3) << std::endl;
	std::cout << "in_err(0, 4) : " << out_sig_buf.GetReal(0, 4) << std::endl;
	std::cout << "in_err(0, 5) : " << out_sig_buf.GetReal(0, 5) << std::endl;
#endif

	smm.Update();

#if 0
	PrintLayerBuffer(lut.m_lut_pre, "m_lut_pre");
//	PrintLayerBuffer(lut.m_act_pre, "m_act_pre");
//	PrintLayerBuffer(lut.m_lut_post, "m_lut_post");
//	PrintLayerBuffer(lut.m_batch_norm, "m_batch_norm");
//	PrintLayerBuffer(lut.m_act_post, "m_act_post");
#endif

}

