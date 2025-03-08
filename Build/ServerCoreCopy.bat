pushd %~dp0

xcopy "..\Project\ServerCore\*.h" "..\Source\ServerCore" /s /y /exclude:Exclude.txt /i

pause