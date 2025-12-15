
#interpolating lookup table
class LookupTable:
    def __init__(self, data: dict = None):
        self._data = data.copy() if data is not None else {} # stores the lookup table data 
    
    def get(self, key):
        if key in self.data: return self.data[key]
    
    
