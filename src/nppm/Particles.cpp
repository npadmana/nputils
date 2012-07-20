#include "Particles.h"

double periodic(double x, double L) {
  double tmp;

  tmp = x/L;
  if (tmp>=1.0) tmp = tmp-floor(tmp);
  if (tmp< 0.0) tmp = 1.0 -(-tmp-floor(-tmp));

  return tmp*L;
}
