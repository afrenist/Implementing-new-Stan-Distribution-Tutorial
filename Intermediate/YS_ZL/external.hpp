#include <cmath>
#include <iostream>
#include <stan/math.hpp>

namespace ys_example_model_namespace {

/** \ingroup prob_dists
 * Returns the lpmf of the Yule-Simon distribution given outcome and shape
 * parameter.
 *
 * @tparam propto Boolean parameter indicating whether to include the
 * normalizing constant.
 * @tparam T_y Type of the observed outcome (integer).
 * @tparam T_a Type of the shape parameter (scalar).
 *
 * @param y Observed outcome (an integer greater than or equal to 1).
 * @param a Shape parameter (a positive real number).
 * @return Log probability mass function value.
 * 
 * @author Zhi Ling, Stan Development Team
 */
template <bool propto, typename T_y, typename T_a>
stan::return_type_t<T_a> yule_simon_lpmf(const T_y &y, const T_a &a,
                                         std::ostream *pstream__) {
  using stan::math::lbeta;
  using stan::math::log;
  auto lpmf = lbeta(y, a + 1.0);
  if constexpr (stan::math::include_summand<propto>::value) {
    lpmf += log(a);
  }
  return lpmf;
}

}