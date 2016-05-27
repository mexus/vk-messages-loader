import sys, os

base_path = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.normpath(os.path.join(base_path, 'ycm')))

import ycmflags

def FlagsForFile( filename ):
    flags = ycmflags.YcmFlags(default_file = ["storage/src/exceptions.cpp", []],
            absolute_project_path = base_path)
    return flags.flags_for_file(filename)
