@echo off
echo �����L�[�������ƃA�b�v���[�h�������J�n���܂�
pause

call set_vc.bat
call clean.bat
call copy_src.bat

python setup.py build

python setup.py sdist
python setup.py bdist_wheel


echo �����L�[��������TestPyPI�փA�b�v���[�h���J�n���܂�
pause
twine upload --repository testpypi dist/*


echo �����L�[��������PyPI�փA�b�v���[�h���J�n���܂�
pause
twine upload --repository pypi dist/*
