------------------------------------------------------------------------------
 BinaryBrain  MNIST�T���v��
                                    Copyright (C) 2018-2019 by Ryuji Fuchikami
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
  MnistLutMlp.cpp                    Binary LUT-Network MLP�T���v��
  MnistLutCnn.cpp                    Binary LUT-Network CNN�T���v��
  MnistDenseMlp.cpp                  FP32�̑S����DNN�� MLP�T���v��
  MnistDenseCnn.cpp                  FP32�̑S����CNN�� CNN�T���v��
  readme.txt                         �{�t�@�C��
  sample_mnist.sln                   Visual-C++ 2017�p�\�����[�V����
  sample_mnist.vcxproj               Visual-C++ 2017�p�v���W�F�N�g
  sample_mnist.vcxproj.filters       Visual-C++ 2017�p
  sample_mnist.vcxproj.user          Visual-C++ 2017�p
  verilog/bb_lut.v                   LUT �� Verilog���f��
  verilog/tb_mnist_lut_mlp.v         MLP LUT-Network �̃e�X�g�x���`
  verilog/tb_mnist_lut_mlp.vtakprj   MLP LUT-Network ��Veritak�v���W�F�N�g
  verilog/iverilog_lut_mlp.bat       MLP LUT-Network ��iverilog���s(Win)
  verilog/iverilog_lut_mlp.sh        MLP LUT-Network ��iverilog���s(Linux)
  verilog/iverilog_lut_mlp_cmd.txt   MLP LUT-Network ��iverilog�R�}���h
  verilog/tb_mnist_lut_cnn.v         CNN LUT-Network �̃e�X�g�x���`
  verilog/tb_mnist_lut_cnn.vtakprj   CNN LUT-Network ��Veritak�v���W�F�N�g
  verilog/iverilog_lut_cnn.bat       CNN LUT-Network ��iverilog���s(Win)
  verilog/iverilog_lut_cnn.sh        CNN LUT-Network ��iverilog���s(Linux)
  verilog/iverilog_lut_cnn_cmd.txt   CNN LUT-Network ��iverilog�R�}���h
  verilog/video_mnist_cnn.v          CNN���W���[��
  verilog/video_mnist_cnn_core.v     CNN���W���[���̃R�A
  verilog/video_dnn_max_count.v      �N���X�^�����O���ʂ̃J�E���e�B���O
  verilog/video_mnist_color.v        ���ʒ��F���W���[��
  verilog/video_mnist_color_core.v   ���ʒ��F���W���[���̃R�A


�y�r���h���@�z
 [Linux�̏ꍇ]
  make all

  �Ńr���h����� ���s�t�@�C�� sample-mnist ���o���܂�

  �Ȃ��A������
  make WITH_CUDA=No all
  �Ƃ���ƁACUDA������CPU�ł��r���h����܂�

  make dl_mnist

  �Ǝ��s����ƁAMNIST�̃f�[�^���_�E�����[�h���܂��B

  �������s����ɂ�

  ./sample-mnist All

  �Ƃ���ƁA���ׂĂ̓����T���v�������ԂɎ��s����܂��B

  sample_mnist �̈�����

  LutMlp                   LUT-Network�̑��w�p�[�Z�v�g���������s
  LutCnn                   LUT-Network��CNN�����s
  DenseMlp                 FP32�S�����̑��w�p�[�Z�v�g���������s
  DenseCnn                 FP32�S������CNN�����s
  All                      ��̂��ׂĂ����s

  �ƂȂ��Ă���A�����������f���������s���邱�Ƃ��\�ł��B
  �܂� -epoch �I�v�V�����Ȃǂ� epoch ���̎w����\�ł��B�ڂ����� main.cpp ���m�F���������B


 [Windows�̏ꍇ]
  Visual C++ 2017 �Ńr���h�ł��܂��B
  MNIST�t�@�C���Ȃǂ͎蓮�_�E�����[�h���K�v�ł��B



�yMLP �� Verilog �V�~�����[�V�����܂Łz

  ./sample-mnist LutMlp

  �����s����ƁA�w�K������ verilog �f�B���N�g���̉���

  mnist_train.txt  �g���[�j���O�f�[�^
  mnist_test.txt   �]���f�[�^
  MnistLutMlp.v    �w�K�ς݂� RTL

  ���o�͂���܂��B

  ���L���A���炩�̃V�~�����[�^�ŃV�~�����[�V�������s����ƁA
�w�K���ʂ������܂��B

  tb_mnist_lut_mlp.v
  bb_lut.v
  MnistLutMlp.v

  iverilog(Icarus Verilog)�p�� iverilog_lut_mlp.sh �Ƃ����X�N���v�g��
�p�ӂ��Ă��܂�(���A�l�b�g���[�N�̓��������\�x���ł�)�B

  tb_mnist_lut_mlp.vtakprj �� Veritak �p�̃v���W�F�N�g�ƂȂ��Ă���܂��̂ŁA
Windows�� Veritak �����p�̃��[�U�[�͊��p���������B



�yCNN �� Verilog �V�~�����[�V�����܂Łz

  ./sample-mnist LutCnn

  �����s����ƁA�w�K������ verilog �f�B���N�g���̉���

  mnist_test_160x120.ppm  �e�X�g�摜(160x120)
  mnist_test_640x480.ppm  �e�X�g�摜(640x480)
  MnistLutCnn.v           �w�K�ς݂� RTL

  iverilog(Icarus Verilog)�p�� iverilog_lut_cnn.sh �Ƃ����X�N���v�g��
�p�ӂ��Ă��܂�(���A�l�b�g���[�N�̓��������\�x���ł�)�B

  tb_mnist_lut_cnn.vtakprj �� Veritak �p�̃v���W�F�N�g�ƂȂ��Ă���܂��̂ŁA
Windows�� Veritak �����p�̃��[�U�[�͊��p���������B

  CNN �̓���ɂ� Jelly(https://github.com/ryuz/jelly) �𗘗p���Ă���A
git �� submodule �ɂĎ�荞��ŁA�K�v�ȃt�@�C�����Q�Ƃ��Ă��܂��̂�
�����ӂ��������B

  ���Ȃ莞�Ԃ��|����܂����A���܂��s���� col_0.ppm �Ȃǂ��Appm�`���̐F�Â����ꂽ
���ʉ摜�Ƃ��ďo�͂���܂��B
  �Ȃ� MaxPooling �� 2��ʂ��Ă��邽�߁A�o�̓T�C�Y�͏c�����ꂼ�ꌳ��
�T�C�Y�� 1/4 �ƂȂ�܂��B

  �F��  0:�� 1:�� 2:�� 3:�� 4:�� 5:�� 6:�� 7:�� 8:�D 9:�� �̃J���[�R�[�h��
���F���Ă��܂��B


------------------------------------------------------------------------------
 end of file
------------------------------------------------------------------------------
