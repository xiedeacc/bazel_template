@echo off
for /f %%i in ('git rev-parse HEAD') do set GIT_REVISION=%%i
echo STABLE_GIT_REV %GIT_REVISION%