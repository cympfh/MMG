#include "./mmg.cc"

void usage() {
  fprintf(stderr, "Usage: ./mmg.exe [-D] [-K <int>] [-l <double>]\n");
  fprintf(stderr, "-K <k>                run k-mmg (assert k > 0)\n");
  fprintf(stderr, "-l <limit>            limit gain\n");
  fprintf(stderr, "-D                    DEBUG mode\n");
  exit(0);
}

int main(int argc, char*argv[]) {

  // parse args
  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];
    if (arg == "-k" or arg == "-K") {
      gm = K_MULTIPLE;
      ++i;
      K = str2int(string(argv[i]));
    }
    else if (arg == "-l") {
      gm = GAIN_LIMIT;
      ++i;
      limit = str2double(string(argv[i]));
    }
    else if (arg == "-D") {
      DEBUG = true;
    }
    else {
      usage();
    }
  }

  { // parameter assertion
    switch (gm) {
    case K_MULTIPLE:
      if (K < 1) usage();
      break;
    case GAIN_LIMIT:
      if (limit <= 0.0) usage();
      break;
    default:
      usage();
    }
  }

  if (DEBUG) { // debug print
    switch (gm) {
    case K_MULTIPLE:
      fprintf(stderr, "  %d-multiple\n", K);
      break;
    case GAIN_LIMIT:
      fprintf(stderr, "  limit with %f\n", limit);
      break;
    }
  }

  // read documents
  vector<vector<string>> docs;
  {
    string s;
    while (getline(cin, s)) {
      if (s == "") continue;
      docs.push_back(as_words(s));
    }
  }

  // 行くぜ
  auto result = kmmg(docs);
  for (const Pattern& p: result) {
    cout << p << endl;
  }

  return 0;
}
