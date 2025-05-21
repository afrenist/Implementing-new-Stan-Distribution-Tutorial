#ifndef STAN_MATH_PRIM_PROB_YULE_SIMON_RNG_HPP
#define STAN_MATH_PRIM_PROB_YULE_SIMON_RNG_HPP

#include <iostream>
#include <stan/math.hpp>
#include <stan/math/prim/meta.hpp>
#include <stan/math/prim/prob/exponential_rng.hpp>
#include <stan/math/prim/prob/neg_binomial_rng.hpp>

namespace ys_example_model_namespace {

/** \ingroup prob_dists
 * Return a yule-simon random variate with the given shape parameter,
 * using the given random number generator.
 *
 * alpha can be a scalar or a one-dimensional container. Any
 * non-scalar inputs must be the same size.
 *
 * @tparam T_alpha type of prior success parameter
 * @tparam RNG type of random number generator
 *
 * @param alpha (Sequence of) prior success parameter(s)
 * @param rng random number generator
 * @return (Sequence of) yule-simon random variate(s)
 * @throw std::domain_error if alpha is nonpositive
 *
 * @author Zhi Ling, Stan Development Team
 */
template <typename T_alpha, typename RNG>
inline auto yule_simon_rng(const T_alpha &alpha, RNG &rng,
                           std::ostream *pstream__) {

  using stan::scalar_seq_view;
  using stan::VectorBuilder;
  using namespace stan::math;
  using T_alpha_ref = stan::ref_type_t<T_alpha>;
  static constexpr const char *function = "yule_simon_rng";

  T_alpha_ref alpha_ref = alpha;
  check_positive_finite(function, "Shape parameter", alpha_ref);

  using T_w = decltype(exponential_rng(alpha_ref, rng));
  T_w w = exponential_rng(alpha_ref, rng);

  scalar_seq_view<T_w> w_vec(w);
  size_t size_w = stan::math::size(w);
  VectorBuilder<true, double, T_w> odds_ratio_p(size_w);
  for (size_t n = 0; n < size_w; ++n) {
    auto p = stan::math::exp(-w_vec.val(n));
    odds_ratio_p[n] = stan::math::exp(stan::math::log(p) - log1m(p));
  }

  return neg_binomial_rng(1.0, odds_ratio_p.data(), rng);
}

} // namespace ys_example_model_namespace
#endif

/** Note:
 * I finally choose this exponential + geometric way to implement, as it is more
 * straightforward. The inverse CDF method requires a while loop, which is not
 * very efficient.
 *
 * In this implementation, support of yule_simon starts from 0 (not 1).
 * If one need to start from 1, just add 1 to the result.
 */
