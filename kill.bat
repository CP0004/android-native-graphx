@echo off

echo Killing process...
adb shell su -c "kill $(pidof matrix_64)"

echo Done.
pause
