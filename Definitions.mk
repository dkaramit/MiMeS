#######################################-data files-#######################################
# relative path to comoslogical data
cosmoDat=src/data/eos2020.dat
#
# relative path to chi data
axMDat=src/data/chi.dat
#
# relative path to anharmonic factor data
anFDat=src/data/anharmonic_factor.dat
#

#######################################-precision-#######################################
# LONG=long : in order to use long doubles. This is slower, but generally more accurate, since we can use tolerances down to 10^-11 (or below that). 
# LONG= : in order to use doubles. This is faster. However the tolerances of the ODE solver cannt be below 1e-8.

# use long doubles in C++
LONG=long
# use doubles in C++
# LONG= 

# use long doubles in python
LONGpy=long
# use doubles in python
# LONGpy=

#######################################-Runge Kutta method-#######################################
#------------------These are Rosenbrock (semi implicit) methods: Generally RECOMMENDED---------------------#
Solver=1

# RODASPR2 is fairly accurate and fast enough (faster than the other two from NaBBODES), but one 
# can use the others or provide another Butcher tableu and use it.
METHOD=RODASPR2 
# METHOD=ROS34PW2
# METHOD=ROS3w

#-------------------------These are explicit RK methods: Generally NOT RECOMMENDED--------------------------#
# Solver=2

# DormandPrince is fairly fast. It can be better than RODASPR2 at very low tolerances 
# because it is higher order. The other two can't even finish...
# METHOD=DormandPrince
# METHOD=CashKarp
# METHOD=RKF45