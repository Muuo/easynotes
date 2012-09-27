import pyaudio
import wave

chunk = 1024

wf = wave.open('recorded.wav', 'rb')

p = pyaudio.PyAudio()

# open stream
stream = p.open(format =
                p.get_format_from_width(wf.getsampwidth()),
                channels = wf.getnchannels(),
                rate = wf.getframerate(),
                output = True)
print str(wf.getnchannels())+" channels"
print str(wf.getframerate())+" rate"

# read data
data = wf.readframes(chunk)

# play stream
while data != '':
    stream.write(data)
    data = wf.readframes(chunk)

stream.close()
p.terminate()
