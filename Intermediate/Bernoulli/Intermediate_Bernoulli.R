setwd("/Users/afrenisterrantino/Library/CloudStorage/OneDrive-TheAlanTuringInstitute/ImperialCollege/STAN/YS_Example/Intermediate/Bernoulli/")
##############################
# Chunk code: set library
#############################
library(cmdstanr)
set_cmdstan_path("/Users/afrenisterrantino/.cmdstan/cmdstan-2.36.0")

##############################
# Chunk code:  model 
#############################


mod <- cmdstan_model('bernoulli_example.stan',
    include_paths=getwd(),
    cpp_options=list(USER_HEADER='external.hpp'),
    stanc_options = list("allow-undefined")
)

##############################
# Chunk code:  model 
#############################


fit <- mod$sample(data=list(N = 10, y = c(0,1,0,0,0,0,1,1,0,1)))
fit$draws('odds')

##############################
# Chunk code:  alternative
############################

# mod1 <-cmdstan_model(write_stan_file("functions {
#   real make_odds(data real theta);
# }
# 
# data {
#   int<lower=0> N;
#   array[N] int<lower=0, upper=1> y;
# }
# parameters {
#   real<lower=0, upper=1> theta;
# }
# model {
#   theta ~ beta(1, 1); // uniform prior on interval 0, 1
#   y ~ bernoulli(theta);
# }
# generated quantities {
#   real odds;
#   odds = make_odds(theta);
# }")



##############################
# Chunk code:  rm 
#############################
rm(list=ls())

##############################
# Chunk code:  With rstan
#############################
library(rstan)
library(ggplot2)
library(gridExtra)
library(bayesplot)
##############################
# Chunk code:  rstan 
#############################
model <- stan_model('bernoulli_example.stan',
    allow_undefined = TRUE,
    includes = paste0('\n#include "', file.path(getwd(), 'external_rstan.hpp'), '"\n'),
)

fit <- sampling(model, data = list(N = 10, y = c(0,1,0,0,0,0,0,0,0,1)))
extract(fit)$odds

monitor(fit)

draws <- as.data.frame(fit)
mcmc_hist(draws, pars='theta', bins=30)
mcmc_hist(draws, pars = 'odds',bins=50)




