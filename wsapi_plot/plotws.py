# Minimal example showing use of matplotlib with data from wsapi (Weather Station API)
import matplotlib.pyplot as plt
from wsapi import wsapi

def main():

    #-- begin weather station query session
    ws = wsapi()

    #-- query data for the last 10 days
    ws.query(ndays=100)

    #-- create XY-plot from outside temperature
    x = ws.time()
    y1 = ws.sensor('opres')
    plt.plot(x[0],y1[0])

    #-- create legend
    legend = [y1[1]]
    plt.legend(legend)


    #-- add a annotations
    plt.title(wsapi.name()+' (elev. '+str(wsapi.get_elevation()) +') ')
    plt.xlabel(x[1])
    plt.grid(visible=True,linestyle='dashed',linewidth=0.5)

    #-- show the graph on screen
    plt.show()

    #-- close connection to database
    ws.close()

if __name__ == "__main__":
    main()