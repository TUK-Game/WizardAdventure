pushd %~dp0

xcopy "..\Server\Project\ServerCore\*.h" "..\Source\ServerCore" /s /y /exclude:Exclude.txt /i
xcopy "..\Server\Bin\ServerCore\ServerCore_d.lib" "..\Bin\ServerCore" /s /y /exclude:Exclude.txt /i
xcopy "..\Server\Bin\ServerCore\ServerCore.lib" "..\Bin\ServerCore" /s /y /exclude:Exclude.txt /i

pause