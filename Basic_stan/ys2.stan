functions{
  real yule_simon_lpmf(int y, real a) {
real lprobs = log(a) + lgamma(y)+ lgamma(a+1) - lgamma(y+1+a);
return lprobs;
}
  }
  
 
data {
int <lower=1> N;
array[N] int y;
}

parameters {
real<lower=0> a;
}

model{
// a ~ exponential(2);
 a~ gamma(0.001, 0.001); 
 for (i in 1:N) {
target += yule_simon_lpmf(y[i]| a);

}
}






 
