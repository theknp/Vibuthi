echo "%1"
cd "%1"
xcopy ..\ThirdParty\OpenCV\vc14\bin\*.dll .\bin\x64\%2 /r /y
xcopy ..\components\ImageProc\X64\%2\*.dll .\bin\x64\%2 /r /y
xcopy ..\components\ImageProc\X64\%2\*.pdb .\bin\x64\%2 /r /y
REM xcopy ..\ThirdParty\OpenCV\vc14\bin\*.dll .\bin\x64\Release /r /y