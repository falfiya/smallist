# smallist

*Like `fastlist` but 0.7% the size (and 2x faster)*

```
Runs Name                      Elapsed Time   Process Time    System   Context    Page    Total I/O
                                                               Calls  Switches   Faults
80   bin\fastlist               0:00:00.006   0:00:00.004      3070        79     1362        5606
80   bin\smallist               0:00:00.003   0:00:00.001       904        60     1166        5288
80   tasklist                   0:00:00.132   0:00:00.073     53788     16355     3292      267542
80   wmic                       0:00:00.103   0:00:00.071     25169      1780     6926       43457
```

For each process, `smallist` prints the process ID, the parent process ID, and
the executable file, each separated by a `\t`, followed by a `\n`. For example:

```batch
> fastlist.exe
0       0       [System Process]
4       0       System
72      4       Secure System
128     4       Registry
520     4       smss.exe
776     640     csrss.exe
864     640     wininit.exe
```
