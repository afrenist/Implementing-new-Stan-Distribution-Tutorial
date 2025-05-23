#ifndef STAN_MATH_PRIM_PROB_YULE_SIMON_LPMF_HPP
#define STAN_MATH_PRIM_PROB_YULE_SIMON_LPMF_HPP

#include <iostream>
#include <stan/math.hpp>
#include <stan/math/prim/err.hpp>
#include <stan/math/prim/fun/constants.hpp>
#include <stan/math/prim/fun/digamma.hpp>
#include <stan/math/prim/fun/lbeta.hpp>
#include <stan/math/prim/fun/lgamma.hpp>
#include <stan/math/prim/fun/max_size.hpp>
#include <stan/math/prim/fun/scalar_seq_view.hpp>
#include <stan/math/prim/fun/size.hpp>
#include <stan/math/prim/fun/size_zero.hpp>
#include <stan/math/prim/fun/value_of.hpp>
#include <stan/math/prim/functor/partials_propagator.hpp>
#include <stan/math/prim/meta.hpp>

namespace ys_example_model_namespace {

/** \ingroup prob_dists
 * Returns the log PMF of the Yule-Simon distribution with shape parameter.
 * Given containers of matching sizes, returns the log sum of probabilities.
 *
 * @tparam T_n type of failure parameter
 * @tparam T_alpha type of prior success parameter
 *
 * @param n failure parameter
 * @param alpha prior success parameter
 * @return log probability or log sum of probabilities
 * @throw std::domain_error if alpha fails to be positive
 * @throw std::invalid_argument if container sizes mismatch
 *
 * @author Zhi Ling, Stan Development Team
 */
template <bool propto, typename T_n, typename T_alpha,
          stan::require_all_not_nonscalar_prim_or_rev_kernel_expression_t<
              T_n, T_alpha> * = nullptr>
inline stan::return_type_t<T_alpha>
yule_simon_lpmf(const T_n &n, const T_alpha &alpha, std::ostream *pstream__) {

  using stan::is_constant;
  using stan::scalar_seq_view;
  using stan::VectorBuilder;
  using namespace stan::math;
  using T_partials_return = stan::partials_return_t<T_n, T_alpha>;
  using T_n_ref = stan::ref_type_t<T_n>;
  using T_alpha_ref = stan::ref_type_t<T_alpha>;
  static constexpr const char *function = "yule_simon_lpmf";
  check_consistent_sizes(function, "Outcome variable", n, "Shape parameter",
                         alpha);
  if (size_zero(n, alpha)) {
    return 0.0;
  }

  T_n_ref n_ref = n;
  T_alpha_ref alpha_ref = alpha;
  check_nonnegative(function, "Outcome variable", n_ref);
  check_positive_finite(function, "Shape parameter", alpha_ref);

  if constexpr (!include_summand<propto, T_alpha>::value) {
    return 0.0;
  }

  auto ops_partials = make_partials_propagator(alpha_ref);

  scalar_seq_view<T_n> n_vec(n);
  scalar_seq_view<T_alpha_ref> alpha_vec(alpha_ref);
  const size_t size_n = stan::math::size(n);
  const size_t size_alpha = stan::math::size(alpha);// delete 
  const size_t max_size_seq_view = max_size(n, alpha);
  T_partials_return logp(0.0);

  // log pmf
  // compute log(alpha)
  VectorBuilder<include_summand<propto>::value, T_partials_return, T_alpha>
      normalizing_constant(size_alpha);
  if constexpr (include_summand<propto>::value)
    for (size_t i = 0; i < size_alpha; i++)
      normalizing_constant[i] = stan::math::log(alpha_vec.val(i));

  // compute lbeta term
  VectorBuilder<true, T_partials_return, T_n, T_alpha> lbeta_term(
      max_size_seq_view);
  for (size_t i = 0; i < max_size_seq_view; i++) {
    lbeta_term[i] = lbeta(n_vec.val(i), alpha_vec.val(i) + 1.0);
  }

  // derivatives, on the basis of needs
  // compute digamma(alpha+1) - digamma(n+alpha+1)
  VectorBuilder<!is_constant<T_alpha>::value, T_partials_return, T_n, T_alpha>
      digamma_alpha(max_size_seq_view);
  if constexpr (!is_constant<T_alpha>::value) {
    for (size_t i = 0; i < max_size_seq_view; i++) {
      digamma_alpha[i] = digamma(alpha_vec.val(i) + 1.0) -
                         digamma(n_vec.val(i) + alpha_vec.val(i) + 1.0);
    }
  }

  for (size_t i = 0; i < max_size_seq_view; i++) {
    if constexpr (include_summand<propto>::value)
      logp += normalizing_constant[i];
    logp += lbeta_term[i];

    if constexpr (!is_constant<T_alpha>::value)
      partials<0>(ops_partials)[i] += 1 / alpha_vec.val(i) + digamma_alpha[i];
  }
  return ops_partials.build(logp);
}

template <typename T_n, typename T_alpha>
inline stan::return_type_t<T_alpha> yule_simon_lpmf(const T_n &n,
                                                    const T_alpha &alpha) {
  return yule_simon_lpmf<false>(n, alpha);
}

} // namespace ys_example_model_namespace
#endif
