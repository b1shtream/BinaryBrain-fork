@echo off

call set_vc.bat

echo �����L�[�������� git pull ���J�n���܂�
pause
git pull


echo �����L�[��������py37�̃r���h���J�n���܂�
pause

call set_py37.bat
call clean.bat
call copy_src.bat

python setup.py build

python setup.py sdist
python setup.py bdist_wheel


echo �����L�[��������py37��TestPyPI�փA�b�v���[�h���J�n���܂�
pause
twine upload --repository testpypi dist/*


echo �����L�[��������py37��PyPI�փA�b�v���[�h���J�n���܂�
pause
twine upload --repository pypi dist/*



echo �����L�[��������py36�̃r���h���J�n���܂�
pause

call set_py36.bat
call clean.bat
call copy_src.bat

python setup.py build

python setup.py sdist
python setup.py bdist_wheel


echo �����L�[��������py36��TestPyPI�փA�b�v���[�h���J�n���܂�
pause
twine upload --repository testpypi dist/*


echo �����L�[��������py36��PyPI�փA�b�v���[�h���J�n���܂�
pause
twine upload --repository pypi dist/*



echo �����L�[��������py38�̃r���h���J�n���܂�
pause

call set_py38.bat
call clean.bat
call copy_src.bat

python setup.py build

python setup.py sdist
python setup.py bdist_wheel


echo �����L�[��������py38��TestPyPI�փA�b�v���[�h���J�n���܂�
pause
twine upload --repository testpypi dist/*


echo �����L�[��������py38��PyPI�փA�b�v���[�h���J�n���܂�
pause
twine upload --repository pypi dist/*
