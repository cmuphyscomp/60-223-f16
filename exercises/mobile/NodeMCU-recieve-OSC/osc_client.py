"""
This program sends 10 random values between 0.0 and 1.0 to the /filter address,
waiting for 1 seconds between each value.
"""
import argparse
import random
import time

from pythonosc import osc_bundle_builder
from pythonosc import osc_message_builder
from pythonosc import udp_client


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--ip", default="127.0.0.1",
          help="The ip of the OSC server")
    parser.add_argument("--port", type=int, default=8000,
          help="The port the OSC server is listening on")
    args = parser.parse_args()

    client = udp_client.UDPClient(args.ip, args.port)

    state = 1
    while True:
        print("sending msg")
        bundle = osc_bundle_builder.OscBundleBuilder(osc_bundle_builder.IMMEDIATELY)
        msg = osc_message_builder.OscMessageBuilder(address="/led")
        msg.add_arg(state)
        msg = msg.build()
        bundle.add_content(msg)
        bundle = bundle.build()
        client.send(bundle)
        state = 1 - state
        time.sleep(1)
