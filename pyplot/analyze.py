import pyaudio
import wave
import numpy as np
from matplotlib.pyplot import *
import array

#chunk = 512*3

wf = wave.open('recorded.wav', 'rb')

print str(wf.getsampwidth()*8)+" bit"
print str(wf.getnchannels())+" channels"
print str(wf.getframerate())+" rate"
print str(wf.getnframes())+" frames"

# read data
data = wf.readframes(wf.getnframes())
data = array.array('i',data)

out=np.fft.fft(data)
plot(abs(out))
show()
plot(data)
show()

