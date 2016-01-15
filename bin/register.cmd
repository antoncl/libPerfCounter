@echo off
lodctr /m:%1
if errorlevel 1 (
   echo Failure Reason Given is %errorlevel%
   exit /b %errorlevel%
)
