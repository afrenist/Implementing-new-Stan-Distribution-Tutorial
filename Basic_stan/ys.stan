functions 
{
  real yule_simon_lpmf(int k, real rho) 
  {
    return log(rho) + lbeta(rho + 1, k);
  }
}

data 
{
  int<lower=1> N;
  array[N] int<lower=1> y;
// int<lower=1> y[N];     // Observed data
}

parameters 
{
  real<lower=0> rho;     // Yule-Simon parameter
}

model 
{
  rho ~ gamma(0.25, 0.05);  // Prior
  for (n in 1:N)
  {
    y[n] ~ yule_simon(rho);
  }
}

