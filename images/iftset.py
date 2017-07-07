#! /usr/local/bin/python

import os
import subprocess

samples = "corel/samples/"
target = "corel/superpixels/"
program = "../isf_demo/bin/iftISF_MIX_MEAN"
program_flags = "150 0.12 12 10 0"
file_list = os.listdir(samples)
for filename in file_list:
    (filename_name, filename_ext) = os.path.splitext(filename)
    if filename_ext == ".png":
        command_to_run = program + ' ' + samples + filename + ' ' + program_flags + ' ' + target + filename_name
        print (command_to_run)
        os.system(command_to_run)
