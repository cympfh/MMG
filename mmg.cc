#include <bits/stdc++.h>
#include "./mmg.h"

using namespace std;
using vi = vector<int>;

#define rep(i,n) for(int i=0;i<(n);++i)
#define loop for(;;)
#define trace(var) cerr<<">>> "<<#var<<" = "<<var<<endl;

template<class T>
ostream& operator<<(ostream& os, vector<T> v) {
  if (v.size() == 0) {
    os << "(empty vector)";
    return os;
  }
  os << v[0];
  for (int i = 1, len = v.size(); i < len; ++i) {
    os << " " << v[i];
  }
  return os;
}

template<class S, class T>
ostream& operator<<(ostream& os, pair<S,T> p) {
  os << '(' << p.first << ", " << p.second << ')';
  return os;
}

vector<string> as_words(string s) {
  stringstream sin(s);
  vector<string> words;
  for (;;) {
    string s; sin >> s;
    if (!sin) break;
    words.push_back(s);
  }
  return words;
}

struct RP
{
  bool is_var;
  string str;
  RP() { is_var = true; }
  RP(string s) { is_var = false; str = s; }
  bool operator!=(RP&rh) {
    if (this->is_var && not rh.is_var) return true;
    if (not this->is_var && rh.is_var) return true;
    if (this->is_var && rh.is_var) return false;
    return this->str != rh.str;
  }
  bool operator==(RP&rh) {
    return not (*this == rh);
  }
};

using P = vector<RP>;
using Text = vector<string>;

bool operator<(RP a, RP b) {
  if (a.is_var) {
    return not b.is_var;
  }
  if (b.is_var) return false;
  return a.str < b.str;
}

ostream& operator<<(ostream& os, RP r) {
  os << (r.is_var ?  "<>" : r.str);
  return os;
}

inline
int var_count(const P& s) {
  const int n = s.size();
  int m = 0;
  rep (i, n) if (s[i].is_var) ++m;
  return m;
}

inline
double var_ratio(const P& s) {
  const int n = s.size();
  int m = 0;
  rep (i, n) if (s[i].is_var) ++m;
  return double(m) / double(n);
}

inline
bool is_good(const P& s) {
  if (gm == VAR_COUNT) {
    const int n = var_count(s);
    return (c_sub <= n) and (n < c_sup);
  } else if (gm == VAR_RATIO) {
    const double r = var_ratio(s);
    return (rho_sub <= r) and (r < rho_sup);
  }
  return false;
}

/* non-erasing generalization system <= */

bool preceq(const string& word, const RP& u) {
  if (u.is_var) return true;
  return word == u.str;
}

bool preceq(const Text& s, const P& p) {
  int n = s.size();
  int m = p.size();
  if (n < m) return false;

  // tail matching
  while (m > 0 and not p[m-1].is_var) {
    if (preceq(s[n-1], p[m-1])) {
      --n; --m;
    } else {
      return false;
    }
  }
  if (n == 0 and m == 0) return true;
  if (m == 0) return false;

  // p should be "<>[.<>]*<>" or "[.<>]*<>"

  int __pos = 0; // of text
  int __begin = 0; // of pattern

  // head matching
  while (not p[__begin].is_var) {
    if (preceq(s[__pos], p[__begin])) {
      ++__pos; ++__begin;
    } else {
      return false;
    }
    if (__pos >= n) return false;
  }

  // p should be "<>[.<>]*<>"
  
  int __end;
  for (int i = p.size();i > 0; --i) {
    while (__begin < m and p[__begin].is_var) {
      ++__pos; ++__begin;
    }
    if (__begin >= m and __pos <= n) return true;
    if (__pos >= n) return false;

    for (__end = __begin + 1; __end < m; ++__end)
      if (p[__end].is_var) break;

    for (; __pos < n - (__end - __begin); ++__pos) {
      bool res = true;
      for (int i = 0; i < __end - __begin; ++i) {
        if (not preceq(s[__pos + i], p[__begin + i])) {
          res = false;
          break;
        }
      }
      if (res) {
        __pos += (__end - __begin);
        __begin = __end;
        break;
      }
    }
    if (__pos >= n - (__end - __begin)) return false;
  }
  return false;
}

bool language_include(const P& p, const vector<Text>& S) {
  for (auto&s: S)
    if (not preceq(s, p)) return false;
  return true;
}

vector<Text> docs;

bool language_include(const P& p, const vi& c) {
  for (int i: c)
    if (not preceq(docs[i], p)) return false;
  return true;
}

vi partial_covering(const P& p, const vi& c) {
  vi r;
  for (int i: c)
    if (preceq(docs[i], p)) r.push_back(i);
  return r;
}

set<string> collect(const P& p, const vi& c) {
  const int n = p.size();
  set<string> s;
  for (int idx: c) {
    vector<string>&words = docs[idx];
    const int m = words.size();

    int i = 0, // on p
        j = 0; // on words
    for (; i < n && j < m;)
    {
      if (p[i].is_var) {
        s.insert(words[j]);
        ++i; ++j;
      }
      else if (p[i].str == words[j]) {
        ++i; ++j;
      }
      else {
        s.insert(words[j]);
        ++j;
      }
    }
    while (j < m)
      s.insert(words[j++]);
  }
  return s;
}

