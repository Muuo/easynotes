import pyaudio
import wave
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
chunk=len(data)
# play stream
while data != '':
	#data = wf.readframes(chunk)
	ndata1=''
	ndata2=''
	state=True
	y=0
	for x in range(chunk):
		if y>3:
			y=0
			state=not state
		if state:
			ndata1+=data[x]
		else:
			ndata2+=data[x]
		y+=1
	ndata1=array.array('i',ndata1)
	ndata2=array.array('i',ndata2)
	clf()
	plot(ndata1)
	#plot(ndata2)
	show()
	#pause(2)
	data=wf.readframes(chunk)
