# mmg-extracted patterns

MMG (Minimal multiple generalizations) algorithm<sup>[1]</sup>
expand the partial covering pattern-tree.
This extracts the tree of multiple-nonerasing-regular-pattern, then
collect all patterns s.t.

- $csub <= var\_count < csup$ (var count goodness mode) or
- $rsub <= var\_ratio < rsup$ (var ratio goodness mode)

where
var\_count is the num of vars in the pattern, and
var\_ratio is the ratio of vars.

## options

- `-c`: var count goodness mode (in default)
- `-r`: var ratio goodness mode
- `-sub`: csub or rsub
- `-sup`: csup or rsup

---

[1] [CiteSeerX — Finding Minimal Generalizations for Unions of Pattern Languages and Its Application to Inductive Inference from Positive Data.](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.52.5331)
