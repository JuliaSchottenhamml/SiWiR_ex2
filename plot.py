import numpy as np
import matplotlib.pyplot as pl
import scipy.optimize as optimize

data = np.loadtxt("data/result.txt").transpose()

slice = np.isclose(data[1], 1)
pl.plot(data[0][slice], data[2][slice], "x", label="simulation")

fitfunc = lambda p, x: p[0] * pow(x, p[1]) # Target function
errfunc = lambda p, x, y: fitfunc(p, x) - y # Distance to the target function
p0 = [500.0, 2.0, 0] # Initial guess for the parameters
p1, success = optimize.leastsq(errfunc, p0[:], args=(data[0][slice], data[2][slice]))

x = np.linspace(data[0][slice].min(), data[0][slice].max(), 100)
y = fitfunc(p1, x)
pl.plot(x, y, label = r"$\mathcal{O}$($n^{"+"{0:.3}".format(p1[1])+"}$)")

pl.legend(loc="upper left")
#pl.loglog(basex=2, basey=2)
pl.grid()

pl.xlabel("system size")
pl.ylabel("runtime (s)")

pl.savefig("img/complexity.pdf")
pl.close()

slice = np.isclose(data[0], 32)
pl.plot(data[1][slice], data[2][slice], "x", label ="32x32")
slice = np.isclose(data[0], 33)
pl.plot(data[1][slice], data[2][slice], "x", label ="33x33")
slice = np.isclose(data[0], 1024)
pl.plot(data[1][slice], data[2][slice], "x", label ="1024x1024")
slice = np.isclose(data[0], 1025)
pl.plot(data[1][slice], data[2][slice], "x", label ="1025x1025")
slice = np.isclose(data[0], 2048)
pl.plot(data[1][slice], data[2][slice], "x", label ="2048x2048")
slice = np.isclose(data[0], 2049)
pl.plot(data[1][slice], data[2][slice], "x", label ="2049x2049")

pl.legend()
pl.loglog(basex=2, basey=2)
pl.grid()

pl.xlabel("# of cpus")
pl.ylabel("runtime (s)")

pl.savefig("img/scaling.pdf")
pl.close()

slice = np.isclose(data[0], 2048)
pl.plot(data[1][slice], data[2][slice], "x", label ="2048x2048")
x = np.arange(1,33)
y = data[2][slice][0] / x
pl.plot(x,y, label="theory")

pl.legend()
pl.loglog(basex=2, basey=2)
pl.grid()

pl.xlabel("# of cpus")
pl.ylabel("runtime (s)")

pl.savefig("img/scalingTheory.pdf")
pl.close()