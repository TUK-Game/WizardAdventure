xcopy ".\..\Project\Engine\*.h" ".\..\Source\Engine" /s /y /exclude:Exclude.txt 
xcopy ".\..\Project\Engine\*.inl" ".\..\Source\Engine" /s /y /exclude:Exclude.txt 
xcopy ".\..\Project\Engine\*.hpp" ".\..\Source\Engine" /s /y /exclude:Exclude.txt
xcopy ".\..\Project\Engine\*.hlsli" ".\..\Content\Shaders" /s /y /exclude:Exclude.txt
xcopy ".\..\Project\Engine\*.hlsl" ".\..\Content\Shaders" /s /y /exclude:Exclude.txt