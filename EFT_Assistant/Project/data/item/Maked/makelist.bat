REM ����filelist.txt������ꍇ�ɂ͍폜���� 
if exist filelist.txt del filelist.txt

REM for���ɂē���g���q�̃t�@�C������������
for /r ".\" %%f in (*.txt) do (
  echo %%~nxf >> filelist.txt
)

pause