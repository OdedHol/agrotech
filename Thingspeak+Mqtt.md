In order to connect the peristaltic pumps to the TDS, pH, and temperature sensors, which will allow fluids to flow at specific times based on the correct readings, we used [MQTT](https://mqtt.org/).

The data displayed pertains to the pH level.
<img src="https://github.com/OdedHol/agrotech/blob/main/Images/WhatsApp%20Image%202023-06-28%20at%2017.17.13.jpeg">

The data displayed pertains to the temperature of the water.
<img src="https://github.com/OdedHol/agrotech/blob/main/Images/WhatsApp%20Image%202023-06-28%20at%2017.15.58.jpeg">

It can be observed that data is received every 5 minutes, and during specific hours, for a duration of half an hour, the system flows fluids as needed.

All the remaining data regarding water conductivity, temperature of the leaf, and ambient temperature is displayed on [Thingspeak](https://thingspeak.com/channels/2184926).

In addition, we utilized three different [Relays](https://randomnerdtutorials.com/guide-for-relay-module-with-arduino/). Two of them were used for controlling the pH pumps and the conductivity (TDS) pumps and 
the third was used for the circulation pump.

Relays have 2 working options: Normally closed or Normally open - [click](https://randomnerdtutorials.com/guide-for-relay-module-with-arduino/) to read detail.

We intended that when the pH in the system is higher than 5.5, the relay would be triggered to send a signal of 1 - To dispense acid. In that condition the relay would be on NC.
Likewise, the second relay would be activated within the specified range.
The third relay is intended to activate when both measurements have been taken, and fluid flow according to the data. The circulation pump will operate and mix all the fluids to achieve a uniform solution, ensuring accurate measurements.

<img src="https://github.com/OdedHol/agrotech/blob/main/Images/WhatsApp%20Image%202023-06-28%20at%2012.34.10.jpeg" width="400" height="550">

### codes

[Press pump](Codes/Pres_Pump.ino)

[Pumps](Codes/Pumps.ino)

[Under water sensors](Codes/Under_water_sensors.ino)

[Leaf sensors](Codes/leaf_sensors_ino)


                 
                
