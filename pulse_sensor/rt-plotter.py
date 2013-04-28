###########################################################
# rt-plotter.py
#
# Autor: Sebastian Sepulveda
# Licencia: New BSD
# Descripcion:
# Recibe datos raw desde el puerto serial y los grafica
# en tiempo real utilizando matplolib
#
###########################################################

import threading
import serial
import time
import numpy as np
from matplotlib import pyplot as plt

## thread para adquirir datos del puerto serial
class SerialThread (threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
    def run (self):
        global data
        ser = serial.Serial('/dev/ttyACM0', 115200, timeout=5)
        while True:
            # Lee los ultimos 10 datos recibidos y busca aquel
            # rodeado de '\n'
            buffer=ser.read(10)
            ini=buffer.find('\n')
            buffer=buffer[ini+1:]
            end=buffer.find('\n')
            data=buffer[:end]

## Inicio de la aplicacion
data='0'
# inicia adquisicion de datos
SerialThread().start()
# permite graficos interactivos
plt.ion()

# variables
ydata=[0]*20
xdata=[0]*20
ax1=plt.axes()

# inicia plot
line,=plt.plot(ydata)
plt.ylim([0,1023])
plt.grid(True)

# permite registrar tiempo
iniTime=time.time()
t=0

while(t<20):
    t=time.time()-iniTime
    print t,':',data

    # agrega nuevo dato y elimina el ultimo dato
    ydata.append(data)
    xdata.append(t)
    del ydata[0]
    del xdata[0]
    # actualiza los datos
    line.set_xdata(np.arange(len(ydata)))
    line.set_ydata(ydata)
    # redibuja el plot
    plt.draw()
    # frecuencia de actualizacion de los datos
    time.sleep(.05)
