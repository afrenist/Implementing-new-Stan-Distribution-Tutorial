

from cmdstanpy import CmdStanModel
model = CmdStanModel(stan_file='ys_example.stan', user_header='ys.hpp',
                     force_compile=True)
fit = model.sample(data={'N':10, 'y':[1,1,1,1,1,2,2,2,2,2]}, seed=1234)


# posterior distribution
import matplotlib.pyplot as plt
a = fit.stan_variable('a')
plt.hist(a, bins=30, density=True)
plt.xlabel('a')

# test the C++ lpmf
def yule_simon_pmf(x, alpha):
    from scipy.special import beta
    return alpha * beta(alpha+1, x)

import numpy as np
np.exp(np.log(yule_simon_pmf(1, 1.1)) + np.log(yule_simon_pmf(2, 1.2)))
fit.stan_variable('pmf_check')[0]

# test the C++ rng
rnum = fit.stan_variable('randomYS').flatten() + 1
np.mean(rnum==1)
yule_simon_pmf(1, 1.1)






