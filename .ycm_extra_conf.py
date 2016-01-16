import sys, os

here = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.normpath(os.path.join(here, 'ycm')))

import ycmflags

def FlagsForFile( filename ):
    flags = ycmflags.YcmFlags()
    return flags.flags_for_file(filename)
