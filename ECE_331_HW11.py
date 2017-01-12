# Joseph Garcia
# ECE 331 Sheaff
# Homework 11

# 1)
#!/usr/bin/python

import smbus  # for bus communication
import time   # for delays
bus = smbus.SMBus(1)   # read from the proper bus (I2C1)

def main():
	bus_addr = 0x41   # listed address for the bus in i2cdetect
	LSByte_offset = 0x06
	MSByte_offset = 0x07     #memory address offsets for temp_t struct members
	
	MSByte = bus.read_byte_data(bus_addr, MSByte_offset)     # grab information from the temp_t struct
	LSByte = bus.read_byte_data(bus_addr, LSByte_offset)
	MSByte = (MSByte << 8)             # bitshift byte so that proper value for ADC value is created

	adc_val = (MSByte | LSByte)   # determine the adc value as it should be

	adc_val = int(adc_val)
	T_Kel = (adc_val - 60)/7.4     #Kelvin-ADC relationship
	T_F = (T_Kel - 273.15)*1.8 + 32  #Standard Fahrenheit-Kelvin relationship
	print T_F             #print Fahrenheit temperature on the terminal

if __name__ == "__main__":
	main()

# 2)

#!/usr/bin/python

#import matplotlib.pyplot as plt
#from matplotlib import rc
#import numpy
#from numpy.linalg import inv

#def calc_H(s_vals, H_s, NUM_OF_VALS):
#	H_s_int = [((8000*s)/((s)**(2) + 5000*s + 10**(6))) for s in s_vals ]
#	for i in range(0,NUM_OF_VALS - 1):                            # function to compute H(s)
#		H_s[i] = H_s_int[i]

#def H_to_dB(H_s, outputdB_s):
#	for i in range(0,NUM_OF_VALS - 1):
#		value = numpy.log10(H_s[i])                  # function to convert H(s) to dB
#		value = 20*value
#		outputdB_s[i] = value

#NUM_OF_VALS = 100
#s_vals = numpy.logspace(1,4,num=NUM_OF_VALS,endpoint=True,base=10.0)    # create logarithmically spaced list values for s
#H_s = [None] * NUM_OF_VALS
#outputdB_s = [None] * NUM_OF_VALS         # create empty lists for the functions

#calc_H(s_vals, H_s, NUM_OF_VALS)       # instantiate the function lists
#H_to_dB(H_s, outputdB_s)
#plt.semilogx(s_vals,outputdB_s)          # plot using logarithmic x-axis

#plt.xlabel('s (rad/sec)')
#plt.ylabel('H(s) in dB')                           # labels
#plt.title('H in dB vs. s (s = j$\omega$)')

#plt.savefig('DB_data.png')          # save the graphic
