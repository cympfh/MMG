#include <bits/stdc++.h>
#include <gmpxx.h>
using namespace std;
using Integer = mpz_class;
#define rep(i,n) for(int i=0;i<(n);++i)

int log(int tei, int n) {
  if (n < 1) return 0;
  if (n < tei) return 1;
  return 1 + log(tei, n / tei);
}

int log(int tei, Integer n) {
  if (n < 1) return 0;
  if (n < tei) return 1;
  return 1 + log(tei, n / tei);
}

bool subset(const set<int>&a, const set<int>&b) {
  for (int x: a) if (not b.count(x)) return false;
  return true;
}

set<int> setcover(vector<pair<set<int>, Integer>> ls) {
  auto bk = ls;
  const int N = ls.size();
  set<int> ret;
  set<int> uncovered;
  for (auto& p: ls) {
    for (int x: p.first) {
      uncovered.insert(x);
    }
  }
  int tei = 4;
  while (uncovered.size() > 0) {
    int k = -1;
    int mr;
    rep (i, N) {
      int s = ls[i].first.size();
      if (s == 0) continue;
      int r = log(tei, s) - log(tei, ls[i].second);
      if (k < 0 or mr < r) { // maximize
        k = i;
        mr = r;
      }
    }
    for (int j: ret) {
      if (subset(bk[j].first, bk[k].first)) {
        ret.erase(j);
      }
    }
    ret.insert(k);
    for (int x: ls[k].first) {
      uncovered.erase(x);
    }
    for (int x: ls[k].first) {
      rep (i, N) {
        ls[i].first.erase(x);
      }
    }
  }
  return ret;
}

set<int> unweighted_setcover(vector<pair<set<int>, Integer>> ls) {
  auto bk = ls;
  const int N = ls.size();
  set<int> ret;
  set<int> uncovered;
  for (auto& p: ls) {
    for (int x: p.first) {
      uncovered.insert(x);
    }
  }
  while (uncovered.size() > 0) {
    int k = -1;
    int mr;
    rep (i, N) {
      int r = ls[i].first.size();
      if (r == 0) continue;
      if (k < 0 or mr < r) {
        k = i;
        mr = r;
      }
    }
    for (int j: ret) {
      if (subset(bk[j].first, bk[k].first)) {
        ret.erase(j);
      }
    }
    ret.insert(k);
    for (int x: ls[k].first) {
      uncovered.erase(x);
    }
    rep (i, N) {
      for (int x: ls[k].first) {
        ls[i].first.erase(x);
      }
    }
  }
  return ret;
}

