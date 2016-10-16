import unittest
from parsley import makeGrammar, ParseError

esriGrammar = """
esri = header+:headers datarows:data -> {
    "headers": {k: v for dict in [{d["type"]: d["value"]} for d in headers] for k, v in dict.items()},
    "data": data
}

header = (rows:v -> {"type": "rows", "value": v}) |
         (cols:v -> {"type": "cols", "value": v}) |
         (origin:v -> {"type": v["xy"]+"_origin", "value": {"offset": v["offset"], "corner": v["corner"]}}) |
         (cellsize:v -> {"type": "cellsize", "value": v}) |
         (nodata:v -> {"type": "nodata", "value": v})

rows = 'NROWS ' pnumber:r '\n' -> r
cols = 'NCOLS ' pnumber:c '\n' -> c
origin  = ('X'|'Y'):xy 'LL' corner:c ' ' pnumber:offset '\n' -> {"xy": xy, "offset": offset, "corner": c}
cellsize = 'CELLSIZE ' pnumber:c '\n' -> c
nodata = 'NODATA_VALUE ' number:c '\n' -> c
datarows = datarow+:rows -> list(rows)

datarow = number:first (' ' number)*:rest '\n'{0,1} -> [first] + rest
corner = ('CORNER' -> True) | ('CENTER' -> False)

number = ('-' | -> ''):sign (pnumber:magnitude -> int(int(sign+"1") * magnitude))
pnumber = ((digit1_9:first digits:rest -> first + rest) | digit):magnitude -> int(magnitude)

digit = :x ?(x in '0123456789') -> x
digits = <digit*>
digit1_9 = :x ?(x in '123456789') -> x
"""

esriParser = makeGrammar(esriGrammar, {})

class EsriTest(unittest.TestCase):
    def test_pnumber(self):
        self.assertEqual(esriParser("123").pnumber(), 123)
        self.assertEqual(esriParser("0").pnumber(), 0)
       
        with self.assertRaises(ParseError):
            self.assertEqual(esriParser("-12").pnumber(), -12)

    def test_number(self):
        self.assertEqual(esriParser("123").number(), 123)
        self.assertEqual(esriParser("-12").number(), -12)
        self.assertEqual(esriParser("0").number(), 0)

    def test_rows(self):
        self.assertEqual(esriParser("NROWS 123\n").rows(), 123)
        self.assertEqual(esriParser("NROWS 0\n").rows(), 0)
    
    def test_cols(self):
        self.assertEqual(esriParser("NCOLS 123\n").cols(), 123)
        self.assertEqual(esriParser("NCOLS 0\n").cols(), 0)
    
    def test_origin(self):
        self.assertEqual(esriParser("XLLCORNER 123\n").origin(), {"xy": "X", "offset": 123, "corner": True})
        self.assertEqual(esriParser("XLLCENTER 456\n").origin(), {"xy": "X", "offset": 456, "corner": False})
        self.assertEqual(esriParser("YLLCORNER 123\n").origin(), {"xy": "Y", "offset": 123, "corner": True})

    def test_cellsize(self):
        self.assertEqual(esriParser("CELLSIZE 123\n").cellsize(), 123)
        self.assertEqual(esriParser("CELLSIZE 0\n").cellsize(), 0)
    
    def test_nodata(self):
        self.assertEqual(esriParser("NODATA_VALUE 123\n").nodata(), 123)
        self.assertEqual(esriParser("NODATA_VALUE -12\n").nodata(), -12)
        self.assertEqual(esriParser("NODATA_VALUE 0\n").nodata(), 0)

    def test_datarow(self):
        self.assertEqual(esriParser("1\n").datarow(), [1])
        self.assertEqual(esriParser("1 2 3\n").datarow(), [1, 2, 3])
        self.assertEqual(esriParser("-1 2 -3\n").datarow(), [-1, 2, -3])
        self.assertEqual(esriParser("1").datarow(), [1])
        self.assertEqual(esriParser("1 2").datarow(), [1, 2])
    
    def test_datarows(self):
        self.assertEqual(esriParser("1 2 3\n").datarows(), [[1,2,3]])
        self.assertEqual(esriParser("1 2 3\n4 5 6").datarows(), [[1,2,3], [4,5,6]])
    
    def test_header(self):
        self.assertEqual(esriParser("NROWS 123\n").header(), {"type": "rows", "value": 123})
        self.assertEqual(esriParser("NCOLS 123\n").header(), {"type": "cols", "value": 123})
        self.assertEqual(esriParser("XLLCENTER 123\n").header(), {"type": "X_origin", "value": {"offset": 123, "corner": False}})
        self.assertEqual(esriParser("CELLSIZE 5\n").header(), {"type": "cellsize", "value": 5})
        self.assertEqual(esriParser("NODATA_VALUE -9999\n").header(), {"type": "nodata", "value": -9999})
    
    def test_esri(self):
        esrifile = """\
NCOLS 5
NROWS 5
XLLCORNER 0
YLLCORNER 0
CELLSIZE 1
0 0 9 0 0
0 0 9 0 0
0 0 0 0 0
0 0 9 0 0
0 0 9 0 0
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
