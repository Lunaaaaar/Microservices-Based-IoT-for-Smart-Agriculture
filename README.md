# Microservices-Based-IoT-for-Smart-Agriculture
Micro-Services based IOT For Smart Agriculture: Ensure contactless and automated measurement of ﬁeld conditions to maximize the yield. 

Technologies used for backend data storage and visualization: Docker, LAMP server, inﬂuxDB, Grafana, and Arduino.

Hardware Design

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/05d10677-60cf-4fb7-8440-0bb5446ec371)

Flow Chart

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/ceb35709-4cda-4520-a8ea-bf9cccc72a9a)

Use Case Diagram

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/9efeb05b-d2f0-4e28-8241-cb5b713013ef)

System Architecture

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/f83bc69d-b10a-4f6d-94e2-490712d6ab21)


System Development 

             This system contains sensors such as DHT11, humidity, soil moisture,Wemos D1 R1 and Ubuntu pre-installed  PC.  All the sensors will be connected to the Wemos D1 R1 using jumper wires and the breadboard. 

             ![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/7b956879-1250-4103-b64b-b1ba47de574f)

The yield voltage of the DHT11 is exactly equivalent to the 5v Centigrade temperature. It has three terminals that range from -55 degrees Celsius to +150 degrees Celsius to calculate temperatures. The temperature sensor (DHT11) output voltage increases by 10mV per degree Celsius and can be worked with less than 60uA of current from a 5V power supply. The soil moisture sensor FC28 is connected to the analog pin A0 with a voltage of 3.3v while the relay module is connected to the pin D8 with the supply of 5v.The purpose of the relay module is to maintain the soil moisture at 25%.

Technology Stack 

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/c69e7011-74f8-4365-aa63-56e28c062ad2)

The steps in this phase can be broken into several components.

● The ESP8266 reads the data from the sensors and sends it to an MQTT broker.
● ESP8266 gets the current moisture of the field and turns on the water pump module only if it is less than 25%. Therefore, this system always keeps the field moisture higher than 25%. 
● Telegraf subscribes to the MQTT topic and saves the received data into the InfluxDB database.
● Grafana reads the data from the InfluxDB database and visualizes it on a custom dashboard.

Running a Mosquitto MQTT server

In this section, the Mosquitto server will be installed and its ports will be exposed to the host machine using Docker.Moreover the installation can be validated by listing the containers. 

Moquitto can be installed by running the official eclipse-mosquitto image and exposing the two working ports.

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/40d734ed-c786-40c8-b30f-4cce0b1aefbf) - Running mqtt image


The -p flag is used to define the ports that you want to expose on the host. The -d flag on the other hand defines that you want to run the container in detached mode.
The installation can be validated by listing all containers and looking at the status field.

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/32cb9f6a-4861-4eae-8fb4-ad9122ef70b7) : Validate installation

Installing and running InfluxDB

Now that you have the Mosquitto MQTT Broker up and running, we will continue by installing InfluxDB and creating a new database.
InfluxDB also provides an official Docker image that makes it easy to deploy their service.

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/7aab08cf-c92b-48de-9483-fdb0285a09c6)- Running InfluxDB

The -v flag defines a volume that will save the database's data even if the container is restarted or destroyed.
After the installation is done, it is time to create the database that will be used to save the data. For that, you will need to use the InfluxDB CLI which you can open using the following command.

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/92d9e610-18b2-4284-987e-1772782d7504)

Opening InfluxDB CLI

we should now be in the InfluxDB CLI and can continue by creating the database and the telegraf user.
 
![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/4ab34e20-4561-4798-8911-8fb103a6558f)- Creating database with password

Here we first create a database named sensors and then create a user called telegraf and grant him full access to the database.

Configuring and running Telegraf

Then continue by installing and configuring Telegraf, a metric collection, and processing agent that collects the data sent to the MQTT broker and stores it in our InfluxDB database.
The first step of the configuration process is to generate the default configuration by starting a Telegraf docker container and copying the config to the host filesystem.

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/9b263331-dbd0-405a-a53f-080c034e1ff0) -Generate telegraf configuration

We should now have a telegraf.conf file in our directory. Here we will have to modify some fields under the inputs.mqtt_consumer tag, which defines the information of your MQTT broker.

 ![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/cbba1135-004b-4021-948a-daf51480bde3) -MQTT Consumer Configuration

The output will be configured under the outputs.influxdb tag which defines the following parameters:

●	The URL of the InfluxDB database
●	The database name
●	Authentication parameters (username, password)

 ![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/11adac3a-ecfe-4c31-86a3-aeea1697a4ca)- InfluxDB Configuration

 ## HTTP Basic Auth
  username = "telegraf"
  password = "telegraf"

After the configuration is finished Telegraf can be started using Docker. The configuration will be transferred into the container using a volume.

 
Running telegraf

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/bf6606e0-20c3-4342-8b96-5b92e31cb114)


Arduino IDE Code

