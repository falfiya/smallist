# smallist

*Like `fastlist` but 0.7% the size (and 2x faster)*

Benchmarks were run on my home machine:
   - CPU: AMD Ryzen 3700X
   - Memory: 16GB

```
Runs Name                      Elapsed Time   Process Time    System   Context    Page    Total I/O
                                                               Calls  Switches   Faults
80   fastlist-0.3.0-x64.exe     0:00:00.009   0:00:00.009      3620        94     1518      110786
80   fastlist_fastio.exe        0:00:00.004   0:00:00.003       980        60     1267        9318
80   smallist.exe               0:00:00.004   0:00:00.005      1021        66     1287       15519
80   smallist32.exe             0:00:00.005   0:00:00.006      1222        74     1618       22796
80   tasklist                   0:00:00.147   0:00:00.079     63381     19949     3466      407810
80   wmic                       0:00:00.105   0:00:00.071     28318      2358     7744      105726
```

```
2023.01.15  06:08 PM           271,872 fastlist-0.3.0-x64.exe
2023.01.15  06:39 PM             5,632 fastlist_fastio.exe
2023.03.10  06:30 PM             1,712 smallist.exe
2023.03.10  06:30 PM             1,584 smallist32.exe
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
