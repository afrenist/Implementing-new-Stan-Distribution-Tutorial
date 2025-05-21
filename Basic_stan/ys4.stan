functions
{
  real yule_simon_rng(real a) 
  {
    real p = uniform_rng(0,1);
    real v = exponential_rng(a);
    real y = 1 + floor(log(1 - p) / log(1 - exp(-v)));
    return y;
  }
}

data 
{
  int<lower=0> N; 
}

generated quantities 
{
  array[N] real y_sim;
  for (n in 1:N) 
  {
    y_sim[n] = yule_simon_rng(2);
  }
}
