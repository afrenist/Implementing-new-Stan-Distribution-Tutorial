functions {
  real yule_simon_lpmf(int y, real a);
}

data {
int <lower=1> N;
array[N] int y;
}

parameters {
real<lower=0> a;
}

model{
  a ~ exponential(2);
  for (i in 1:N) {
    target += yule_simon_lpmf(y[i] | a);
    // target += yule_simon_lupmf(y[i] | a); // try this and see what's the difference of the posterior distribution of a!
  }
}

generated quantities {
  real pmf_check = exp(yule_simon_lpmf(1 | 1.1));
}


