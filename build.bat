@echo off

@REM :: Kill existing process
@REM echo Killing existing process...
@REM adb shell su -c "killall -9 memlib"

:: Build using ndk-build
echo Building with NDK...
call "C:/Users/mobil/AppData/Local/Android/Sdk/ndk/29.0.13113456/ndk-build"
if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    pause
    exit /b 1
)

@REM :: Push to device
@REM echo Pushing to device...
@REM adb push "C:/Users/mobil/Desktop/project/c++/yun-ang/libs/arm64-v8a/memlib" "/data/local/tmp/"
@REM if %ERRORLEVEL% NEQ 0 (
@REM     echo Push failed!
@REM     pause
@REM     exit /b 1
@REM )

@REM :: Set permissions
@REM echo Setting permissions...
@REM adb shell su -c "chmod 777 /data/local/tmp/memlib"
@REM adb shell chmod 777 /data/local/tmp/memlib

@REM :: Start game
@REM echo Starting PUBG Mobile...
@REM adb shell monkey -p com.tencent.ig -c android.intent.category.LAUNCHER 1
@REM :: Wait for game to load
@REM timeout /t 5

@REM :: Start our lib
@REM echo Starting memlib...
@REM adb shell su -c "/data/local/tmp/memlib"

echo Press any key to exit...
pause