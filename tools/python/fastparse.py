import unittest

class esriParser(object):
    def __init__(self, source):
        self.data = []
        self.nodata_value = -9999

        lines = source.replace("\r", "").split("\n")
        for line in lines:
            if line == "":
                continue
            if "NROWS" in line.upper():
                self.rows = int(line.split()[-1])
            elif "NCOLS" in line.upper():
                self.cols = int(line.split()[-1])
            elif "XLL" in line.upper():
                self.xll = int(line.split()[-1])
                self.corner = True if "CORNER" in line.upper() else False
            elif "YLL" in line.upper():
                self.yll = int(line.split()[-1])
                self.corner = True if "CORNER" in line.upper() else False
            elif "CELLSIZE" in line.upper():
                self.cellsize = int(line.split()[-1])
            elif "NODATA_VALUE" in line.upper():
                self.nodata_value = float(line.split()[-1])
            else:
                self.data.append([float(element) for element in line.split()])
        self.validate()

    def esri(self):
        return {
            "headers": {
                "cols": self.cols,
                "rows": self.rows,
                "X_origin": {"corner": self.corner, "offset": self.xll},
                "Y_origin": {"corner": self.corner, "offset": self.yll},
                "cellsize": self.cellsize,
                "nodata_value": self.nodata_value,
            },
            "data": self.data
        }

    def validate(self):
        assert self.rows == len(self.data), "NROWS not equal to row count: %d != %d" %(self.rows, len(self.data))
        assert self.cols == len(self.data[0]), "NCOLS not equal to col count: %d != %d" %(self.rows, len(self.data[0]))
        assert type(self.xll) == type(1)
        assert type(self.yll) == type(1)
        assert type(self.corner == type(1))
        assert type(self.cellsize == type(1))
        assert type(self.nodata_value == type(1.0))

class EsriTest(unittest.TestCase):
    def test_esri(self):
        esrifile = """\
NCOLS 5
NROWS 5
XLLCORNER 0
YLLCORNER 0
CELLSIZE 1
0.0 0.0 9.0 0.0 0.0
0.0 0.0 9.0 0.0 0.0
0.0 0.0 0.0 0.0 0.0
0.0 0.0 9.0 0.0 0.0
0.0 0.0 9.0 0.0 0.0
"""
        data = [
            [0, 0, 9, 0, 0],
            [0, 0, 9, 0, 0],
            [0, 0, 0, 0, 0],
            [0, 0, 9, 0, 0],
            [0, 0, 9, 0, 0],
        ]
        esri = esriParser(esrifile).esri()

        self.assertEqual(esri["headers"]["cols"], 5)
        self.assertEqual(esri["headers"]["rows"], 5)
        self.assertEqual(esri["headers"]["X_origin"]["corner"], True)
        self.assertEqual(esri["headers"]["Y_origin"]["offset"], 0)
        self.assertEqual(esri["headers"]["rows"], 5)
        self.assertEqual(esri["headers"]["cellsize"], 1)
        self.assertEqual(esri["data"], data)

if __name__ == '__main__':
    unittest.main()
