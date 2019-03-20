------------------------------------------------------------------------------
 BinaryBrain  MNIST�T���v��
                                         Copyright (C) 2018 by Ryuji Fuchikami
                                         https://github.com/ryuz
                                         ryuji.fuchikami@nifty.com
------------------------------------------------------------------------------


�y�T�v�z
  �{�f�B���N�g���� LUT-Network �ɂ� MNIST �f�[�^���w�K����T���v���ꎮ��
�Ȃ�܂��B
  �������������������̂ŁA�w�K���Ԃ�Z�߂ɐݒ肵�Ă���A�F�����Ȃǂ�
��������܂���̂ł��炩���߂��������������B


�y�t�@�C���\���z
  Makefile                           Linux�p���C�N�t�@�C��
  get_nmist.bat                      NMIST�_�E�����[�h�p(Windows+cygwin�Ȃ�)
  get_nmist.sh                       NMIST�_�E�����[�h�p(Linux)
  main.cpp                           main�֐�
  MnistSimpleLutMlp.cpp              LUT-Network �P��MLP�T���v��
  MnistSimpleLutCnn.cpp              LUT-Network CNN�T���v��
  MnistDenseAffine.cpp               �ʏ�DNN�̒P��MLP�T���v��
  MnistSimpleMicroMlpScratch.cpp     �l�b�g���X�N���b�`�ŏ����ꍇ�̃T���v��
  readme.txt                         �{�t�@�C��
  sample_mnist.sln                   Visual-C++ 2017�p�\�����[�V����
  sample_mnist.vcxproj               Visual-C++ 2017�p�v���W�F�N�g
  sample_mnist.vcxproj.filters       Visual-C++ 2017�p
  sample_mnist.vcxproj.user          Visual-C++ 2017�p
  verilog/bb_lut.v                   LUT �� Verilog���f��
  verilog/tb_mnist_lut_net.v         �P��MLP LUT-Network �̃e�X�g�x���`
  verilog/tb_mnist_lut_net.vtakprj   �P��MLP LUT-Network ��Veritak�v���W�F�N�g


�y�r���h���@�z
 [Linux�̏ꍇ]
  make all

  �Ńr���h����� ���s�t�@�C�� sample_mnist ���o���܂�

  �Ȃ��A������
  make WITH_CUDA=No all
  �Ƃ���ƁACUDA������CPU�ł��r���h����܂�


  �P���Ɏ��s����ɂ�

  make WITH_CUDA=Yes run

  �Ƃ���ƁAMNIST �f�[�^���Ȃ���΃_�E�����[�h���Ă���
  ./sample_mnist All

  �����s����܂��B

  sample_mnist �̈�����

  LutMlp                   LUT-Network�̒P�����w�p�[�Z�v�g���������s
  LutCnn                   LUT-Network��CNN�����s
  DenseAffine              ���ʂ̒P�����w�p�[�Z�v�g���������s
  SimpleMicroMlpScratch    �P�����w�p�[�Z�v�g�������x�^�������T���v�������s
  All                      ��̂��ׂĂ����s

  �ƂȂ��Ă��܂��B


 [Windows�̏ꍇ]
  Visual C++ 2017 �Ńr���h�ł��܂��B
  MNIST�t�@�C���Ȃǂ͎蓮�_�E�����[�h���K�v�ł��B



�yMLP �� Verilog �V�~�����[�V�����܂Łz

  ./sample_mnist LutMlp

  �����s����ƁA�w�K������ verilog �f�B���N�g����

  mnist_train.txt       �g���[�j���O�f�[�^
  mnist_test.txt        �]���f�[�^
  MnistSimpleLutMlp.v   �w�K�ς݂� RTL

  ���o�͂���܂��B

  ���L���A���炩�̃V�~�����[�^�ŃV�~�����[�V�������s����ƁA
�w�K���ʂ������܂��B

  tb_mnist_lut_net.v
  MnistSimpleLutMlp.v
  bb_lut.v

  tb_mnist_lut_net.vtakprj �� Veritak �p�̃v���W�F�N�g�ƂȂ��Ă���܂��̂ŁA
Veritak �����p�̃��[�U�[�͂����Ɏ������Ƃ��o���܂��B



�yCNN �� Verilog �V�~�����[�V�����܂Łz
  ���݂܂�Ver3�ɈڐA���I����Ă��܂���B
  ���΂炭���҂��������� m(_ _)m



------------------------------------------------------------------------------
 end of file
------------------------------------------------------------------------------
