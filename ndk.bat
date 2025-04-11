@echo off

echo Killing process...
adb shell su -c "kill $(pidof matrix_64)"

echo Executing ndk-build...
call C:\Users\mobil\AppData\Local\Android\Sdk\ndk\26.1.10909125\ndk-build
echo ndk-build completed.

echo Pushing file to device...
adb push "C:/Users/mobil/OneDrive/Desktop/project/C++/touch-sm/libs/arm64-v8a/matrix_64" "/data/local/tmp"

echo Chmod app...
adb shell chmod 777 /data/local/tmp/matrix_64

@REM echo Start app...
@REM adb shell su -c /data/local/tmp/matrix_64

echo Press "q" to kill the process and exit the script.

echo Done.
pause
