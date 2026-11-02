@echo off
chcp 936 >nul
cd /d "%~dp0"
demo_pipeline_sim.exe
echo.
pause
