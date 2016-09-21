"""Small example OSC server

This program listens to several addresses, and prints some information about
received packets.

Example edited on 9-20-16 by Joseph Paetz for use in Intro to Physical
    Computing (60-223) at Carnegie Mellon University
"""
import argparse
import math

from pythonosc import dispatcher
from pythonosc import osc_server

# this is the hanlder we will use with the /button address later
# this is the input argumets that are expected
def printButton(unused_addr, data):
    print("The button at address " + str(unused_addr) + " is " + str(data))

if __name__ == "__main__":
    # REPLACE WITH YOUR COMPUTER'S IP
    ip = "127.0.0.1" 

    # This should match the outPort in the Arduino sketch
    port = 9999

    # these lines set things up to recieve input from the command line
    #  that will over write the default ip and port above
    parser = argparse.ArgumentParser()
    parser.add_argument("--ip", default=ip, help="The ip to listen on")
    parser.add_argument("--port", type=int, default=port, 
                        help="The port to listen on")
    args = parser.parse_args()

    # set up the dispatcher to handle the osc
    dispatcher = dispatcher.Dispatcher()

    # Send the address and data of an osc message to a function 
    #  of your choice. You can use the same function for multiple addresses.
    #  Check out the examples at https://github.com/attwad/python-osc
    #  for more info on what you can do with the dispatchers
    dispatcher.map("/button", printButton)
    dispatcher.map("/text", print)

    # Start the server to recieve osc
    server = osc_server.ThreadingOSCUDPServer(
        (args.ip, args.port), dispatcher)
    print("Serving on {}".format(server.server_address))
    server.serve_forever()

    # no commands below the serve_forever() line will execute
