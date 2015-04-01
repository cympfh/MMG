# mmg-extracted patterns

MMG (Minimal multiple generalizations) algorithm
expand the partial covering pattern-tree.
This extracts the tree of multiple-nonerasing-regular-pattern, then
collect all patterns s.t.

$rho^{minus} <= var\_ratio < rho^plus$

where, var\_ratio is the ratio of
the num of var
to the length of the pattern.

## usage

```bash
g++ -std=c++11 -o kmmg.exe ./kmmg.cc
./kmmg.exe < sample/input.txt
./kmmg.exe -m 0.3 -p 0.5 < sample/input.txt # -m rho^minus -p rho^plus
```

