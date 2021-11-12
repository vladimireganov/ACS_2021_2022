import matplotlib.pyplot as plt
import matplotlib.cbook as cbook
import matplotlib.gridspec as gridspec
from matplotlib.patches import Polygon
import csv
import numpy as np
import pandas as pd
import math

df = pd.read_csv("data.csv",sep=",")
####initial decribing of data
print (df)
print (df.shape)
print (df.dtypes)
print (df.columns)
print (df.describe(include='all'))

# df = df[107379:115235]


df['iteration_time'] = df.iteration_time

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

fig12 = plt.figure()
ax12 = plt.subplot()
ax12.plot(df.iteration_time,df.pressure)# marker='o')
ax12.legend(['pressure'], loc = 'upper right')
ax12.set_xlabel('Time (s)')


fig13 = plt.figure()
ax13 = plt.subplot()
ax13.plot(df.iteration_time,df.altitude)# marker='o')
ax13.legend(['altitude'], loc = 'upper right')
ax13.set_xlabel('Time (s)')

df['Acc_f'] = (df.acceleration_x + df.acceleration_y  ) #** .5


fig14 = plt.figure()
ax14 = plt.subplot()
ax14.plot(df.iteration_time,df.Acc_f)# marker='o')
ax14.legend(['full acc'], loc = 'upper right')
ax14.set_xlabel('Time (s)')


df['dT'] = (df.iteration_time).diff()

fig15 = plt.figure()
ax15 = plt.subplot()
ax15.plot(df.iteration_time,df.dT)# marker='o')
ax15.legend(['delta t'], loc = 'upper right')
ax15.set_xlabel('Time (s)')


fig16 = plt.figure()
ax16 = plt.subplot()
# ax16.plot(df.iteration_time,df.Acc_f)# marker='o')
# ax16.plot(df.iteration_time,df.acceleration_x)# marker='o')
ax16.plot(df.iteration_time,df.acceleration_y)# marker='o')
# ax16.plot(df.iteration_time,df.acceleration_z)# marker='o')
# ax16.legend(['acceleration_x', 'acceleration_y', 'acceleration_z'], loc = 'upper right')
ax16.legend(['acceleration_y'], loc = 'upper right')
ax16.set_xlabel('Time (s)')

# xtick = pd.date_range( start=df.index.min( ), end=df.index.max( ), freq='W' )
# ax16.set_xticks( xtick, minor=True )
ax16.grid('on', which='minor', axis='x' )
ax16.grid('on', which='major', axis='x' )
ax16.grid('on', which='minor', axis='y' )
ax16.grid('on', which='major', axis='y' )


fig17 = plt.figure()
ax17 = plt.subplot()
# ax17.plot(df.iteration_time,df.Acc_f)# marker='o')
ax17.plot(df.iteration_time,df.gyroscope_x)# marker='o')
ax17.plot(df.iteration_time,df.gyroscope_y)# marker='o')
ax17.plot(df.iteration_time,df.gyroscope_z)# marker='o')
ax17.legend([ 'gyroscope_x', 'gyroscope_y', 'gyroscope_z'], loc = 'upper right')
ax17.set_xlabel('Time (s)')

# xtick = pd.date_range( start=df.index.min( ), end=df.index.max( ), freq='W' )
# ax16.set_xticks( xtick, minor=True )
ax17.grid('on', which='minor', axis='x' )
ax17.grid('on', which='major', axis='x' )
ax17.grid('on', which='minor', axis='y' )
ax17.grid('on', which='major', axis='y' )



fig18 = plt.figure()
ax18 = plt.subplot()
ax19 = ax18.twinx()
# ax16.plot(df.iteration_time,df.Acc_f)# marker='o')
# ax16.plot(df.iteration_time,df.acceleration_x)# marker='o')
ax18.plot(df.iteration_time,df.acceleration_y)# marker='o')
ax19.plot(df.iteration_time,df.gyroscope_y,color='green')
# ax16.plot(df.iteration_time,df.acceleration_z)# marker='o')
# ax16.legend(['acceleration_x', 'acceleration_y', 'acceleration_z'], loc = 'upper right')
ax18.legend(['acceleration_y'], loc = 'upper right')
ax18.set_xlabel('Time (s)')
ax19.legend(['gyroscope_y'], loc = 'upper left')

plt.show()