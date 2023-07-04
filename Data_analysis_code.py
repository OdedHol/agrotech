import urllib
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import matplotlib.dates as mdates

# define what to download
channels = "2184926"
fields = "1,2,4,6,7,8"

# see here all the "Query String Parameters"
# https://www.mathworks.com/help/thingspeak/readdata.html

results = 8000
# Download the last N results
# url = f"https://api.thingspeak.com/channels/{channels}/fields/{fields}.csv?results={results}"

minutes = "100"
# Number of minutes before NOW to include in response.
# url = f"https://api.thingspeak.com/channels/{channels}/fields/{fields}.csv?minutes={minutes}"

start = "2023-06-28%2000:00:00"  # format YYYY-MM-DD%20HH:MM:SS
end = "2023-06-30%2000:00:00"    # format YYYY-MM-DD%20HH:MM:SS
url = f"https://api.thingspeak.com/channels/{channels}/fields/{fields}.csv?start={start}&end={end}"

# Open the URL and get the response
data = urllib.request.urlopen(url)
# Read the response data
d = data.read()
# save data to csv
filename1 = "downloaded_data.csv"
file = open(filename1, "w")
file.write(d.decode('UTF-8'))
file.close()

# load data
df = pd.read_csv(filename1)
# rename columns
df = df.rename(columns={"created_at": "timestamp",
                        "field1": "ShtTemp",
                        "field2": "ShtHumi",
                        "field4": "MlxObject",
                        "field6": "pH",
                        "field7": "UnderWaterTemp",
                        "field8": "TDS",
                        # "field4": "whatever",
                        })
# set timestamp as index
df['timestamp'] = pd.to_datetime(df['timestamp'])
df = df.set_index('timestamp')
df

#clean the data from Nan
clean_data_pH = df['pH'].dropna()
clean_data_TDS=df['TDS'].dropna()
clean_data_Under = df['UnderWaterTemp'].dropna()
clean_data_ShtTemp=df['ShtTemp'].dropna()
clean_data_ShtHumi=df['ShtHumi'].dropna()
clean_data_MlxObject=df['MlxObject'].dropna()
filtered_data_pH=clean_data_pH[clean_data_pH > 0]
filtered_data_under=clean_data_Under[clean_data_Under < 40]
filtered_data_TDS=clean_data_TDS[clean_data_TDS>10]
filtered_data_TDS=filtered_data_TDS/100

#Plot above ground sensors data
%matplotlib widget
fig, ax = plt.subplots(1)
ax.plot(df['ShtTemp'], label="ShtTemp") 
ax.plot(df['ShtHumi'], label="ShtHumi")
ax.plot(df['MlxObject'], label="MlxObject")
ax.axhline(y=0, color='red', linestyle='--') #y=1 is the result we expected 
ax.set_xlabel("time")
ax.set_ylabel("variable")
ax.legend()


plt.show()

#Plot under water sensor data

%matplotlib widget
fig, ax = plt.subplots(1)
ax.plot((filtered_data_pH), label="pH")
ax.plot((filtered_data_under), label="UnderWaterTemp")
ax.plot((filtered_data_TDS), label="TDS")
ax.axhline(y=0, color='red', linestyle='--') #y=1 is the result we expected 
ax.set_xlabel("time")
ax.set_ylabel("variable")
ax.legend()


plt.show()





