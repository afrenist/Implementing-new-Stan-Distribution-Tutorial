functions {
  // Yule-Simon distribution signatures
  real yule_simon_lpmf(int n, real alpha);
  real yule_simon_lpmf(int n, vector alpha);
  real yule_simon_lpmf(array[] int n, real alpha);
  real yule_simon_lpmf(array[] int n, vector alpha);
  int yule_simon_rng(real alpha);
  array[] int yule_simon_rng(vector alpha);
}

data {
  int <lower=1> N;
  array[N] int<lower=1> y;
}

transformed data {
  int n = 1000;
}

parameters {
  real<lower=0> a;
}

model{
  a ~ exponential(2);
  target += yule_simon_lpmf(y | a);
}

generated quantities {
  real pmf_check = exp(yule_simon_lpmf( { 1, 2 } | [ 1.1, 1.2 ]' ));
  array[n] int randomYS = yule_simon_rng(rep_vector(1.1, n));
}




