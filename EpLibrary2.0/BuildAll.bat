if not exist "C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv.exe" goto :VS9Compile
"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv" EpLibrary80.sln /build "Debug" /project "EpLibrary\EpLibrary80.vcproj" /projectconfig "Debug"
"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv" EpLibrary80.sln /build "Debug DLL" /project "EpLibrary\EpLibrary80.vcproj" /projectconfig "Debug DLL"
"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv" EpLibrary80.sln /build "Debug DLL Unicode" /project "EpLibrary\EpLibrary80.vcproj" /projectconfig "Debug DLL Unicode"
"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv" EpLibrary80.sln /build "Debug Unicode" /project "EpLibrary\EpLibrary80.vcproj" /projectconfig "Debug Unicode"
"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv" EpLibrary80.sln /build "Release" /project "EpLibrary\EpLibrary80.vcproj" /projectconfig "Release"
"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv" EpLibrary80.sln /build "Release DLL" /project "EpLibrary\EpLibrary80.vcproj" /projectconfig "Release DLL"
"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv" EpLibrary80.sln /build "Release DLL Unicode" /project "EpLibrary\EpLibrary80.vcproj" /projectconfig "Release DLL Unicode"
"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv" EpLibrary80.sln /build "Release Unicode" /project "EpLibrary\EpLibrary80.vcproj" /projectconfig "Release Unicode"

:VS9Compile

if not exist "C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv.exe" goto :VS10Compile

"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpLibrary90.sln /build "Debug" /project "EpLibrary\EpLibrary90.vcproj" /projectconfig "Debug"
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpLibrary90.sln /build "Debug DLL" /project "EpLibrary\EpLibrary90.vcproj" /projectconfig "Debug DLL"
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpLibrary90.sln /build "Debug DLL Unicode" /project "EpLibrary\EpLibrary90.vcproj" /projectconfig "Debug DLL Unicode"
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpLibrary90.sln /build "Debug Unicode" /project "EpLibrary\EpLibrary90.vcproj" /projectconfig "Debug Unicode"
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpLibrary90.sln /build "Release" /project "EpLibrary\EpLibrary90.vcproj" /projectconfig "Release"
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpLibrary90.sln /build "Release DLL" /project "EpLibrary\EpLibrary90.vcproj" /projectconfig "Release DLL"
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpLibrary90.sln /build "Release DLL Unicode" /project "EpLibrary\EpLibrary90.vcproj" /projectconfig "Release DLL Unicode"
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpLibrary90.sln /build "Release Unicode" /project "EpLibrary\EpLibrary90.vcproj" /projectconfig "Release Unicode"

:VS10Compile

if not exist "C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv.exe" goto :Done

"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpLibrary100.sln /build "Debug" /project "EpLibrary\EpLibrary100.vcxproj" /projectconfig "Debug"
"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpLibrary100.sln /build "Debug DLL" /project "EpLibrary\EpLibrary100.vcxproj" /projectconfig "Debug DLL"
"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpLibrary100.sln /build "Debug DLL Unicode" /project "EpLibrary\EpLibrary100.vcxproj" /projectconfig "Debug DLL Unicode"
"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpLibrary100.sln /build "Debug Unicode" /project "EpLibrary\EpLibrary100.vcxproj" /projectconfig "Debug Unicode"
"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpLibrary100.sln /build "Release" /project "EpLibrary\EpLibrary100.vcxproj" /projectconfig "Release"
"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpLibrary100.sln /build "Release DLL" /project "EpLibrary\EpLibrary100.vcxproj" /projectconfig "Release DLL"
"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpLibrary100.sln /build "Release DLL Unicode" /project "EpLibrary\EpLibrary100.vcxproj" /projectconfig "Release DLL Unicode"
"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpLibrary100.sln /build "Release Unicode" /project "EpLibrary\EpLibrary100.vcxproj" /projectconfig "Release Unicode"

:Done