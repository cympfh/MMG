#include <bits/stdc++.h>
#include "./util.cc"
#include "./mmg.h"
#include "./setcover.hh"
using namespace std;

using vi = vector<int>;
// using Integer = mpz_class;

#define rep(i,n) for(int i=0;i<(n);++i)
#define loop for(;;)
#define trace(var) cerr<<">>> "<<#var<<" = "<<var<<endl;

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

using Pattern = vector<RP>;
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

// {{{ non-erasing generalization system <=

bool preceq(const string& word, const RP& u) {
  if (u.is_var) return true;
  return word == u.str;
}

bool preceq(const Text& s, const Pattern& p) {
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
// }}}

bool language_include(const Pattern& p, const vector<Text>& S) {
  for (auto&s: S)
    if (not preceq(s, p)) return false;
  return true;
}

vector<Text> docs;

bool language_include(const Pattern& p, const vi& c) {
  for (int i: c)
    if (not preceq(docs[i], p)) return false;
  return true;
}

set<int> coverset(Pattern&p, const vi&c) {
  set<int> s;
  for (int i: c)
    if (preceq(docs[i], p)) s.insert(i);
  return s;
}

vector<string> collect(const Pattern& p, const vi& c)
{
  const int n = p.size();

  vector<string> v;

  for (int idx: c) {
    vector<string>&words = docs[idx];
    const int m = words.size();

    int i = 0, // on p
        j = 0; // on words
    for (; i < n && j < m;)
    {
      if (p[i].is_var) {
        v.push_back(words[j]);
        ++i; ++j;
      }
      else if (p[i].str == words[j]) {
        ++i; ++j;
      }
      else {
        v.push_back(words[j]);
        ++j;
      }
    }
    while (j < m) v.push_back(words[j++]);
  }

  sort(begin(v), end(v));
  v.erase(std::unique(v.begin(), v.end()), v.end());
  return v;
}

int uplength(set<int>& c) {
  int r = 0;
  for (int i: c) r = max<int>(r, docs[i].size());
  return r;
}

inline int var_count(const Pattern& p) {
  int m = 0; for (auto&u: p) { if (u.is_var) ++m; }
  return m;
}

inline bool is_text(const Pattern& p) { return var_count(p) == 0; }

// log of
int language_size(const Pattern&p, int uplen=30) {
  int n = p.size();
  int m = var_count(p);
  if (m == 0) return 0;
  return max(0, uplen - n + m);
}

// takes: one pattern and its covering
// returns: one pattern extended
Pattern tighten(const Pattern&p, const vi& c)
{
  const int n = p.size();
  assert((n < 300) && "What the fuck long sentence!");

  // <> -> <> <>
  {
    Pattern r(n+1);
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
    Pattern q = p;
    auto alphabets = collect(p, c);
    for (const string& s: alphabets) {
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

vector<pair<Pattern, set<int>>> cspc(Pattern p, const vi& c) {
  const int n = p.size();
  const int M = c.size();
  auto alphabets = collect(p, c);

  vector<pair<Pattern, set<int>>> ret; 

  // <> -> a
  for (const string& a: alphabets) {
    rep (i, n) {
      if (not p[i].is_var) continue;
      p[i].is_var = false;
      p[i].str = a;
      auto s = coverset(p, c);
      if (s.size() > 0) {
        assert(s.size() < c.size() && "<> -> a");
        ret.push_back({ p, s });
      }
      p[i].is_var = true;
    }
  }

  for (const string& a: alphabets) {
    rep(i, n) {
      if (not p[i].is_var) continue;
      Pattern q;

      // <> -> <> a
      for (int k = 0; k < i; ++k) q.push_back(p[k]);
      q.push_back(RP()); // q[i]
      q.push_back(RP(a)); // q[i+1]
      for (int k = i + 1; k < n; ++k) q.push_back(p[k]);
      {
        auto s = coverset(q, c);
        if (s.size() > 0) {
          assert(s.size() < c.size() && "<> -> <> a");
          ret.push_back({ q, s });
        }
      }
    }
  }

  { // <> -> <> <>
    Pattern q(n+1);
    rep (i, n) {
      q[i] = p[i];
      if (p[i].is_var and (i == 0 or (not p[i-1].is_var))) {
        q[i+1] = RP();
        for (int j = i+1; j < n; ++j) q[j+1] = p[j];
        auto s = coverset(q, c);
        if (s.size() > 0) {
          assert(s.size() < c.size() && "<> -> a <>");
          ret.push_back({ p, s });
        }
      }
    }
  }

  // tighten all patterns
  rep (i, ret.size()) {
    auto p = tighten(ret[i].first, set_to_vi(ret[i].second));
    ret[i].first = p;
  }

  return ret;
}

// 重みを附けるかのフラグ附
vector<pair<Pattern, vi>>
division(vector<pair<Pattern, set<int>>> & ps, const vi& c, bool weighted=true)
{
  vector<pair<Pattern, vi>> ret;
  if (ps.size() == 0) return ret;

  // 集合被覆を解く
  vector<pair<set<int>, int>> ls(ps.size());
  rep (i, ps.size()) {
    int w; // language_size(ps[i].first, uplength(ps[i].second));
    if (is_text(ps[i].first)) w = 2; else w = 3;
    ls[i] = { ps[i].second, w };
  }

  set<int> sol;
  if (weighted) {
    sol = setcover(ls);
  } else {
    sol = unweighted_setcover(ls);
  }

  vector<pair<Pattern, vi>> div;
  for (auto id: sol) div.push_back({ ps[id].first, set_to_vi(ps[id].second) });

  return div;
}

// <> <>* -> <>
Pattern var_simplify(Pattern&s) {
  Pattern r;
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

vector<Pattern> kmmg(vector<vector<string>>&_docs)
{
  docs = _docs;
  const int n = docs.size();

  vi ids(n); rep(i, n) ids[i] = i;

  Pattern top { RP() };
  auto pc = tighten(top, ids);

  // いくつのパターンに被覆されているか
  vi cover_count(n, 1);

  // (priority, Pattern, cover)
  priority_queue<pair<int, pair<Pattern, vi>>> pcs;
  pcs.push({ 1, { pc, ids }});
  vector<Pattern> ret;

  /* division */
  while (not pcs.empty())
  {
    auto pc   = pcs.top(); pcs.pop();
    Pattern p = pc.second.first;
    vi& c     = pc.second.second;

    if (DEBUG) {
      cerr << endl;
      cerr << "# div of " << p << " (" << c << ')' << endl;
      trace(language_size(p));
    }

    vi S; // S = Doc - L( Pi - p)
    for (int i: c) {
      if (cover_count[i] == 1) S.push_back(i);
      --cover_count[i];
    }
    if (S.size() == 0) continue;

    if (is_text(p)) { // not divisible clearly
      if (DEBUG) { cerr << "  clearly not-divisible " << p << endl; }
      ret.push_back(p);
      continue;
    }

    auto ps = cspc(p, S);
    if (DEBUG) {
      cerr << "\n{{{ cspc: " << ps.size() << endl;
      rep (i, min<int>(32, ps.size())) cerr << ps[i].first << " (" << ps[i].second << ')' << endl;
      cerr << "}}}" << endl;
    }

    auto qs = division(ps, S, true); // weighted
    if (DEBUG) {
      cerr << "\n{{{ weighted: " << qs.size() << endl;
      rep (i, min<int>(10, qs.size())) cerr << qs[i].first << " (" << qs[i].second << ')' << endl;
      cerr << "}}}" << endl;
    }

    if ((qs.size() == 0) or (gm == K_MULTIPLE and (ret.size() + pcs.size() + qs.size() > K))) {
      qs = division(ps, S, false); // unweighted
      if (DEBUG) {
        cerr << "\n{{{ unweighted: " << qs.size() << endl;
        rep (i, min<int>(10, qs.size())) cerr << qs[i].first << " (" << qs[i].second << ')' << endl;
        cerr << "}}}" << endl;
      }
    }

    // when not divisible
    if (qs.size() < 2) {
      if (DEBUG) { cerr << "  not divisible: " << p << endl; }
      ret.push_back(p);
      continue;
    }

    if (gm == K_MULTIPLE and (ret.size() + pcs.size() + qs.size() > K)) {
      if (DEBUG) {
        cerr << "#pattern is over K=" << K << endl;
        trace(ret.size()); trace(pcs.size()); trace(qs.size());
      }
      ret.push_back(p);
      continue;
    }

    if (gm == GAIN_LIMIT)
    {
      int g0 = language_size(p);
      int g1 = 0;
      rep (i, qs.size()) {
        g1 = max(g1, language_size(qs[i].first));
      }
      if (DEBUG) {
        cerr << "# Language Size : " << g0 << " -> " << g1 << endl;
      }
      if (g1 + limit < g0) {
        ret.push_back(p);
        continue;
      }
    }

    for (auto&pc_next: qs) {
      for (int i: pc_next.second) ++cover_count[i];
    }

    for (auto&pc_next: qs) {
      vi S; // Doc - (Pi - pc_next)
      for (int i: pc_next.second) {
        if (cover_count[i] == 1) S.push_back(i);
        else --cover_count[i];
      }
      if (S.size() == 0) continue;
      pc_next = { tighten(pc_next.first, S), S };
      pcs.push({ S.size(), pc_next });
    }

  }

  return ret;
}
