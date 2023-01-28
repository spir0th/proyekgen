:: proyekgen deployment script, run this script for Windows builds
@echo off

set WORKDIR=%~dp0
set SRCDIR=%WORKDIR:~0,-1%\..\
set DEPLOYDIR=%SRCDIR%\build\x64-Release\proyekgen

set COMPILER=iscc.exe

set SETUP64=%WORKDIR%\setup-x64.iss
set SETUP32=%WORKDIR%\setup-x86.iss

echo Deploying proyekgen... Please wait...

dir /b /s /a %DEPLOYDIR% | findstr . > nul || (
	echo No release build has been compiled, aborting deployment...
	pause && exit /b 127
)

:: Finding out CPU architecture
reg Query "HKLM\Hardware\Description\System\CentralProcessor\0" | find /i "x86" > NUL && set ARCH=x86 || set ARCH=x64

if %ARCH% == x64 (
	%COMPILER% %SETUP64%
) else (
	if %ARCH% == x86 (
		%COMPILER% %SETUP32%
	) else (
		echo CPU architecture not supported, aborting deployment...
		pause && exit 127
	)
)

echo Deployment success.