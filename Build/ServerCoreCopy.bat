pushd %~dp0

xcopy "..\Server\Project\ServerCore\*.h" "..\Source\ServerCore" /s /y /exclude:Exclude.txt /i

pause