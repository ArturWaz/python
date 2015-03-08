import numpy as np
from matplotlib import pyplot as plt

x = [0.02*i-10 for i in range(500)]
y = [0]*500

plt.ion()
line, = plt.plot(x,y)
plt.xlim([min(x),max(x)])
plt.ylim([min(y)-1,max(y)+1])

def addValueToBuffer(i):
    y.append(i)
    del y[0]
    #print(y[-1])
    return

def refresh():
    line.set_ydata(y)
    return

def pause(i):
    plt.pause(i)
    return

'''
for i in range(1,300):
    addValueToBuffer(i/10)
    refresh()
    pause(0.1)
'''
