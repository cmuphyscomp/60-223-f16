"""
This program sends 10 random values between 0.0 and 1.0 to the /filter address,
waiting for 1 seconds between each value.

Example edited on 9-20-16 by Joseph Paetz for use in Intro to Physical
    Computing (60-223) at Carnegie Mellon University
"""
import argparse
import random
import time

from pythonosc import osc_bundle_builder
from pythonosc import osc_message_builder
from pythonosc import udp_client


if __name__ == "__main__":
    # REPLACE WITH YOUR NODEMCU's IP
    ip = "127.0.0.1" 

    # This should match the localPort variable in the Arduino sketch
    port = 8888

    # these lines set things up to recieve input from the command line
    #  that will over write the default ip and port above
    parser = argparse.ArgumentParser()
    parser.add_argument("--ip", default=ip, help="The ip of the OSC server")
    parser.add_argument("--port", type=int, default=port,
                        help="The port the OSC server is listening on")
    args = parser.parse_args()

    # set up udp client
    client = udp_client.UDPClient(args.ip, args.port)

    LED_state = 1
    while True:
        print("sending msg: " + str(LED_state))

        # build the bundle to send to the NodeMCU
        bundle = osc_bundle_builder.OscBundleBuilder(osc_bundle_builder.IMMEDIATELY)

        # build the message that will go in the bundle
        # this is the address that the NodeMCU will be watching for
        msg = osc_message_builder.OscMessageBuilder(address="/led")

        # add the data to the message
        msg.add_arg(LED_state)
        msg = msg.build()

        # add the message to the bundle
        bundle.add_content(msg)
        bundle = bundle.build()

        #send the bundle
        client.send(bundle)

        # if LED_state = 1, change to 0 and vice versa
        LED_state = 1 - LED_state

        # wait 1 second
        time.sleep(1)
