#datalogger for rocket
from rocket import Rocket
import quaternion as qt
import numpy as np
import csv

def disectXYZ(name: str, coord):
    d = dict()
    d[name+'_x'] = coord[0]
    d[name+'_y'] = coord[1]
    d[name+'_z'] = coord[2]
    return d

def disectQ(name: str, quat : qt.quaternion):
    d = dict()
    d[name+'_w'] = quat.w
    d[name+'_x'] = quat.x
    d[name+'_y'] = quat.y
    d[name+'_z'] = quat.z
    return d


class RocketLogger:
    def __init__(self, rocket: Rocket, filePath: str):
        self.rocket = rocket
        self.path = filePath
        self.file = None
        self.writer: csv.DictWriter | None = None
        self.header_written = False
    
    def open(self):
        if self.file is not None:
            return  # already open
        self.file = open(self.path, "w", newline="", encoding="utf-8")

    def close(self):
        if self.file is None:
            return
        self.file.flush()  
        self.file.close()
        self.file = None
        self.writer = None
        self.header_written = False

    def record(self):
        if self.file is None:
            self.open()

        row = {}
        #record rocket state variables
        row['time'] = self.rocket.global_time
        row = row | disectXYZ('pos_w',self.rocket.x)
        row = row | disectXYZ('vel_w',self.rocket.v)
        row = row | disectXYZ('ang_vel_b',self.rocket.w_body)
        row = row | disectQ('quat_b_to_w',self.rocket.q)
        row = row | disectXYZ('F_net_w',self.rocket.F_net_world)
        row = row | disectXYZ('Tau_b',self.rocket.tau_body)

        #record rocket component state variables
        for component in self.rocket.components:
            componentData = component.get_state_data()
            row = row | componentData

        # create writer + write header the first time
        if self.writer is None:
            self.writer = csv.DictWriter(self.file, fieldnames=list(row.keys()))
        if not self.header_written:
            self.writer.writeheader()
            self.header_written = True

        self.writer.writerow(row)

