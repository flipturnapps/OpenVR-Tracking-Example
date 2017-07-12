import numpy as np
import matplotlib.pyplot as plt


xdata=np.loadtxt('r-x.txt')
ydata=np.loadtxt('r-y.txt')
top = min(len(xdata), len(ydata))
X=xdata[:top]
Y=ydata[:top]
plt.plot(X,Y,':ro')
#plt.ylim((0,55000))
plt.show() #or
plt.save('figure.png')

