#include "assembly_planner/math_functions.h"

namespace math_functions{

long long factorial(long long n)
{
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

}
