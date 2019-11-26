#!/usr/bin/env python3
"""
Module Docstring
"""

import amf

__author__ = "Your Name"
__version__ = "0.1.0"
__license__ = "MIT"


def main():
    """ Main entry point of the app """
    amf_tree = amf.AMF("cube_group.AMF")
    for instance in amf_tree.instances:
        print("Current Instance: " + str(instance.id) 
                + ", dx=" + str(instance.delta_x) + ", dy=" + str(instance.delta_y) + ", dz=" + str(instance.delta_z))



if __name__ == "__main__":
    """ This is executed when run from the command line """
    main()