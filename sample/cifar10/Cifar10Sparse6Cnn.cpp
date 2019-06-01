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
#include "bb/SparseLutN.h"
#include "bb/BinaryLutN.h"
#include "bb/LoweringConvolution.h"
#include "bb/BatchNormalization.h"
#include "bb/ReLU.h"
#include "bb/MaxPooling.h"
#include "bb/Reduce.h"
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



// CNN with LUT networks
void Cifar10Sparse6Cnn(int epoch_size, int mini_batch_size, int max_run_size, bool binary_mode, bool file_read)
{
    std::string net_name = "Cifar10Sparse6Cnn";

  // load cifar-10 data
#ifdef _DEBUG
    auto td = bb::LoadCifar10<>::Load(1);
    std::cout << "!!! debug mode !!!" << std::endl;
#else
    auto td = bb::LoadCifar10<>::Load();
#endif

     // create network
    auto layer_cnv0_sl0 = bb::SparseLutN<6>::Create(512);
    auto layer_cnv0_sl1 = bb::SparseLutN<6>::Create(384);
    auto layer_cnv0_sl2 = bb::SparseLutN<6>::Create(64);
    auto layer_cnv1_sl0 = bb::SparseLutN<6>::Create(512);
    auto layer_cnv1_sl1 = bb::SparseLutN<6>::Create(384);
    auto layer_cnv1_sl2 = bb::SparseLutN<6>::Create(64);
    auto layer_cnv2_sl0 = bb::SparseLutN<6>::Create(1024);
    auto layer_cnv2_sl1 = bb::SparseLutN<6>::Create(768);
    auto layer_cnv2_sl2 = bb::SparseLutN<6>::Create(128);
    auto layer_cnv3_sl0 = bb::SparseLutN<6>::Create(1024);
    auto layer_cnv3_sl1 = bb::SparseLutN<6>::Create(768);
    auto layer_cnv3_sl2 = bb::SparseLutN<6>::Create(64);
    auto layer_sl4      = bb::SparseLutN<6>::Create(2048);
    auto layer_sl5      = bb::SparseLutN<6>::Create(1024);
    auto layer_sl6      = bb::SparseLutN<6>::Create(360);
    auto layer_sl7      = bb::SparseLutN<6>::Create(60);
    auto layer_sl8      = bb::SparseLutN<6>::Create(10);

    {
        auto cnv0_sub = bb::Sequential::Create();
        cnv0_sub->Add(layer_cnv0_sl0);
        cnv0_sub->Add(layer_cnv0_sl1);
        cnv0_sub->Add(layer_cnv0_sl2);

        auto cnv1_sub = bb::Sequential::Create();
        cnv1_sub->Add(layer_cnv1_sl0);
        cnv1_sub->Add(layer_cnv1_sl1);
        cnv1_sub->Add(layer_cnv1_sl2);

        auto cnv2_sub = bb::Sequential::Create();
        cnv2_sub->Add(layer_cnv2_sl0);
        cnv2_sub->Add(layer_cnv2_sl1);
        cnv2_sub->Add(layer_cnv2_sl2);

        auto cnv3_sub = bb::Sequential::Create();
        cnv3_sub->Add(layer_cnv3_sl0);
        cnv3_sub->Add(layer_cnv3_sl1);
        cnv3_sub->Add(layer_cnv3_sl2);
        
        auto net = bb::Sequential::Create();
        net->Add(bb::LoweringConvolution<>::Create(cnv0_sub, 3, 3));
        net->Add(bb::LoweringConvolution<>::Create(cnv1_sub, 3, 3));
        net->Add(bb::MaxPooling<>::Create(2, 2));
        net->Add(bb::LoweringConvolution<>::Create(cnv2_sub, 3, 3));
        net->Add(bb::LoweringConvolution<>::Create(cnv3_sub, 3, 3));
        net->Add(bb::MaxPooling<>::Create(2, 2));
        net->Add(layer_sl4);
        net->Add(layer_sl5);
        net->Add(layer_sl6);
        net->Add(layer_sl7);
        net->Add(layer_sl8);
        net->SetInputShape(td.x_shape);

        // �����e�[�u���Ƃ���
        net->SendCommand("lut_binarize false");

        // print model information
        net->PrintInfo();

        // run fitting
        bb::Runner<float>::create_t runner_create;
        runner_create.name               = net_name;
        runner_create.net                = net;
        runner_create.lossFunc           = bb::LossSoftmaxCrossEntropy<float>::Create();
        runner_create.metricsFunc        = bb::MetricsCategoricalAccuracy<float>::Create();
        runner_create.optimizer          = bb::OptimizerAdam<float>::Create();
        runner_create.max_run_size       = max_run_size;    // ���ۂ�1��̎��s�T�C�Y
        runner_create.file_read          = file_read;       // �O�̌v�Z���ʂ�����Γǂݍ���ōĊJ���邩
        runner_create.file_write         = true;            // �v�Z���ʂ��t�@�C���ɕۑ����邩
        runner_create.print_progress     = true;            // �r�����ʂ�\��
        runner_create.initial_evaluation = file_read;       // �t�@�C����ǂ񂾏ꍇ�͍ŏ��ɕ]�����Ă���
        auto runner = bb::Runner<float>::Create(runner_create);
        runner->Fitting(td, epoch_size, mini_batch_size);
    }
}



// end of file
