@echo off

if exist timeit.dat (
	del timeit.dat
)

:: Get carriage return
for /f %%c in ('copy /z %~f0 nul') do (
	for /l %%i in (1, 1, 100) do (
		< nul set /p="Run: %%i/100%%c"
		bin\timeit tasklist > nul 2> nul
		bin\timeit wmic process get name,processid,parentprocessid > nul 2> nul
		bin\timeit bin\fastlist > nul 2> nul
		bin\timeit bin\smallist > nul 2> nul
		bin\timeit bin\fastlist_fastio > nul 2> nul
		< nul set /p=%%c
	)
)

bin\timeit -t
