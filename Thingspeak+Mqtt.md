In order to connect the peristaltic pumps to the TDS, pH, and temperature sensors, which will allow fluids to flow at specific times based on the correct readings, we used [MQTT](https://mqtt.org/).

The data displayed pertains to the pH level.
<img src="https://github.com/OdedHol/agrotech/blob/main/Images/WhatsApp%20Image%202023-06-28%20at%2017.17.13.jpeg">

The data displayed pertains to the temperature of the water.
<img src="https://github.com/OdedHol/agrotech/blob/main/Images/WhatsApp%20Image%202023-06-28%20at%2017.15.58.jpeg">

It can be observed that data is received every 5 minutes, and during specific hours, for a duration of half an hour, the system flows fluids as needed.

All the remaining data regarding water conductivity, temperature of the leaf, and ambient temperature is displayed on [Thingspeak](https://thingspeak.com/channels/2184926).

### codes

[Press pump](Codes/Pres_Pump.ino)

[Pumps](Codes/Pumps.ino)

[Under water sensors](Codes/Under_water_sensors.ino)

[Leaf sensors](Codes/leaf_sensors_ino)


                 
                
