# mmg-extracted patterns

MMG (Minimal multiple generalizations) algorithm<sup>[1]</sup>
expands multiple Regular Patterns from a set of Texts.
The division step
approximately
minimizes the language-size with most fitting cover<sup>[2]</sup>.

# Usage

```
   ./mmg.exe
Usage: ./mmg.exe [-s] [-K <int>] [-l <double>]
-K <k>                run k-mmg (assert k > 0)
-l <limit>            limit gain
-s                    silent mode
```

## k-multiple

```bash
./mmg.exe -k 4 < sample/data
```

generates at most 4 patterns

## gain-limit

When `k` cant determined,

```bash
./mmg.exe -l 1.0 < sample/algorithm
```

---

[1] [Finding Minimal Generalizations for Unions of Pattern Languages and Its Application to Inductive Inference from Positive Data.](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.52.5331)
[2] [Inferring Unions of the Pattern Languages by the Most Fitting Covers](http://link.springer.com/chapter/10.1007/11564089_22)
