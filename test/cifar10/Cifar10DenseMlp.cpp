﻿// --------------------------------------------------------------------------
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


// CIFAR-10 MLP with DenseAffine networks
void Cifar10DenseMlp(int epoch_size, int mini_batch_size, int max_run_size, bool binary_mode, bool file_read)
{
    std::string net_name = "Cifar10DenseMlp";

  // load cifar-10 data
#ifdef _DEBUG
    auto td = bb::LoadCifar10<>::Load(1);
    std::cout << "!!! debug mode !!!" << std::endl;
#else
    auto td = bb::LoadCifar10<>::Load();
#endif

    // create network
    auto net = bb::Sequential::Create();
    net->Add(bb::BatchNormalization<>::Create());
    net->Add(bb::DenseAffine<>::Create(1024));
    net->Add(bb::ReLU<>::Create());
    net->Add(bb::BatchNormalization<>::Create());
    net->Add(bb::DenseAffine<>::Create(1024));
    net->Add(bb::ReLU<>::Create());
    net->Add(bb::BatchNormalization<>::Create());
    net->Add(bb::DenseAffine<>::Create(1024));
    net->Add(bb::ReLU<>::Create());
    net->Add(bb::BatchNormalization<>::Create());
    net->Add(bb::DenseAffine<>::Create(1024));
    net->Add(bb::ReLU<>::Create());
    net->Add(bb::BatchNormalization<>::Create());
    net->Add(bb::DenseAffine<>::Create(1024));
    net->Add(bb::ReLU<>::Create());
    net->Add(bb::BatchNormalization<>::Create());
    net->Add(bb::DenseAffine<>::Create(512));
    net->Add(bb::ReLU<>::Create());
    net->Add(bb::BatchNormalization<>::Create());
    net->Add(bb::DenseAffine<>::Create(256));
    net->Add(bb::ReLU<>::Create());
    net->Add(bb::BatchNormalization<>::Create());
    net->Add(bb::DenseAffine<>::Create(td.t_shape));
//  net->Add(bb::ReLU<>::Create());
    net->SetInputShape(td.x_shape);

    if ( binary_mode ) {
        net->SendCommand("binary true");
        std::cout << "binary mode" << std::endl;
    }
    // print model information
    net->PrintInfo();

    // run fitting
    bb::Runner<float>::create_t runner_create;
    runner_create.name               = net_name;
    runner_create.net                = net;
    runner_create.lossFunc           = bb::LossSoftmaxCrossEntropy<>::Create();
    runner_create.metricsFunc        = bb::MetricsCategoricalAccuracy<>::Create();
    runner_create.optimizer          = bb::OptimizerAdam<>::Create();
    runner_create.max_run_size       = max_run_size;    // 実際の1回の実行サイズ
    runner_create.file_read          = file_read;       // 前の計算結果があれば読み込んで再開するか
    runner_create.file_write         = true;            // 計算結果をファイルに保存するか
    runner_create.print_progress     = true;            // 途中結果を表示
    runner_create.initial_evaluation = file_read;       // ファイルを読んだ場合は最初に評価しておく
    auto runner = bb::Runner<float>::Create(runner_create);
    runner->Fitting(td, epoch_size, mini_batch_size);
}



// end of file
