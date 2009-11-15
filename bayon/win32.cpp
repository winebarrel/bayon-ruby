#include "win32.h"

namespace std {
  int _isnan(double x) {
    return ::_isnan(x);
  }
}
