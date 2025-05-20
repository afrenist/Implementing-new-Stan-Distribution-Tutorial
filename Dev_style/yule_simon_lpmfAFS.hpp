namespace <THE_NAME_OF_STAN_MODEL>_model_namespace {
    ......
}


//Declaration of the function.

template <bool propto, typename T_n,  typename T_size1,
          stan::require_all_not_nonscalar_prim_or_rev_kernel_expression_t<
              T_n,  T_size1>* = nullptr>
stan::return_type_t<T_size1> yule_simon_lpmf(const T_n& n,
                                                    const T_size1& alpha,
                                                    std::ostream* pstream__) {
    ......
}
// Specify aliases.

using stan::partials_return_t;
  using stan::ref_type_t;
  using stan::ref_type_if_t;
  using stan::is_constant;
  using stan::is_constant_all;
  using stan::VectorBuilder;
  using stan::scalar_seq_view;
  using stan::math::lgamma;
  using stan::math::size;
  using stan::math::max_size;
  using T_partials_return = partials_return_t< T_size1>;
  using T_alpha_ref = ref_type_t<T_size1>;
//Check whether the shape of the incoming data conforms to the specification. It throws a std::invalid_argument if the sizes of the input containers don't match.

static const char* function = "yule_simon_lpmf";
  check_consistent_sizes(function, "Frequency  variable", n,
                         "Shape parameter", alpha);
  if (size_zero(n, alpha)) {
    return 0.0;
  }
//Check whether the value of the incoming parameter vectors are within the parameter spaces. throws a std::domain_error if any of the parameters are not positive and finite.

  T_alpha_ref alpha_ref = alpha;
  check_positive_finite(function, "Prior Shape parameter", alpha_ref);

//If propto = TRUE and all other parameters are not autodiff types, return zero.

if (!include_summand<propto, T_size1>::value) {
    return 0.0;
  }
//Initialization return value, as well as some quantities that will be reused in subsequent calculations.

T_partials_return logp(0.0);
  operands_and_partials< T_alpha_ref> ops_partials( alpha_ref);

  scalar_seq_view<T_n> n_vec(n);
  scalar_seq_view<T_alpha_ref> alpha_vec(alpha_ref);
  size_t size_n = stan::math::size(n);
  size_t size_alpha = stan::math::size(alpha);
// Determines whether support for incoming observations is valid.

for (size_t i = 0; i < max_size_seq_view; i++) {
    if (n_vec[i] < 0) {
      return ops_partials.build(LOG_ZERO);
    }
  }
//Compute the log pmf

// compute gamma(n+1)
  VectorBuilder<include_summand<propto>::value, T_partials_return, T_n>
      normalizing_constant(size_n);
  for (size_t i = 0; i < size_n; i++)
    if (include_summand<propto>::value)
      normalizing_constant[i] = lgamma(n_vec[i]);  //  log gamma (y)

  // compute lgamma numerator  with size alpha
  VectorBuilder<true, T_partials_return, T_size1> loga_denominator(size_alpha);
  for (size_t i = 0; i < size_beta; i++) {
    loga_denominator[i] = log(alpha_vec.val(i)); // log(alpha)
  }

  // compute lgamma numerator with size alpha and 1 
  VectorBuilder<true, T_partials_return, T_n, T_size2> lgamma_numerator(size_alpha);
  for (size_t i = 0; i < size_n_beta; i++) {
    lgamma_numerator[i] = lgamma(1 + alpha_vec.val(i));  // log gamma (alpha+1)
  }


  // compute lgamma numerator with size n and alpha
  VectorBuilder<true, T_partials_return, T_n, T_size2> lgamma_denumerator(size_n_alpha);
  for (size_t i = 0; i < size_n_beta; i++) {
    lgamma_denumerator[i] =  - lgamma(n_vec[i] + alpha_vec.val(i)+1); // log gamma (alpha+1+y) 
  }


  // compute lbeta numerator with size n,  alpha
  VectorBuilder<true, T_partials_return, T_n, T_r, T_size1, T_size2> lbeta_diff(max_size_seq_view);
  for (size_t i = 0; i < max_size_seq_view; i++) {
    lfoo_diff[i] =    loga_denominator[i]+ lgamma_numerator[i]
                     - lgamma_denominator[i];
  }
//Compute derivative with respect to  α
 
// compute digamma(n+alpha+1)
  VectorBuilder<!is_constant_all<T_size1>::value, T_partials_return,
                T_n,  T_size1>
      digamma_n_alpha(max_size_seq_view);
  if (!is_constant_all<T_r, T_size1, T_size2>::value) {
    for (size_t i = 0; i < max_size_seq_view; i++) {
      digamma_n_alpha[i]
          = digamma(n_vec[i] + alpha_vec.val(i));
    }
  }

  // compute digamma(alpha+1)
  VectorBuilder<!is_constant_all<T_size1>::value, T_partials_return,
                T_size1>
      digamma_alpha(size_alpha);
  if (!is_constant_all<T_size1, T_size2>::value) {
    for (size_t i = 0; i < size_alpha_beta; i++) {
      digamma_alpha[i] = digamma(alpha_vec.val(i) + 1);
    }
  }


  // compute log(alpha)
  VectorBuilder<!is_constant_all<T_size1>::value, T_partials_return, T_size1> log_alpha(size_alpha);
  if (!is_constant_all<T_size1>::value) {
    for (size_t i = 0; i < size_alpha; i++) {
    log_alpha[i] = 1/(alpha_vec.val(i));
    }
  }


// Build the return value.

for (size_t i = 0; i < max_size_seq_view; i++) {
    if (include_summand<propto>::value)
      logp += normalizing_constant[i];
    logp += lfoo_diff[i];
    
    if (!is_constant_all<T_size1>::value)
      ops_partials.edge2_.partials_[i]
	+= log_alpha[i] +  digamma_alpha[i]  - digamma__n_alpha[i]);
						    
  }
  return ops_partials.build(logp);
//For pmf/pdf functions, we have to overload the template function defined above. This version of the function template does not include the propto parameter (default to false). It provides a simpler interface, which is used for direct function calls.

template <typename T_n,  typename T_size1>
  inline stan::return_type_t<T_size1> yule_simon_lpmf(const T_n& n, 
                                                   const T_size1& alpha,
                                                   std::ostream* pstream__) {
  return yule_simon_lpmf<false>(n,  alpha);
}
