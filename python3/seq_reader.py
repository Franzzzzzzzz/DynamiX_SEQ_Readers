import os, json 
import numpy as np 

def load_seq(filename, varian=False):
    """Load an SEQ file and related logfile, reshaping the file into a 3D array if a logfile is present.

    Args:
        filename (str): location of SEQ/log file to load. Can be the full path to the SEQ or log file including or excluding the file format.
        varian (bool): Was this recorded using the Varian software?

    Returns:
        A 3D array with dimensions [nt,nx,ny]
    """
    if (filename[-3:] == "seq") or (filename[-3:] == "log"):
        filename = filename[:-4]  # strip ending if present
    
    print(filename) ; 
    
    with open(filename + ".seq", "rb") as f:
        if varian:
            data = np.memmap(f, dtype="H", mode="r", offset=2048)
            try:
                data = data.reshape(-1, 960 * 2, 768 * 2)
            except:
                try:
                    data = data.reshape(-1, 960, 768)
                except:
                    pass
            logfile = {}  # TODO
        else:
            data = np.memmap(f, dtype="H", mode="r")
            if os.path.exists(filename + ".log"):
                with open(filename + ".log") as g:
                    try:
                        logfile = json.load(g)

                        nt = len(logfile["detector"]["frames"])
                        nx = logfile["detector"]["image_size"]["height"]  # in landscape mode by default
                        ny = logfile["detector"]["image_size"]["width"]
                        try:
                            data = data.reshape(nt, nx, ny)
                        except:
                            data = data.reshape(-1, nx, ny)
                            print("WARNING: WRONG NUMBER OF FRAMES IN LOG FILE. THIS IS VERY BAD.")
                        if logfile["detector"]["rotate"] == 0:  # landscape
                            pass
                        if logfile["detector"]["rotate"] >= 1:  # portrait
                            data = np.rot90(data, axes=(2, 1))
                        if logfile["detector"]["rotate"] >= 2:  # landscape
                            data = np.rot90(data, axes=(2, 1))
                        if logfile["detector"]["rotate"] == 3:  # portrait
                            data = np.rot90(data, axes=(2, 1))
                    except:
                        # for line in g:
                        # if line.find('frames') != -1: nt = 0
                        try:
                            data = data.reshape(-1, 960, 768)
                            print(
                                """WARNING: Haven't implemented old log file checking, assumed no ROI and 2x2 binning so this may be garbage. Try using pynamix.io.upgrade_logfile() to upgrade your old log file."""
                            )
                        except:
                            raise Exception(
                                "Could not find a log file and also could not load your file."
                            )
                        logfile = {}

            else:
                raise Exception("No log file found!")
    return data, logfile
