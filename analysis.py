import matplotlib.pyplot as plt
import matplotlib.cbook as cbook
import matplotlib.gridspec as gridspec
from matplotlib.patches import Polygon
import csv
import numpy as np
import pandas as pd
import math

df = pd.read_csv("data.csv",sep=",") # texas launch
####initial decribing of data
print (df)
print (df.shape)
print (df.dtypes)
print (df.columns)
print (df.describe(include='all'))


fig9 = plt.figure()
ax9 = plt.subplot()
ax9.plot(df.iteration_time,df.acceleration_x)# marker='o')
ax9.legend(['acceleration_x'], loc = 'upper right')
ax9.set_xlabel('Time (s)')

fig10 = plt.figure()
ax10 = plt.subplot()
ax10.plot(df.iteration_time,df.acceleration_y)# marker='o')
ax10.legend(['acceleration_y'], loc = 'upper right')
ax10.set_xlabel('Time (s)')

fig11 = plt.figure()
ax11 = plt.subplot()
ax11.plot(df.iteration_time,df.acceleration_z)# marker='o')
ax11.legend(['acceleration_z'], loc = 'upper right')
ax11.set_xlabel('Time (s)')


plt.show()