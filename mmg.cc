#include <bits/stdc++.h>
#include "./mmg.h"

using namespace std;

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
int var_count(vector<RP>&s) {
  const int n = s.size();
  int m = 0;
  rep (i, n) if (s[i].is_var) ++m;
  return m;
}

inline
double var_ratio(vector<RP>&s) {
  const int n = s.size();
  int m = 0;
  rep (i, n) if (s[i].is_var) ++m;
  return double(m) / double(n);
}

inline
bool is_good(vector<RP>&s) {
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
bool preceq(vector<string>&sentence, vector<RP>&sigma) {
  const int n = sentence.size();
  const int m = sigma.size();
  if (m == 0) return false;
  int i = 0,
      j = 0;
  bool erasing = false;
  for (; i < n and j < m;) {
    if (sigma[j].is_var) { ++i; ++j; erasing = true; }
    else if (erasing
        and sentence[i] == sigma[j].str
        and i < n-1 and j < m-1
        and (not sigma[j+1].is_var)
        and sentence[i+1] != sigma[j+1].str) {
      ++i; }
    else if (sentence[i] == sigma[j].str) { ++i; ++j; erasing = false; }
    else if (erasing) { ++i; }
    else { return false; }
  }
  while (i < n and erasing) ++i;
  return i == n and j == m;
}

bool language_include(vector<RP>&p, vector<vector<string>>&S) {
  for (auto&s: S)
    if (not preceq(s, p)) return false;
  return true;
}

vector<vector<string>> doc;

bool language_include(vector<RP>&p, vector<int> c) {
  for (int i: c)
    if (not preceq(doc[i], p)) return false;
  return true;
}

vector<int> partial_covering(vector<RP> p, vector<int> c) {
  vector<int> r;
  for (int i: c)
    if (preceq(doc[i], p)) r.push_back(i);
  return r;
}

set<string> collect(vector<RP> p, vector<int> c) {
  const int n = p.size();
  set<string> s;
  for (int idx: c) {
    vector<string>&words = doc[idx];
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
vector<RP> tighten(vector<RP>&p, vector<int>&c)
{
  const int n = p.size();

  rep (i, n) {
    if (p[i].is_var) {
      vector<RP> r;
      for (int j = 0; j < i; ++j) r.push_back(p[i]);
      r.push_back(RP());
      for (int j = i; j < n; ++j) r.push_back(p[i]);
      if (language_include(r, c)) {
        return tighten(r, c);
      }
    }
  }
  set<string> alphabets = collect(p, c);
  for (auto&s: alphabets) {
    for (int i = 0; i < n; ++i) {
      if (not p[i].is_var) continue;
      p[i].is_var = false;
      p[i].str = s;
      if (language_include(p, c)) {
        return tighten(p, c);
      }
      p[i].is_var = true;
    }
  }
  return p;
}

vector<pair<vector<RP>, vector<int>>>
division(vector<RP> p, vector<int> c)
{
  const int n = p.size();
  const int M = c.size();
  set<string> alphabets = collect(p, c);

  vector<pair<vector<RP>, vector<int>>> cspc;

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
      vector<RP> q;

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
      [](pair<vector<RP>, vector<int>> x,
         pair<vector<RP>, vector<int>> y) {
          return x.second.size() > y.second.size(); });

  vector<pair<vector<RP>, vector<int>>> ret;
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
vector<RP> var_simplify(vector<RP>&s) {
  vector<RP> r;
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

vector<vector<RP>> kmmg(vector<int>&ids)
{
  const int n = ids.size();

  vector<RP> top { RP() };
  auto pc = tighten(top, ids);

  // いくつのパターンに被覆されているか
  vector<int> cover_count(n, 1);

  // collection of (pattern::vector<RP>, covering::vector<int>)
  queue<pair<vector<RP>, vector<int>>> pcs;
  pcs.push(make_pair(pc, ids));
  vector<vector<RP>> ret;

  /* division */
  while (not pcs.empty()) {
    auto pc = pcs.front(); pcs.pop();
    if (gm != K_MULTIPLE) {
      auto p = var_simplify(pc.first);
      if (is_good(p)) ret.push_back(p);
    }

    // S = Doc \ L( Pi \ p)
    vector<int> S;
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
      vector<int> S;
      for (int i: pc_next.second) {
        if (cover_count[i] == 1) S.push_back(i);
        else --cover_count[i];
      }
      pc_next = make_pair( tighten(pc_next.first, S), S);
      pcs.push(pc_next);
    }
  }

  return ret;
}

double str2double(string s) {
  stringstream a(s);
  double r; a >> r;
  return r;
}

int str2int(string s) {
  stringstream a(s);
  int r; a >> r;
  return r;
}

int main(int argc, char*argv[])
{
  for (int i = 1; i < argc; ++i) {
    string s = string(argv[i]);
    if (s == "-c") {
      gm = VAR_COUNT;
    } else if (s == "-r") {
      gm = VAR_RATIO;
    } else if (s == "-k") {
      gm = K_MULTIPLE;
      ++i;
      K = str2int(string(argv[i]));
    } else if (s == "-sub") {
      ++i;
      if (gm == VAR_COUNT) {
        c_sub = str2int(string(argv[i]));
      } else {
        rho_sub = str2double(string(argv[i]));
      }
    } else if (s == "-sup") {
      ++i;
      if (gm == VAR_COUNT) {
        c_sup = str2int(string(argv[i]));
      } else {
        rho_sup = str2double(string(argv[i]));
      }
    } else {
      cerr << "[error] unknown option: " << s << endl;
      return 1;
    }
  }
  { // parameter check
    switch (gm) {
    case VAR_RATIO:
      if (rho_sub >= rho_sup) {
        cerr << "[error] rho_minus < rho_plus" << endl;
        return 1;
      }
      break;
    case VAR_COUNT:
      if (c_sub >= c_sup) {
        cerr << "[error] rho_minus < rho_plus" << endl;
        return 1;
      }
      break;
    case K_MULTIPLE:
      if (K < 1) {
        cerr << "[error] K must be int larger then 0" << endl;
        return 1;
      }
      break;
    default:
      cerr << "[error] program error" << endl;
      return 1;
    }
  }
  { // debug print
    cerr << "# parameteres" << endl;
    switch (gm) {
    case VAR_COUNT:
      cerr << "is_good by the var count" << endl;
      cerr << "[sub, sup) = " << c_sub << ", " << c_sup << endl;
      break;
    case VAR_RATIO:
      cerr << "is_good by the var ratio" << endl;
      cerr << "[sub, sup) = " << rho_sub << ", " << rho_sup << endl;
      break;
    case K_MULTIPLE:
      cerr << K << "-multiple" << endl;
    }
  }
  loop {
    string s;
    getline(cin, s);
    if (not cin) break;
    if (s == "") continue;
    doc.push_back(as_words(s));
  }
  const int n = doc.size();
  vector<int> ids(n);
  rep (i, n) ids[i] = i;

  // 行くぜ
  set<vector<RP>> patterns;
  {
    auto ps = kmmg(ids);
    for (auto&p: ps) patterns.insert(p);
  }
  for (auto&p: patterns) cout << p << endl;

  return 0;
}
