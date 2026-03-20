#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Jan  7 19:02:05 2022
Design 1 Analog/Digital Filter Module:
    Understand mathematical filter functions (Butterworth, Bessel, Chebyshev)
    Uses scipy.signal package for analog and digital filter design
@author: erin patrick
"""
import numpy as np
from scipy import signal
import matplotlib.pyplot as plt
from scipy.fftpack import fft
from scipy.fftpack import fftfreq
from scipy.fftpack import ifft



#--------Time domain signals / Input waveforms---------
fs = 100e3  #sampling frequency samples/s (note one can only plot frequencies up to 1/2fs, i.e. Nyquist frequency)
dt = 1.0/fs #time step (s)
end = 0.01  #length of time signal (s)
N = int(end*fs)   #number of samples 
t = np.linspace(0,end,N)   #array of time points of quantity N (s)

##define the square wave using signal.square 
fsq = 1000 + 68   #repetition frequency of square wave (Hz)
square_sig=signal.square(2 * np.pi * fsq * t)  # duty cycle defaults to 0.5

# add any other waveforms you wish to explore (e.g., sine, sine with braodband noise, etc.)


#-----------Filters---------------------
#Specs for lowpass filter

wc2= 2*np.pi*(fsq+100)  # cut off frequency (rad/s), Change this accordignly
order = 4 #filter order (1=20 dB/decade rolloff, 2 = 40 dB/dec, etc.)
fplot = np.logspace(1, 4, N)  # (specify frequency range of interest, 2=10e2, 4=10e4)

#  Butterworth filter
b, a = signal.butter(order, wc2, btype='low', analog = True)
w, h = signal.freqs(b, a, worN=fplot*2*np.pi)

#  Bessel
b2, a2 = signal.bessel(order,  wc2 ,'lowpass', analog=True)
w2, h2 = signal.freqs(b2, a2, worN=fplot*2*np.pi)

#   Chebyshev    
rp = 1  #ripple in pass pand
b3, a3 = signal.cheby1(order, rp, wc2 , 'lowpass', analog=True)
w3, h3 = signal.freqs(b3, a3, worN=fplot*2*np.pi)

#--------------------------------------------------



#-----------Plot the fitler response and time-domain filtered signal------

#plot the frequency response of the analog filters

plt.figure(1)
plt.subplot(2,1,1)
plt.title('Anallog Filter')
plt.semilogx(w/(2*np.pi), 20 * np.log10(abs(h)), "b", w2/(2*np.pi), 20 * np.log10(abs(h2)), '--r', w3/(2*np.pi), 20 * np.log10(abs(h3)),'g')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Analog Filter Gain (dB)')
plt.margins(0, 0.1)
plt.grid(which='both', axis='both')
plt.axvline(fsq, color='k') # square wave frequency

# Compute FFT of time-domain signals
fft_square = fft(square_sig) #complex value of the FFT spectrum
freq_pulse = fftfreq(len(t), d=dt) #frequnecy array, pass in length of signal and step size

# compute a transfer function of a filter type using the same frequencies as the FFT of the time signal
w, h = signal.freqs(b, a, worN=freq_pulse*2*np.pi) # change filter type as needed 
w2, h2 = signal.freqs(b2, a2, worN=freq_pulse*2*np.pi) # change filter type as needed (i.e., a,b go to a Butterworth filter. , a2,b2 go to a Bessel, etc.)
w3, h3= signal.freqs(b3, a3, worN=freq_pulse*2*np.pi) 

#Compute an inverseFFT of the filtered signal (multipy square FFT by filter transfer function )
filteredt1 = ifft(fft_square*h)
filteredt2 = ifft(fft_square*h2)
filteredt3 = ifft(fft_square*h3)


#plot square wave and filtered time domain wave

plt.subplot(2,1,2)

plt.plot(t, square_sig, '-k', t, (filteredt1), '-b', t, filteredt2, '--r', t, filteredt3, '-g')
plt.legend(['original','filtered-Butterworth','filtered-Bessel','filtered-Chebyshev'])
plt.xlabel('time (s)')
plt.ylabel('Voltage (V)')

# plt.legend(['Butterworth', 'Bessel', 'Chebyshev'])
plt.show()

#-----------Plot the frequency-domain filtered signal------

# compute a transfer function of a filter type using the same frequencies as the FFT of the time signal
#w, h = signal.freqs(b, a, worN=freq_pulse*2*np.pi) # change filter type as needed (i.e., a,b go to a Butterworth filter. , a2,b2 go to a Bessel, etc.)
#w, h2 = signal.freqs(b2, a2, worN=freq_pulse*2*np.pi) # change filter type as needed (i.e., a,b go to a Butterworth filter. , a2,b2 go to a Bessel, etc.)
#w, h3= signal.freqs(b3, a3, worN=freq_pulse*2*np.pi) # change filter type as needed (i.e., a,b go to a Butterworth filter. , a2,b2 go to a Bessel, etc.)
# Multiply the FFT of the square wave by the transfer function of the filter to get the filterred signal
filtered1 =2.0/N * np.abs(fft_square[0:int(len(freq_pulse)/2)]*(h[0:int(len(freq_pulse)/2)]))  #only take 1/2 of data since it is mirrored and scale by factor to get correct output
filtered2 =2.0/N * np.abs(fft_square[0:int(len(freq_pulse)/2)]*(h2[0:int(len(freq_pulse)/2)]))
filtered3 =2.0/N * np.abs(fft_square[0:int(len(freq_pulse)/2)]*(h3[0:int(len(freq_pulse)/2)]))



plt.figure(3)
# plot the FFT of the original signal
plt.semilogx(freq_pulse[0:int(len(freq_pulse)/2)], 2.0/N * np.abs(fft_square[0:int(len(freq_pulse)/2)]), '-k')
#plot the frequency response of the filtered signal
plt.semilogx(freq_pulse[0:int(len(freq_pulse)/2)], filtered1 , '-b', freq_pulse[0:int(len(freq_pulse)/2)], filtered2 , '--r',freq_pulse[0:int(len(freq_pulse)/2)], filtered3 , '-g')
plt.xlabel('frequency (Hz)')
plt.ylabel('FFT Magnitude')
plt.legend(['original','filtered-Butterworth','filtered-Bessel','filtered-Chebyshev' ])
plt.title('Signal Frequency Response (analog)')


#-----------Digital Filter Section-------------------
rate = 10000 #(samples/s, rate defined in the DMA Python code)
order = 4
x = np.linspace(0, (1023*1/rate), 1023)
y = signal.square(2*np.pi*fsq*x)+1.0
f_cutoff = fsq+00 #(Hz)
w_cutoff = f_cutoff*2*np.pi
sos_cheb = signal.cheby1(order, rp, f_cutoff, 'lowpass', fs=rate, output='sos')
sos_butter = signal.butter(order, f_cutoff, 'lowpass', fs=rate, output='sos')
sos_bessel = signal.bessel(order, f_cutoff, 'lowpass', fs=rate, output='sos')
sosfilt_cheb = signal.sosfilt(sos_cheb, y)
sosfilt_butter = signal.sosfilt(sos_butter, y)
sosfilt_bessel = signal.sosfilt(sos_bessel, y)

plt.figure(4)
plt.subplot(2,1,2)
plt.plot(x,y, 'k', x, sosfilt_butter, '-b', x, sosfilt_bessel, '--r', x, sosfilt_cheb, '-g')
plt.xlabel('Time (s)')
plt.ylabel('Time Signal (V)')
plt.legend(['original','filtered-Butterworth','filtered-Bessel','filtered-Chebyshev'])
plt.show()

#plt.figure(5)
w, h_butter = signal.sosfreqz(sos_butter)
w, h_bessel = signal.sosfreqz(sos_bessel)
w, h_cheb = signal.sosfreqz(sos_cheb)
db_butter = 20*np.log10(np.maximum(np.abs(h_butter), 1e-5))
db_bessel = 20*np.log10(np.maximum(np.abs(h_bessel), 1e-5))
db_cheb = 20*np.log10(np.maximum(np.abs(h_cheb), 1e-5))

plt.subplot(2,1,1)
plt.title('Digital Filter')
plt.semilogx(w/np.pi*rate/2, db_butter,'-b', w/np.pi*rate/2, db_bessel,'--r', w/np.pi*rate/2, db_cheb, '-g')
plt.grid(which='both', axis='both')
plt.xlabel('frequency (Hz)')
plt.ylabel('Digital Filter Gain (dB)')
plt.axvline(fsq, color='k') # square wave frequency
plt.legend(['Butterworth', 'Bessel', 'Chebyshev'])

plt.show()
