pushd %~dp0

protoc.exe -I=./ --cpp_out=./ ./Enum.proto
protoc.exe -I=./ --cpp_out=./ ./Struct.proto
protoc.exe -I=./ --cpp_out=./ ./Protocol.proto

GenPackets.exe --path=./Protocol.proto --output=ClientPacketHandler --name=Client --recv=S_ --send=C_
GenPackets.exe --path=./Protocol.proto --output=ServerPacketHandler --name=Server --recv=C_ --send=S_

IF ERRORLEVEL 1 PAUSE

XCOPY /Y Enum.pb.h "../../../Project/Engine"
XCOPY /Y Enum.pb.cc "../../../Project/Engine"
XCOPY /Y Struct.pb.h "../../../Project/Engine"
XCOPY /Y Struct.pb.cc "../../../Project/Engine"
XCOPY /Y Protocol.pb.h "../../../Project/Engine"
XCOPY /Y Protocol.pb.cc "../../../Project/Engine"
XCOPY /Y ClientPacketHandler.h "../../../Project/Engine"
XCOPY /Y ServerPacketHandler.h "../../../Project/Engine"
XCOPY /Y PacketFlag.h "../../../Project/Engine"

DEL /Q /F *.pb.h
DEL /Q /F *.pb.cc
DEL /Q /F *.h

PAUSE