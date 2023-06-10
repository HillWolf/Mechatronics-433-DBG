import matplotlib.pyplot as plt
import numpy as np
import csv

t = [] # column 0
d = [] # column 1

with open('sigA.csv') as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0])) # leftmost column
        d.append(float(row[1])) # second column

dt = t[1] - t[0]
# t = np.arange(0.0, 1.0, dt) # 10s
# a constant plus 100Hz and 1000Hz

Fs = 10000 # sample rate
Ts = 1.0/Fs; # sampling interval
ts = np.arange(0,t[-1],Ts) # time vector


# uncomment one of the filters to get it to work

# MAF Filter
# f = []
# av = []
# for dp in d:
#     if len(f) < 10:
#         f.append(0)
#         av.append(dp)
#     else:
#         f.append(sum(av)/len(av))
#         av.pop(0)
#         av.append(dp)

# IIR Filter
# f = []
# A = 0.9
# B = 0.1
# for dp in d:
#     if len(f) == 0:
#         f.append(0)
#     else:
#         f.append(f[-1]*A + dp*B)

# FIR Filter
# f = []

# #short list of weights bc python was being super slow, don't think these are very correct
# # because idk much about signals but this was better than other ones I tried before.
# h = [
#     0.000000000000000001,0.000000000000000000,0.005758461608425023,-0.000000000000000005,
#     -0.048720975300308715,0.000000000000000014,0.292961742380823398,0.500001542622120376,
#     0.292961742380823453,0.000000000000000014,-0.048720975300308743,-0.000000000000000005,
#     0.005758461608425029,0.000000000000000000,0.000000000000000001]
# av = []
# sl = []

# for dp in d:
#     if len(f) < len(h):
#         f.append(0)
#         av.append(dp)
#     else:
#         for count in range(len(h)):
#             sl.append(av[count] * h[count])
#             av.pop(0)
#             av.append(dp)
#         f.append(sum(sl)/len(sl))
#         sl.clear




y = d # the data to make the fft from
n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y = np.fft.fft(y)/n # fft computing and normalization
Y = Y[range(int(n/2))]

y2 = f
n2 = len(y2)
k2 = np.arange(n2)
T2 = n2/Fs
frq2 = k2/T2
frq2 = frq2[range(int(n2/2))]
Y2 = np.fft.fft(y2)/n2
Y2 = Y2[range(int(n2/2))]

fig, (ax1, ax2) = plt.subplots(2, 1)
ax1.title.set_text("FIR | Sampling Rate: 400Hz | Cutoff: 100Hz | Bandwidth: 20Hz")
ax1.plot(t,y,'b', t,y2,'r')
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax2.loglog(frq,abs(Y),'b',frq2,abs(Y2),'r') # plotting the fft
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')
plt.show()