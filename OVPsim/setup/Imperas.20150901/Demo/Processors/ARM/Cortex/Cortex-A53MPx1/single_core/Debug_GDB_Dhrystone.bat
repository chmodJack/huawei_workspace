@echo off
;rem Debug_GDB_Dhrystone.bat

;rem move into the directory
set BATCHDIR=%~dp0%
cd /d %BATCHDIR%
;rem Check Installation supports this demo
if EXIST ..\..\..\..\..\..\bin\%IMPERAS_ARCH%\checkinstall.exe (
  ..\..\..\..\..\..\bin\%IMPERAS_ARCH%\checkinstall.exe  --group run --noruntime -p install.pkg --nobanner
  if ERRORLEVEL 1 ( goto end )
)
%IMPERAS_ISS% --verbose --program ../../../Applications/dhrystone/dhrystone.AARCH64-O1-g.elf ^
    --processorvendor arm.ovpworld.org --processorname arm --variant Cortex-A53MPx1 ^
    --gdbconsole --numprocessors 1     --parameter endian=little    --gdbinit dhrystone.gdb %* 

:end
pause
