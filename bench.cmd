@echo off

cd bin

if exist timeit.dat (
   del timeit.dat
)

for /f %%c in ('copy /z %~f0 nul') do (
   for /l %%i in (1, 1, 200) do (
      < nul set /p="Run: %%i/200%%c"
      timeit tasklist > nul 2> nul
      timeit wmic process get name,processid,parentprocessid > nul 2> nul
      timeit fastlist-0.3.0-x64.exe > nul 2> nul
      timeit smallist.exe > nul 2> nul
      @REM timeit smallist32.exe > nul 2> nul
      timeit fastlist_fastio.exe > nul 2> nul
      timeit fastlist-rs.exe > nul 2> nul
      < nul set /p=%%c
   )
)

timeit -t
