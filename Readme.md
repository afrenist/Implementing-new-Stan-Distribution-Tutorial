
# Description


This repository contains codes and scripts associated to a tutorial on how to 
implement a new distribution in STAN.
It contains three apporaches from the simplest to the most advanced: (i) coding 
the new distribution in STAN;(ii) as an
external C++ file and then load it into the model compilations and  (iii) Developer style
on how to write the C++ files for full implementation in STAN ( including _lpmf, lcdf, lccdf, rng).



##  Repository Structure
```
Basic_stan/
├── ys.R  # R code to run the stan models and data generated
├── ys.stan #  log probabilty mass function for Yule-Simon Distribution
├── ys2.stan # alternative parametrization  for Yule-Simon Distribution
├── ys3.stan #  log probabilty mass function and generated quantities for Yule-Simon Distribution
└── ys4.stan # Random generator number function for Yule-Simon Distribution

Intermediate/Bernoulli/ 

├── Intermediate_Bernoulli.R#  R file to run the example 
├── bernoulli_example.stan  # stan code
├── external_rstan.hpp # C++ code for a compute odds function  for rstan
└── external.hpp #  C++ code for a compute odds function  for cmdstanr 

Intermediate/YS_ZL/ 

├── Intemediate_YS.R #  R file to run the example 
├── Intemediate_YS.py # Python file to run the example 
├── ys_example.stan  # stan code
└── external.hpp # C++ code for a compute lpmf 

Dev_style/

├── Template_File.hpp #  strucuture file for coding _lpmf/lcdf/lccdf
├── yule_simon_lpmf.hpp # C++ and stan-math code for lpmf
├── yule_simon_lpmfAFS.hpp # C++ code extended version
├── yule_simon_rng.hpp #  C++ and stan-math code for rng
├── ys.hpp# C++  file to call into stan model compiling
├── Dev_ys.R # R file to run the example 
├── ys_example.stan  # stan code
└── dev_style.py # Python file to run the example

```

## Getting Started

### Dependencies

* The code runs on R 4.5.0 "How About a Twenty-Six"
* cmdstanr version 0.9.0
* CmdStan version: 2.36.0

## Authors

[Anna Freni Sterrantino](mailto:afrenisterrantino@turing.ac.uk).

