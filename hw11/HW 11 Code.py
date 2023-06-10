from ulab import numpy as np  # to get access to ulab numpy functions
import board
import time
import digitalio
import analogio
import math


# Declare an array with some made up data like
data = np.zeros(1024)
t = np.arange(1024)

sine1 = np.sin(20 * math.pi * t/1024)
sine2 = np.sin(50 * math.pi * t/1024)
sine3 = np.sin(90 * math.pi * t/1024)

data = sine1 + sine2 + sine3
fft_data = np.fft.fft(data)
sum = 0

while 1:
#    for ii in range(len(fft_data)):
#        sum = sum + math.sqrt(((fft_data[ii][0])**2) + (fft_data[ii][1]**2))
 #       if (ii % 5) == 0:
  #          print("(" + str(sum) + ",)")
   #         time.sleep(0.1)
    #        sum = 0

    for ii in range(len(sine3)):
        print("(" + str(sine1[ii]) + ", " + str(sine2[ii]) + ", " + str(sine3[ii]) + ")")
        time.sleep(0.1)
