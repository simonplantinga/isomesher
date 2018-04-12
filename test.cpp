#include <boost/numeric/interval.hpp>

int sign_polynomial(double x, double P[], int sz) {
  using namespace boost::numeric;
  using namespace interval_lib;

  typedef interval<double> I;

  I y = P[sz - 1];
  for (int i = sz - 2; i >= 0; i--)
    y = y * x + P[i];

  using namespace compare::certain;
  if ( y > 0. ) return 1;
  if ( y < 0. ) return -1;
  return 0;
}

