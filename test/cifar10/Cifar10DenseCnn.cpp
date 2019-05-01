// --------------------------------------------------------------------------
//  BinaryBrain  -- binary network evaluation platform
//   CIFAR-10 sample
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
#include "bb/UniformDistributionGenerator.h"


// Dense CNN
void Cifar10DenseCnn(int epoch_size, int mini_batch_size, int max_run_size, int frame_mux_size, int lut_frame_mux_size, bool binary_mode, bool file_read)
{
    std::string net_name = "Cifar10DenseCnn";

  // load cifar-10 data
#ifdef _DEBUG
    auto td = bb::LoadCifar10<>::Load(1);
    std::cout << "!!! debug mode !!!" << std::endl;
#else
    auto td = bb::LoadCifar10<>::Load();
#endif

    // create network
    auto net = bb::Sequential::Create();
    if ( binary_mode ) {
//      net->Add(bb::RealToBinary<>::Create(frame_mux_size));
        net->Add(bb::RealToBinary<>::Create(frame_mux_size, bb::UniformDistributionGenerator<float>::Create(0.0f, 1.0f, 1)));
    }
    net->Add(bb::LoweringConvolution<>::Create(bb::DenseAffine<>::Create(32), 3, 3));
    net->Add(bb::BatchNormalization<>::Create());
    net->Add(bb::ReLU<>::Create());
    net->Add(bb::LoweringConvolution<>::Create(bb::DenseAffine<>::Create(32), 3, 3));
    net->Add(bb::BatchNormalization<>::Create());
    net->Add(bb::ReLU<>::Create());
    net->Add(bb::MaxPooling<>::Create(2, 2));
    net->Add(bb::LoweringConvolution<>::Create(bb::DenseAffine<>::Create(64), 3, 3));
    net->Add(bb::BatchNormalization<>::Create());
    net->Add(bb::ReLU<>::Create());
    net->Add(bb::LoweringConvolution<>::Create(bb::DenseAffine<>::Create(64), 3, 3));
    net->Add(bb::BatchNormalization<>::Create());
    net->Add(bb::ReLU<>::Create());
    net->Add(bb::MaxPooling<>::Create(2, 2));
    net->Add(bb::DenseAffine<>::Create(512));
    net->Add(bb::BatchNormalization<>::Create());
    net->Add(bb::ReLU<>::Create());
    net->Add(bb::DenseAffine<>::Create(td.t_shape));
    if ( binary_mode ) {
        net->Add(bb::BatchNormalization<>::Create());
        net->Add(bb::Binarize<>::Create());
        net->Add(bb::BinaryToReal<>::Create(td.t_shape, frame_mux_size));
    }
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
