@echo off
:loop
del "Name.exe"
if exist "Name.exe" goto :loop
del %0