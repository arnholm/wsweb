# Weather Station API
# Carsten Arholm 2020

import os
import sys
from datetime import datetime

# load environment settings from wsapi.env
# it must define WSAPI_PATH,WSAPI_DB,WSAPI_NAME,WSAPI_ELEVATION as a minimum
from dotenv import load_dotenv
load_dotenv(dotenv_path="wsapi.env")

# enable the path to where the wsapi_py.so file exists so it can be found
os.sys.path.append(os.environ['WSAPI_PATH']) 
import wsapi_py

# python level API class 
class wsapi:
    def __init__(self):
        self.db = 0
        self.ndays = 0
        self.data = 0  # in-core data

    # return site elevation
    def get_elevation():
        return float(os.environ['WSAPI_ELEVATION'])

    # site name
    def name():
        return os.environ['WSAPI_NAME']

    # query data for ndays in the past
    def query(self,ndays):
        if ndays != self.ndays:
            # open database, read some data and close it
            self.db = wsapi_py.wsdb(os.environ['WSAPI_DB'])
            self.db.set_elevation(float(os.environ['WSAPI_ELEVATION']))
            self.ndays = ndays
            self.data = self.db.get_data_days(self.ndays)
            self.db.close()

    # return time as list of datetime
    def time(self):
        #-- convert unix time to Python datetime
        x = []
        for t in self.data.tstmp():
            x.append(datetime.fromtimestamp(t))
        return (x,"time [local]")

    # list of tuples (sensor name, description)
    def sensor_list():
        slist = [
                     ('itemp',"indoor temperature [C]" )
                    ,('otemp',"outdoor temperature [C]" )
                    ,('ihumi',"indoor humidity [%]" )
                    ,('ohumi',"outdoor humidity [%]" )
                    ,('opres',"pressure [hPa]" )
                    ,('owspd',"wind speed [m/s]" )
                    ,('owgus',"wind gust [m/s]" )
                    ,('owdir',"wind direction [deg]" )
                    ,('orain',"accumulated rain [mm]" )  
               ]
        return slist

    # return (sensor time series, sensor description) based on sensor name
    def sensor(self,name):
        # return sensor time series based on sensor name
        sensors = wsapi.sensor_list()
        switch={
            sensors[0][0]: (self.data.itemp(),sensors[0][1])
           ,sensors[1][0]: (self.data.otemp(),sensors[1][1])
           ,sensors[2][0]: (self.data.ihumi(),sensors[2][1])
           ,sensors[3][0]: (self.data.ohumi(),sensors[3][1])
           ,sensors[4][0]: (self.data.opres(),sensors[4][1])
           ,sensors[5][0]: (self.data.owspd(),sensors[5][1])
           ,sensors[6][0]: (self.data.owgus(),sensors[6][1])
           ,sensors[7][0]: (self.data.owdir(),sensors[7][1])
           ,sensors[8][0]: (self.data.orain(),sensors[8][1])
        }
        return switch.get(name,(self.time()[0],name+'=name error'))

