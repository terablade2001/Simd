REM Set the folder of android-ndk
SET NDK_FOLDER=c:/android-ndk-r11c

REM Set the path to the Snapdragon device
SET SDPath=/data/test

REM Set to '1' in order to push the binary image & xml to the device.
SET INSTALL_BINARY_IMAGE=1


SET CURRENT_FOLDER=%~dp0


cls
cd %CURRENT_FOLDER%

REM Copy the version file...
copy ..\..\..\prj\txt\SimdVersion.h.txt %CURRENT_FOLDER%\..\..\Simd\Simd\SimdVersion.h


call %NDK_FOLDER%\ndk-build.cmd NDK_DEBUG=0
if ERRORLEVEL 1 (exit /b 1)

adb shell "cd %SDPath% && rm TestSimd"
adb push %CURRENT_FOLDER%/libs/arm64-v8a/TestSimd %SDPath%
if %INSTALL_BINARY_IMAGE% EQU 1 (
	adb push %CURRENT_FOLDER%/FHD.bdat %SDPath%
	adb push %CURRENT_FOLDER%/../../../data/cascade/lbp_face.xml %SDPath%
)
if ERRORLEVEL 1 (exit /b 1)

cd %CURRENT_FOLDER%
adb shell "cd %SDPath% && chmod 777 TestSimd && ./TestSimd"