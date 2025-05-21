#setwd("/Users/afrenisterrantino/Library/CloudStorage/OneDrive-TheAlanTuringInstitute/ImperialCollege/STAN/YS_Example/Intermediate/YS_ZL")

##############################
# Chunk code: set library
#############################
library(cmdstanr)
library(posterior)
library(ggplot2)
library(gridExtra)

#set_cmdstan_path("./cmdstan/cmdstan-2.36.0")


##############################
# Chunk code:  simulate data
#############################
simulate_yule_simon <- function(n, rho) {
  r <- numeric(n)
  for (i in 1:n) {
    p <- runif(1)
    r[i] <- 1 + floor(log(1 - p) / log(1 - exp(-rexp(1, rate = rho))))
  }
  return(r)
}

 
##############################
# Chunk code: Simulated Yule-Simon
#############################


y_data <- simulate_yule_simon(n = 100, rho = 2.0)

##############################
# Chunk code:  Prepare data for Stan
#############################

stan_data <- list(
  N = length(y_data),
  y = y_data
)
##############################
# Chunk code:  model 
#############################


mod <- cmdstan_model('ys_example.stan',
                       include_paths=getwd(),
                       cpp_options=list(USER_HEADER='external.hpp'),
                       stanc_options = list("allow-undefined")
)

#############################
# Chunk code:  model fit
#############################


fit <- mod$sample(data=stan_data)


fit$draws('')
print(fit$summary())
mcmc_hist(fit$draws("a")) 
