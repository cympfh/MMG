# mmg-extracted patterns

MMG (Minimal multiple generalizations) algorithm<sup>[1]</sup>
expands a tree of partial covering Regular Patterns.

## k-multiple

option `-k`

```
./mmg.exe -k 4 < sample/data
```

generates at most 4 patterns
which expresses the all `sample/data`.

## filtering with var_count

option `-c`

```
./mmg.exe -c < sample/job
```

expands the whole tree, and collect all patterns  
such that

`csub <= var_count < csup`

where
`var_count` is the num of vars in the pattern.  
In default `csub = 1` and `csup = 4` (written in `mmg.h`).

### `-sub` and `-sup`

```
./mmg.exe -c -sub 3 -sup 6 < sample/job
```

## filtering with var_ratio

optione `-r`

uses the `var_ratio` (the ratio of var in the pattern)  
instead of `var_count` in `-c`

`rsub <= var_ratio < rsup`

In default (written in `mmg.h`), `rsub = 0.2` and `rsup = 0.8`.

---

[1] [CiteSeerX — Finding Minimal Generalizations for Unions of Pattern Languages and Its Application to Inductive Inference from Positive Data.](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.52.5331)
