REM 既にfilelist.txtがある場合には削除する 
if exist filelist.txt del filelist.txt

REM for文にて特定拡張子のファイルを検索する
for /r ".\" %%f in (*.txt) do (
  echo %%~nxf >> filelist.txt
)

pause