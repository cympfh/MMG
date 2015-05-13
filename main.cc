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
  // parse args
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
