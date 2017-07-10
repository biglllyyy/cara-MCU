@echo off

set d=%date:~0,10%
set t=%time:~0,8%
echo #define BUILD_DATE  "%d% %t%"