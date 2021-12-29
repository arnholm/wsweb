import io
import matplotlib.pyplot as plt
from   matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas
import matplotlib.dates as mdates
from   matplotlib.figure import Figure
from   flask import Flask, render_template, request, Response

#-- Weather Station API
from wsapi import wsapi

# -- helper class to manage current selection state and plotting
class ws_helper:
    def __init__(self):
        self.sensor = 'opres'
        self.ndays = 7
        self.ws = wsapi()

    def make_plot(self):
        # -- default matplotlib graph style
        plt.style.use('ggplot')

        #-- query data for the selected number of days and for selected sensor
        self.ws.query(self.ndays)
        x = self.ws.time()
        y = self.ws.sensor(self.sensor)
        title = y[1] 

        #-- plot layout
        plt.rcParams["figure.figsize"] = [13.0, 3.50]
        plt.rcParams["figure.autolayout"] = True

        #-- create graph
        fig = Figure()
        axis = fig.add_subplot(1, 1, 1)
        axis.plot(x[0], y[0])
        axis.set_title(title,visible=True)
        axis.figure.autofmt_xdate(rotation=10, ha='center');
        axis.xaxis.set_major_formatter(mdates.DateFormatter('%Y-%m-%d %H:%M'))
        legend = [y[1]]
        axis.legend(legend)    

        #-- create PNG
        output = io.BytesIO()
        FigureCanvas(fig).print_png(output)
        return Response(output.getvalue(), mimetype='image/png')


app = Flask(__name__)  # Flask application
wsh = ws_helper()      # Weather Station helper

@app.route('/simple.png')
def simple_image():
    return wsh.make_plot()

@app.route('/submit',methods=['POST','GET'])
def submit():
    if request.method=='POST':
        wsh.sensor = str(request.form['sensor'])
        wsh.ndays = int(request.form['ndays'])

    # -- simply render the page, return selections
    return render_template(
        'index.html'
        ,title=wsapi.name() + ' (' + str(wsapi.get_elevation()) + 'm)'
        ,sensors=wsapi.sensor_list()
        ,selected_sensor=wsh.sensor
        ,ndays=wsh.ndays
    )

@app.route('/')
def index():
    # -- simply render the page, return selections
    return render_template(
        'index.html'
        ,title=wsapi.name() + ' (' + str(wsapi.get_elevation()) + 'm)'
        ,sensors=wsapi.sensor_list()
        ,selected_sensor=wsh.sensor
        ,ndays=wsh.ndays
    )

if __name__ == '__main__':
    import sys
    app.run(debug='d' in sys.argv[-1])
