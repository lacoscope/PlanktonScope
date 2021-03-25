from loguru import logger
import multiprocessing
import os
import time

from .Adafruit_ADS1x15.ADS1x15 import ADS1115

import planktoscope.mqtt
import planktoscope.light

logger.info("planktoscope.sensors is loaded")

class SensorsProcess(multiprocessing.Process):
    
    __ph4=14700     #valeurs par défaut de calibration
    __ph7=10100
    

    def __init__(self, event):
        super(SensorsProcess, self).__init__()
        logger.info("Initialising the sensors process")
        
        self.adc = ADS1115()
        
        self.stop_event = event
        logger.info(f"Sensors initialisation is over")

    def __message_sensors(self, last_message):
        logger.debug("We have received a sensors command")
        if last_message["action"] == "ph":
            logger.debug("We have received a PH command")
            self.mesure_ph()
        else:
            logger.warning(f"The received message was not understood {last_message}")
    
    def __message_calibration(self, last_message):
        logger.debug("We have received a calibration command")
        if last_message["action"] == "calibration_ph":
            ph=last_message["ph"]
            mesure=self.adc.read_adc(0,gain=1)
            self.calibration(ph,mesure)
        else:
            logger.warning(f"The received message was not understood {last_message}")
    
    def calibration(self,ph,mesure):
        if ph==4:
            self.__ph4=mesure
        if ph==7:
            self.__ph7=mesure
        logger.debug("Calibration ok ph:{ph},valeur:{mesure}")
    
    def mesure_ph(self):
        mes=self.adc.read_adc(0,gain=1)
        mes=(((mes-self.__ph4)*3)/(self.__ph7-self.__ph4))+4
        self.sensor_client.client.publish("sensors/data",str(round(mes,2)))
        logger.debug("Mesure ph:",mes)
        return mes

    def treat_command(self):
        command = ""
        logger.info("We received a new message")
        last_message = self.sensor_client.msg["payload"]
        logger.debug(last_message)
        command = self.sensor_client.msg["topic"].split("/", 1)[1]
        logger.debug(command)
        self.sensor_client.read_message()

        if command == "measure":
            self.__message_sensors(last_message)
        if command == "calibration":
            self.__message_calibration(last_message)
        elif command != "":
            logger.warning(f"We did not understand the received request {command} - {last_message}")
	
    
    @logger.catch
    def run(self):
        """This is the function that needs to be started to create a thread"""
        logger.info(
            f"The sensors control process has been started in process {os.getpid()}"
        )

        # Creates the MQTT Client
        # We have to create it here, otherwise when the process running run is started
        # it doesn't see changes and calls made by self.sensor_client because this one
        # only exist in the master process
        # see https://stackoverflow.com/questions/17172878/using-pythons-multiprocessing-process-class
        self.sensor_client = planktoscope.mqtt.MQTT_Client(topic="sensors/#", name="sensor_client")
        # Publish the status "Ready" to via MQTT to Node-RED
        self.sensor_client.client.publish("status/sensors", '{"status":"Ready"}')

        logger.success("Sensors are READY!")
        delay = 0.001
        while not self.stop_event.is_set():
            if self.sensor_client.new_message_received():
                self.treat_command()
            time.sleep(delay)
            delay = 0.001
        logger.info("Shutting down the sensors process")
        self.sensor_client.client.publish("status/sensors", '{"status":"Dead"}')
        self.sensor_client.shutdown()
        logger.success("Sensors process shut down! See you!")
