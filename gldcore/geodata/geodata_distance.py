"""GridLAB-D Geodata Distance Package

The distance package computes the shortest distance between consecutive
positions.

OPTIONS

    "--units=<unit>" specifies the units in which to compute the distance.
    Valid units are "meters", "m", "kilometers", "km", "miles", "mi",
    "yards", "yd", "ft", or "feet". The default is "meters".

CONFIGURATION

    "--method=haversine" specifies the method used to calculate distances.
    The default 'haversine' method calculates great circle distances.
"""

version = 1 # specify API version

import sys
import json
import math, numpy
from pandas import DataFrame
import haversine

#
# Defaults
#
default_options = {
    "units" : "meters",
    "relative" : False,
}

default_config = {
    "method" : "haversine",
}

valid_units = {
    "m" : 1.0,
    "meters" : 1.0,
    "km" : 1e-3,
    "kilometers" : 1e-3,
    "mi" : 0.000621371,
    "miles" : 0.000621371,
    "yards" : 1.09361296,
    "yd" : 1.09361296,
    "ft" : 3.28083888,
    "feet" : 3.28083888,
}
#
# Implementation of address package
#
def apply(data, options=default_options, config=default_config, warning=print):
    """Perform distance calculation at locations in data

    ARGUMENTS:

        data (pandas.DataFrame)

            The data frame must contain `latitude` and `longitude` fields between which
            distances will be computed.

        options (dict)

            "units" specifies the units in which distances are measured.  Valid units
            are ["meters","m"], ["kilometers","km"], ["feet","ft"], ["yards","yd",
            and ["miles","mi"].

        config (dict)

            There are no configuration options

    RETURNS:

        pandas.DataFrame

            The first (and only) return value is the `data` data frame with either the
            `distance` fields updated/added for consecutive fields.

    """

    # convert lat,lon to address
    try:
        path = list(zip(data[config["column_names"]["LAT"]],data[config["column_names"]["LON"]],data[config["column_names"]["ID"]]))
    except Exception as err:
        path = None
    if type(path) == type(None):
        raise Exception("distance calculation requires 'latitude', 'longitude', and 'id' fields")
    if len(path) == 0:
        dist = []
    else:
        dist = [0.0]
        pos1 = path[0]
        lat1 = pos1[0]
        lon1 = pos1[1]
        if config["method"] == "haversine":
            for pos2 in path[1:]:
                id = pos2[2]
                lat2 = pos2[0]
                lon2 = pos2[1]
                d = haversine.haversine([lat1,lon1],[lat2,lon2],unit=haversine.Unit.METERS)
                if options["relative"]:
                    if math.isnan(id):
                        dist.append(d)
                    else:
                        lat1 = lat2
                        lon1 = lon2
                        dist.append(0.0)
                else:
                    lat1 = lat2
                    lon1 = lon2
                    dist.append(d+dist[-1])
        else:
            raise Exception(f"method '{config[method]}' is not recognized")
        try:
            global valid_units
            data[config["column_names"]["DIST"]] = (numpy.array(dist) * valid_units[options["units"]]).round(options["precision"]["distance"])
        except:
            raise Exception(f"unit '{options['units']}' is not recognized")
    return data

#
# Perform validation tests
#
if __name__ == '__main__':

    import unittest

    class TestDistance(unittest.TestCase):

        def test_distance(self):
            test = DataFrame({
                "latitude" : [37.4205,37.5205],
                "longitude" : [-122.2046,-122.3046],
                })
            result = apply(test)
            self.assertEqual(result["distance"][1],12604.0)

    unittest.main()
