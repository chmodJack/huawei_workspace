@echo off
;rem Run_MultiCore2.bat

;rem move into the directory
set BATCHDIR=%~dp0%
cd /d %BATCHDIR%
;rem Check Installation supports this demo
if EXIST ..\..\..\..\..\..\bin\%IMPERAS_ARCH%\checkinstall.exe (
  ..\..\..\..\..\..\bin\%IMPERAS_ARCH%\checkinstall.exe  --group run --noruntime -p install.pkg --nobanner
  if ERRORLEVEL 1 ( goto end )
)
%IMPERAS_ISS% --verbose --program ../../../Applications/multicore2/multicore2.AARCH64-O1-g.elf ^
    --processorvendor arm.ovpworld.org --processorname arm --variant AArch64 ^
     --numprocessors 2 --hishare 0x800FFFFF --loshare 0x80000000   --parameter endian=little     %* 

:end
pause
