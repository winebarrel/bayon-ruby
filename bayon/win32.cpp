#include "win32.h"

namespace std {
  int isnan(double x) {
    return _isnan(x);
  }
}

namespace local {
  double _sqrt(double x) {
    return ::sqrt(x);
  }
}