// takes: one pattern and its covering
// returns: one pattern extended
P tighten(const P&p, const vi& c)
{
  const int n = p.size();
  assert((n < 300) && "What the fuck long sentence!");

  // <> -> <> <>
  {
    P r(n+1);
    rep (i, n) {
      r[i] = p[i];
      if (p[i].is_var and (i == 0 or (not p[i-1].is_var))) {
        r[i+1] = RP();
        for (int j = i+1; j < n; ++j) r[j+1] = p[j];
        if (language_include(r, c)) {
          return tighten(r, c);
        }
      }
    }
  }

  // <> -> a
  {
    P q = p;
    set<string> alphabets = collect(p, c);
    for (auto&s: alphabets) {
      for (int i = 0; i < n; ++i) {
        if (not p[i].is_var) continue;
        q[i].is_var = false;
        q[i].str = s;
        if (language_include(q, c)) {
          return tighten(q, c);
        }
        q[i].is_var = true;
      }
    }
  }

  // else
  return p;
}

vector<pair<P, vi>> division(P p, vi c) {
  const int n = p.size();
  const int M = c.size();
  set<string> alphabets = collect(p, c);

  vector<pair<P, vi>> cspc; 
  // <> -> a
  for (auto&s: alphabets) {
    rep (i, n) {
      if (not p[i].is_var) continue;
      p[i].is_var = false;
      p[i].str = s;
      auto pc = partial_covering(p, c);

      if ((pc.size() > 0) and (pc.size() < M)) {
        cspc.push_back(make_pair(p, pc));
      }
      p[i].is_var = true;
    }
  }
  for (auto&s: alphabets) {
    rep(i, n) {
      if (not p[i].is_var) continue;
      P q;

      // <> -> <> a
      for (int k = 0; k < i; ++k) q.push_back(p[k]);
      q.push_back(RP()); // q[i]
      q.push_back(RP(s)); // q[i+1]
      for (int k = i + 1; k < n; ++k) q.push_back(p[k]);
      {
        auto pc = partial_covering(q, c);
        if ((pc.size() > 0) and (pc.size() < M)) {
          cspc.push_back(make_pair(q, pc));
        }
      }

      // <> -> a <>
      q[i] = RP(s);
      q[i+1] = RP();
      {
        auto pc = partial_covering(q, c);
        if ((pc.size() > 0) and (pc.size() < M)) {
          cspc.push_back(make_pair(q, pc));
        }
      }

      // <> -> <> <>
      q[i] = RP();
      {
        auto pc = partial_covering(q, c);
        if ((pc.size() > 0) and (pc.size() < M)) {
          cspc.push_back(make_pair(q, pc));
        }
      }
    }
  }

  // 最小被覆としたいところだが、NP-hard である
  // 被覆が大きいのからあれしてこう
  sort(cspc.begin(), cspc.end(), 
      [](pair<P, vi> x,
         pair<P, vi> y) {
          return x.second.size() > y.second.size(); });

  vector<pair<P, vi>> ret;
  set<int> m;
  for (auto&pc: cspc) {
    // 被覆してない部分がある?
    bool bl = false;
    for (int i: pc.second) {
      if (m.count(i) == 0) {
        bl = true;
        break;
      }
    }
    if (bl) {
      ret.push_back(pc);
      for (int i: pc.second) m.insert(i);
    }
  }

  return ret;
}

// <> <>* -> <>
P var_simplify(P&s) {
  P r;
  bool b = false;
  for (auto&u: s) {
    if (u.is_var) {
      if (b) {
        continue;
      } else {
        r.push_back(u);
        b = true;
      }
    } else {
      r.push_back(u);
      b = false;
    }
  }
  return r;
}

vector<P> kmmg(vector<vector<string>>&_docs)
{
  docs = _docs;
  const int n = docs.size();

  vi ids(n); rep(i, n) ids[i] = i;

  P top { RP() };
  auto pc = tighten(top, ids);

  // いくつのパターンに被覆されているか
  vi cover_count(n, 1);

  // collection of (pattern::vector<RP>, covering::vector<int>)
  queue<pair<P, vi>> pcs;
  pcs.push(make_pair(pc, ids));
  vector<P> ret;

  /* division */
  while (not pcs.empty()) {
    auto pc = pcs.front(); pcs.pop();
    if (gm != K_MULTIPLE) {
      auto p = var_simplify(pc.first);
      if (is_good(p)) ret.push_back(p);
    }

    // S = Doc \ L( Pi \ p)
    vi S;
    for (int i: pc.second) {
      if (cover_count[i] == 1) S.push_back(i);
      --cover_count[i];
    }
    if (S.size() == 0) continue;

    auto pcs_next = division(pc.first, S);
    if (pcs_next.size() < 2) { // not divisible
      if (gm == K_MULTIPLE) ret.push_back(pc.first);
      continue;
    }
    if (gm == K_MULTIPLE) {
      if (ret.size() + pcs.size() + pcs_next.size() > K) {
        ret.push_back(pc.first);
        while (not pcs.empty()) {
          ret.push_back(pcs.front().first);
          pcs.pop();
        }
        return ret;
      }
    }

    for (auto&pc_next: pcs_next) {
      for (int i: pc_next.second) ++cover_count[i];
    }
    for (auto&pc_next: pcs_next) {
      // Doc - (Pi - pc_next)
      vi S;
      for (int i: pc_next.second) {
        if (cover_count[i] == 1) S.push_back(i);
        else --cover_count[i];
      }
      if (S.size() == 0) continue;
      pc_next = make_pair( tighten(pc_next.first, S), S);
      pcs.push(pc_next);
    }
  }

  return ret;
}
