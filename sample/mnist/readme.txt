MNIST�T���v��

�y��{�z
[Linux�̏ꍇ]
  make WITH_CUDA=Yes all

  �Ńr���h����� ���s�t�@�C�� sample_mnist ���o����

  make WITH_CUDA=Yes run

  �Ƃ���ƁAMNIST �f�[�^���Ȃ���΃_�E�����[�h���Ă���
  ./sample_mnist All

  �����s�����B
  sample_mnist �̈�����

  LutMlp                   LUT-Network�̒P�����w�p�[�Z�v�g���������s
  LutCnn                   LUT-Network��CNN�����s
  DenseAffine              ���ʂ̒P�����w�p�[�Z�v�g���������s
  SimpleMicroMlpScratch    �P�����w�p�[�Z�v�g�������x�^�������T���v�������s
  All                      ��̂��ׂĂ����s

  �ƂȂ��Ă���


[Windows�̏ꍇ]
  Visual C++ 2017 �Ńr���h�ł���BMNIST�t�@�C���Ȃǂ͎蓮�_�E�����[�h���K�v


�yMLP�̎������z
  ���݁AMLP�ł̂� Verilog�܂œ���m�F

  ./sample_mnist LutMlp

  �����s����� verilog �f�B���N�g����

  mnist_train.txt       �g���[�j���O�f�[�^
  mnist_test.txt        �]���f�[�^
  MnistSimpleLutMlp.v   �w�K�ς݂� RTL

  ���o�͂����

  ���L���A�V�~�����[�V�������s����ƁA�w�K���ʂ�������B

  tb_mnist_lut_net.v
  MnistSimpleLutMlp.v
  bb_lut.v

  �Ȃ��Atb_mnist_lut_net.vtakprj �� Veritak �p�̃v���W�F�N�g�ł���B

  �����_�ł́A�p�X��ʂ��m�F�݂̂ŁA�F�����͈����̂ł��������������B
  �o�C�i���̏ꍇ�A���o�͂ɍH�v���K�v�ł����A��₱�����Ȃ�̂�
��U�V���v���Ȏ����ɂ��Ă��܂��B

�yCNN�̎������z
  ���݂܂�Ver3�ɈڐA���I����Ă��܂���B
  ���΂炭���҂��������� m(_ _)m


