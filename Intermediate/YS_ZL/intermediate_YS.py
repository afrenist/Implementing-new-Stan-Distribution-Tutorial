
from cmdstanpy import CmdStanModel
model = CmdStanModel(stan_file='ys_example.stan', user_header='external.hpp',
                     force_compile=True)
fit = model.sample(data={'N':10, 'y':[1,1,1,1,1,2,2,2,2,2]})
fit.stan_variable('pmf_check')

# posterior distribution
import matplotlib.pyplot as plt
a = fit.stan_variable('a')
plt.hist(a, bins=30, density=True)
plt.xlabel('a')

# test the C++ function
def yule_simon_pmf(x, alpha):
    from scipy.special import beta
    return alpha * beta(alpha+1, x)

import numpy as np
np.allclose(fit.stan_variable('pmf_check')[0], yule_simon_pmf(1, 1.1))





