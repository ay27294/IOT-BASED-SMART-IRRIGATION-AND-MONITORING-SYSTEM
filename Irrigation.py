import time
from Adafruit_IO import Client, Feed, Data,RequestError
from joblib import dump, load
import numpy as np
ADAFRUIT_IO_KEY = 'aio_Mtbe731IiUdaXFZtsTvWLo4YKl9q'
ADAFRUIT_IO_USERNAME = 'shivvamm'
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
model = load('irrigation.joblib') #ML model
hum_threshold = 60
hum_val = 0
temp_threshold1 = 15
temp_threshold2 = 23
temp_val = 0
rain = aio.feeds('rain')
humidity = aio.feeds('humidity1')
temperature = aio.feeds('temperature1')
moisture = aio.feeds('soil')
a=0
while(True):
 if(int(aio.receive(rain.key).value)==0):
 # print(aio.receive(rain.key).value)
Page | 51
 hum_val = int(aio.receive(humidity.key).value)
 temp_val = int(aio.receive(temperature.key).value)
 mois_val = int(aio.receive(moisture.key).value)
 # model = load('irrigation.joblib')
 features = np.array([[temp_val, 430, hum_val, mois_val, aio.receive(rain.key).value]])
 pred = model.predict(features)
 if(pred[0]>10):
 print(pred[0])
 aio.send_data('moter',1)
 print("moter on")
 time.sleep(pred[0])
 else:
 time.sleep(0.1)
 aio.send_data('moter',0)
 a=0
 print("moter off due to rain")
 hum_threshold = 60
 time.sleep(10)
