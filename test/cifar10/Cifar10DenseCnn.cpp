// --------------------------------------------------------------------------
//  BinaryBrain  -- binary network evaluation platform
//   MNIST sample
//
//                                Copyright (C) 2018-2019 by Ryuji Fuchikami
// --------------------------------------------------------------------------


#include <iostream>
#include <fstream>
#include <numeric>
#include <random>
#include <chrono>

#include "bb/RealToBinary.h"
#include "bb/BinaryToReal.h"
#include "bb/DenseAffine.h"
#include "bb/LoweringConvolution.h"
#include "bb/BatchNormalization.h"
#include "bb/ReLU.h"
#include "bb/Sigmoid.h"
#include "bb/MaxPooling.h"
#include "bb/LossSoftmaxCrossEntropy.h"
#include "bb/MetricsCategoricalAccuracy.h"
#include "bb/OptimizerAdam.h"
#include "bb/OptimizerSgd.h"
#include "bb/LoadCifar10.h"
#include "bb/ShuffleSet.h"
#include "bb/Utility.h"
#include "bb/Sequential.h"
#include "bb/Runner.h"
#include "bb/ExportVerilog.h"


// MNIST CNN with LUT networks
void Cifar10DenseCnn(int epoch_size, int mini_batch_size, int max_run_size, bool binary_mode, bool file_read)
{
    std::string net_name = "Cifar10SimpleLutCnn";
    int const   frame_mux_size = 7;

  // load cifar-10 data
#ifdef _DEBUG
	auto td = bb::LoadCifar10<>::Load(1);
    std::cout << "!!! debug mode !!!" << std::endl;
#else
    auto td = bb::LoadCifar10<>::Load();
#endif

    // create network
    auto net = bb::Sequential::Create();
    net->Add(bb::LoweringConvolution<>::Create(bb::DenseAffine<>::Create(32), 3, 3));
    net->Add(bb::ReLU<>::Create());
    net->Add(bb::LoweringConvolution<>::Create(bb::DenseAffine<>::Create(32), 3, 3));
    net->Add(bb::ReLU<>::Create());
    net->Add(bb::MaxPooling<>::Create(2, 2));
    net->Add(bb::LoweringConvolution<>::Create(bb::DenseAffine<>::Create(64), 3, 3));
    net->Add(bb::ReLU<>::Create());
    net->Add(bb::LoweringConvolution<>::Create(bb::DenseAffine<>::Create(64), 3, 3));
    net->Add(bb::ReLU<>::Create());
    net->Add(bb::MaxPooling<>::Create(2, 2));
    net->Add(bb::DenseAffine<>::Create(512));
    net->Add(bb::ReLU<>::Create());
    net->Add(bb::DenseAffine<>::Create(td.t_shape));
    net->SetInputShape(td.x_shape);

    if ( binary_mode ) {
        net->SendCommand("binary true");
        std::cout << "binary mode" << std::endl;
    }

    // print model information
    net->PrintInfo();

    // run fitting
    bb::Runner<float>::create_t runner_create;
    runner_create.name        = net_name;
    runner_create.net         = net;
    runner_create.lossFunc    = bb::LossSoftmaxCrossEntropy<>::Create();
    runner_create.metricsFunc = bb::MetricsCategoricalAccuracy<>::Create();
    runner_create.optimizer   = bb::OptimizerAdam<>::Create();
    runner_create.max_run_size       = max_run_size;    // ���ۂ�1��̎��s�T�C�Y
    runner_create.file_read          = file_read;       // �O�̌v�Z���ʂ�����Γǂݍ���ōĊJ���邩
    runner_create.file_write         = true;            // �v�Z���ʂ��t�@�C���ɕۑ����邩
    runner_create.print_progress     = true;            // �r�����ʂ�\��
    runner_create.initial_evaluation = file_read;       // �t�@�C����ǂ񂾏ꍇ�͍ŏ��ɕ]�����Ă���
    auto runner = bb::Runner<float>::Create(runner_create);
    runner->Fitting(td, epoch_size, mini_batch_size);
}


// end of file
