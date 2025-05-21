functions{
  real yule_simon_lpmf(array[] int y, real a) {
    int N = size(y);
    vector[N] lprobs;
    for (i in 1:N) {
      lprobs[i] = log(a) + lgamma(y[i])+ lgamma(a+1)
      - lgamma(y[i]+1+a);
    }
    return  sum(lprobs);
  }
  // real yule_simon_rng(real a) {
    //   real u = uniform_rng(0, 1);
    //   real v = beta_rng(a,1);
    //   real y = 1+floor(log(u)/log(v));
    //   return y;
    // }
    
    real yule_simon_rng(real a) {
      real p = uniform_rng(0,1);
      real v = exponential_rng(a);
      real y = 1 + floor(log(1 - p) / log(1 - exp(-v)));
      return y;
    }
}


data {
  int <lower=1> N;
  array[N] int <lower=1> y;
}

parameters {
  real<lower=0> a;
}

model{
  a ~  uniform(1,5);
  y ~ yule_simon(a);
}

generated quantities {
  array[N] real y_sim;
  // use current estimate of theta to generate new sample
  for (n in 1:N) {
    y_sim[n] = yule_simon_rng(a);
  }
}


