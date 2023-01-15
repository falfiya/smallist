# smallist

*Like `fastlist` but 0.7% the size (and 2x faster)*

```
Runs Name                      Elapsed Time   Process Time    System   Context    Page    Total I/O
                                                               Calls  Switches   Faults
83   smallist.exe               0:00:00.005   0:00:00.004      1813       539     1381       25447
83   fastlist_fastio.exe        0:00:00.006   0:00:00.006      2112       619     1639       67909
83   fastlist-0.3.0-x64.exe     0:00:00.008   0:00:00.010      5184       926     1699       70417
83   wmic                       0:00:00.106   0:00:00.074     42975     10109     9728     1323837
83   tasklist                   0:00:00.139   0:00:00.077     79334     28915     6063     1431833
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

## development

Run `make build` to build `smallist.exe`. The resulting binary will be in `bin`.
