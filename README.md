# smallist

*Like `fastlist` but 0.7% the size (and 2x faster)*

Benchmarks were run on my home machine:
   - CPU: AMD Ryzen 3700X
   - Memory: 16GB

```
Runs Name                      Elapsed Time   Process Time    System   Context    Page    Total I/O
                                                               Calls  Switches   Faults
160  fastlist-0.3.0-x64.exe     0:00:00.007   0:00:00.008      3699       173     1498       33142
160  fastlist_fastio.exe        0:00:00.004   0:00:00.004      1133       131     1268       11278
160  smallist.exe               0:00:00.004   0:00:00.004      1179       148     1311       35524
160  smallist32.exe             0:00:00.005   0:00:00.005      1381       165     1641       29051
160  tasklist                   0:00:00.151   0:00:00.076     64993     20598     4325      828846
160  wmic                       0:00:00.107   0:00:00.077     29451      3193     7748      278393
```

```
2023.01.15  07:08 PM           271,872 fastlist-0.3.0-x64.exe
2023.01.15  07:39 PM             5,632 fastlist_fastio.exe
2023.03.23  02:34 AM             1,680 smallist.exe
2023.03.23  02:34 AM             1,552 smallist32.exe
```

For each process, `smallist` prints the process id, the parent process id, and
the executable file path, each separated by a tab.

```batch
> smallist
0       0       [System Process]
4       0       System
172     4       Registry
476     4       smss.exe
640     628     csrss.exe
784     628     wininit.exe
856     784     services.exe
876     784     lsass.exe
```
