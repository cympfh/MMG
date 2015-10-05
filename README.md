# mmg-extracted patterns

MMG (Minimal multiple generalizations) algorithm<sup>[1]</sup>
expands a tree of partial covering Regular Patterns.

# Usage

```
% ./mmg.exe
Usage: ./mmg.exe [ -K <int> ] [ -c -sub <int> -sup <int> ] [ -r -sub <double> -sup <double> ]
-K k                  run k-mmg (assert k > 0)
-c -sub c0 -sup c1    outputs all patterns when containing n var and (c0 <= n < c1) (assert c0 < c1)
-r -sub r0 -sup r1    outputs all patterns when its var-ratio is r and (r0 <= r < r1) (assert r0 < r1)
```

## k-multiple

```
./mmg.exe -k 4 < sample/data
```

generates at most 4 patterns

## filtering with var count

`-c` not determine `k`.

Assume k is infinity,
`-c` outputs all pattens
which contains `n` variable symbols
(c0 <= n < c1).

option `-c`

## filtering with var ratio

`-r` also not determine `k`.

Assume k is infinity,
`-c` outputs all pattens
whose var ratio is `r`
(r = n / length, r0 <= r < r1).

# Example

see `make test`

---

[1] [CiteSeerX — Finding Minimal Generalizations for Unions of Pattern Languages and Its Application to Inductive Inference from Positive Data.](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.52.5331)
