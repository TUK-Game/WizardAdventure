pushd %~dp0

protoc.exe -I=./ --cpp_out=./ ./Enum.proto
protoc.exe -I=./ --cpp_out=./ ./Struct.proto
protoc.exe -I=./ --cpp_out=./ ./Protocol.proto

GenPackets.exe --path=./Protocol.proto --output=ClientPacketHandler --recv=S_ --send=C_
GenPackets.exe --path=./Protocol.proto --output=ServerPacketHandler --recv=C_ --send=S_

IF ERRORLEVEL 1 PAUSE

XCOPY /Y Enum.pb.h "../../../Project/GameServer"
XCOPY /Y Enum.pb.cc "../../../Project/GameServer"
XCOPY /Y Struct.pb.h "../../../Project/GameServer"
XCOPY /Y Struct.pb.cc "../../../Project/GameServer"
XCOPY /Y Protocol.pb.h "../../../Project/GameServer"
XCOPY /Y Protocol.pb.cc "../../../Project/GameServer"
XCOPY /Y ServerPacketHandler.h "../../../Project/GameServer"

XCOPY /Y Enum.pb.h "../../../../Project/Engine"
XCOPY /Y Enum.pb.cc "../../../../Project/Engine"
XCOPY /Y Struct.pb.h "../../../../Project/Engine"
XCOPY /Y Struct.pb.cc "../../../../Project/Engine"
XCOPY /Y Protocol.pb.h "../../../../Project/Engine"
XCOPY /Y Protocol.pb.cc "../../../../Project/Engine"
XCOPY /Y ClientPacketHandler.h "../../../../Project/Engine"

DEL /Q /F *.pb.h
DEL /Q /F *.pb.cc
DEL /Q /F *.h

PAUSE