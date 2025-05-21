setwd('/Users/or105/git/Implementing-new-Stan-Distribution-Tutorial/Basic_stan')
##############################
# Chunk code:  libraries
#############################

library(cmdstanr)
library(posterior)
library(ggplot2)
library(gridExtra)
library(bayesplot)
theme_set(bayesplot::theme_default(base_family = "sans"))
set.seed(42)


##############################
# Chunk code: setting the cmdstan path
#############################

# be sure you know what cmdstan version we are working with/on
# use to find the path: cmdstan_path()
set_cmdstan_path("/Users/or105/.cmdstan/cmdstan-2.36.0")


##############################
# Chunk code:  # Write Stan model to file
#############################

#writeLines(readLines("ys.stan"), "ys.stan")

##############################
# Chunk code: Simulated Yule-Simon data (for example)
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

hist(y_data)

##############################
# Chunk code:  Prepare data for Stan
#############################

stan_data <- list(
  N = length(y_data),
  y = y_data
)

##############################
# Chunk code: compile the model 
#############################

mod <- cmdstan_model("ys.stan")

mod
##############################
# Chunk code: fit the model
#############################

fit <- mod$sample(
  data = stan_data,
  seed = 123,
  chains = 4,
  parallel_chains = 4,
  iter_sampling = 1000,
  iter_warmup = 500
)

##############################
# Chunk code: results
#############################

print(fit$summary())
mcmc_hist(fit$draws("rho"))


##############################
# Chunk code:  different param
#############################
mod2 <- cmdstan_model("ys2.stan")
mod2 
##############################
# Chunk code:   model fit 
#############################
fit2 <- mod2$sample(
  data = stan_data,
  seed = 123,
  chains = 4,
  parallel_chains = 4,
  iter_sampling = 1000,
  iter_warmup = 500
)


print(fit2$summary())
mcmc_hist(fit2$draws("a"))

##############################
# Chunk code:  a more efficient coding 
#############################
mod3 <- cmdstan_model("ys3.stan")

mod3


fit3 <- mod3$sample(
  data = stan_data,
  seed = 123,
  chains = 4,
  parallel_chains = 4,
  iter_sampling = 1000,
  iter_warmup = 500
)


print(fit3$summary())
mcmc_hist(fit3$draws("a"))


##############################
# Chunk code:   Only rng
#############################


mod4 <- cmdstan_model("ys4.stan")

mod4


fit4 <- mod4$sample(
  data = stan_data,
  seed = 123,
  chains = 4,
  parallel_chains = 4,
  iter_sampling = 1000,
  iter_warmup = 500
)

print(fit4$summary())

ans1=fit4$summary()
par(mfrow=c(1,2))
hist(stan_data$y)
hist(ans1$mean[2:101])
