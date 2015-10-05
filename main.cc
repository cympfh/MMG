#include "./mmg.cc"

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

  goto begin;

help:
      cerr << "Usage: ./mmg.exe [ -K <int> ] [ -c -sub <int> -sup <int> ] [ -r -sub <double> -sup <double> ]" << endl;
      cerr << "-K k                  run k-mmg (assert k > 0)" << endl;
      cerr << "-c -sub c0 -sup c1    outputs all patterns when containing n var and (c0 <= n < c1) (assert c0 < c1)" << endl;
      cerr << "-r -sub r0 -sup r1    outputs all patterns when its var-ratio is r and (r0 <= r < r1) (assert r0 < r1)" << endl;
      return 0;

begin:

  // parse args
  for (int i = 1; i < argc; ++i) {
    string arg = string(argv[i]);
    if (arg == "-c") {
      gm = VAR_COUNT;
    } else if (arg == "-r") {
      gm = VAR_RATIO;
    } else if (arg == "-k" or arg == "-K") {
      gm = K_MULTIPLE;
      ++i;
      K = str2int(string(argv[i]));
    } else if (arg == "-sub") {
      ++i;
      if (gm == VAR_COUNT) {
        c_sub = str2int(string(argv[i]));
      } else {
        rho_sub = str2double(string(argv[i]));
      }
    } else if (arg == "-sup") {
      ++i;
      if (gm == VAR_COUNT) {
        c_sup = str2int(string(argv[i]));
      } else {
        rho_sup = str2double(string(argv[i]));
      }
    } else {
      goto help;
    }
  }

  { // parameter assertion
    switch (gm) {
    case VAR_RATIO:
      if (rho_sub >= rho_sup) { goto help; }
      break;
    case VAR_COUNT:
      if (c_sub >= c_sup) { goto help; }
      break;
    case K_MULTIPLE:
      if (K < 1) { goto help; }
      break;
    default:
      goto help;
    }
  }

  { // debug print
    cerr << "# parameteres" << endl;
    switch (gm) {
    case VAR_COUNT:
      cerr << "  is_good by the var count" << endl;
      cerr << "  [sub, sup) = " << c_sub << ", " << c_sup << endl;
      break;
    case VAR_RATIO:
      cerr << "  is_good by the var ratio" << endl;
      cerr << "  [sub, sup) = " << rho_sub << ", " << rho_sup << endl;
      break;
    case K_MULTIPLE:
      cerr << "  " << K << "-multiple" << endl;
    }
  }

  // read documents
  vector<vector<string>> docs;
  loop {
    string s;
    getline(cin, s);
    if (not cin) break;
    if (s == "") continue;
    docs.push_back(as_words(s));
  }

  // 行くぜ
  auto result = kmmg(docs);

  // print out in order
  set<P> ps;
  for (auto&p: result) ps.insert(p);
  for (auto&p: ps) cout << p << endl;

  return 0;
}