Now that we have successfully set up our Mosquitto MQTT broker, InfluxDB and Telegraf.It is time to send our sensor data. This section will show how to do precisely that using an Wemos D1 R1 microcontroller and sensors.

The following steps need to be executed to successfully send data over MQTT:

●	Connect the microcontroller to Wifi.
●	Connect to the MQTT broker.
●	Read sensor data.
●	Maintain Soil Moisture.
●	Send the data over MQTT.

First,following libraries will be installed to support the sensor readings.Arduino IDE is used to upload and verify the microcontroller codes.

 ![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/cdc9df63-5035-4a19-accf-414e3cc92d1e)


Next, add our WIFI credentials at the top and use them to connect to the internet using the WiFi library.
 
Network Credentials

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/54492211-f947-4cbb-b7a7-a12b58d39f43)

After filling in our wifi credentials, it is time to continue by connecting to the MQTT broker. We will also implement a function to reconnect to the broker if the connection fails.And we can keep the water motor off initially. 

  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, LOW); 

Next,inside the loop function the current temperature,humidity and moisture will be read using the sensors. The moisture level will be converted into a percentage using the following formula.And also if the moisture percentage is less than 25 the motor pin will be HIGH,means it will be turned ON.Else it will remain turned OFF.

 
Irrigation Mechanism

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/4227b1b5-c0c0-4c96-a251-687f9691acab)

The following message will then send the data to the sensors topic.

 
Messages to be sent through mqtt

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/5c56cd20-5c93-4501-a65b-9aa9f04eae9a)


The sensor needs to be initialized using the begin () function and can then be read using the readTemperature(),readHumidity() and analogRead(SensorPin) functions.
After that, we can format the sensor data into a String that is accepted by the InfluxDB database. Then the data will be sent over MQTT using the publish() function on the MQTT client we created above.The finalised code shall be verified and uploaded to the microcontroller.

Running and configuring Grafana

Grafana is an analytics platform and one of the most popular data visualizers out there. Grafana makes it easy to build fully customizable dashboards using data from a wide variety of data sources.

In this section, we will start Grafana using Docker and configure InfluxDB as the data source. Then we will configure our Grafana dashboard by fetching the sensor data from the InfluxDB database.

First, Grafana docker containers will be started and publish port 3000 to the host system.

 
Running Grafana

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/3a028a2e-6ce4-4e43-be50-466981b379d0)

Now we are able to visit Grafana on your localhost:3000 and see the following screen. We can use admin as standard login credentials for the username and password.

Grafana Login Page

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/49fa1379-56e4-4f0b-8f0b-8de701f561b0)

Adding data source

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/13bc8f9b-29fa-4f63-94c2-6646cfc0c131)

Then we shall navigate to the data source page by clicking on Configurations > Data Sources in the side menu.

We can continue by selecting InfluxDB from the time series databases drop-down menu. Now we need to configure the data source by filling in the URL and database fields based on the settings we configured earlier. The URL is a combination of the IP address and port of our InfluxDB API and the database is the database name we set earlier.

 
Grafana Settings

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/28f2020e-b60e-4b07-a91a-9d0bb5262291)


After successfully configuring and saving the data source we can continue by creating a new dashboard. Following steps shall be followed:

1.	Click New dashboard.
2.	Click Add new panel. Grafana creates a basic graph panel that needs to be configured using InfluxDB data.


Now the panel can be configured using InfluxDB data.Weather will be chosen as the measurement and temperature,humidity and moisture as the fields.And the query settings can be changed according to our needs.

 
Data visualization 

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/0a0b4929-bf9f-4b35-b794-25ba6b2adaae)

    Deployment
    
Running the deployment using Docker Compose

Another way to run this whole setup is through Docker compose, which makes the entire process a lot easier and more reusable. 

We can start with the basic structure of the file and the mosquitto container consists of an image and publishes two ports to the host.

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/e45c4e10-b5d2-47d1-8511-16fdcf819c01)

 
Docker compose part 1

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/73636508-0459-4763-9b64-3e80352976c7)


The Telegraf container needs a bind mount to load the configuration into the container, which can be specified using the volumes key.

 
Docker compose part 2

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/fa712fc1-65a5-4e8d-bd6c-cbd44898aae4)

The grafana container will have a normal volume instead which is defined using the volumes keyword at root level.

 
 Docker compose part 3

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/0e73e039-3ef7-4a50-bab5-b63b255970d9)


InfluxDB also sets the same kind of volume to save its data but will also define the database, username and password as environment variables so we don't need to configure it manually as we did above.

 Docker compose part 4

 ![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/19635107-d517-416f-aa21-ab9d67432ef4)


Next, run the Docker compose file using the following command:

 
Running docker-compose 

![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/71c405e7-5c1a-4044-a56c-38f674f9d256)


The running containers can be validated using the following command.

Running Containers
 ![image](https://github.com/Lunaaaaar/Microservices-Based-IoT-for-Smart-Agriculture/assets/149289751/ab4568a6-398e-42c4-a883-ccd4fe89ef94)


































