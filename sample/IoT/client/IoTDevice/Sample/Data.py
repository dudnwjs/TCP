# automatically generated by the FlatBuffers compiler, do not modify

# namespace: Sample

import flatbuffers

class Data(object):
    __slots__ = ['_tab']

    @classmethod
    def GetRootAsData(cls, buf, offset):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, offset)
        x = Data()
        x.Init(buf, n + offset)
        return x

    # Data
    def Init(self, buf, pos):
        self._tab = flatbuffers.table.Table(buf, pos)

    # Data
    def Name(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(4))
        if o != 0:
            return self._tab.String(o + self._tab.Pos)
        return bytes()

    # Data
    def Value(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(6))
        if o != 0:
            return self._tab.Get(flatbuffers.number_types.Int16Flags, o + self._tab.Pos)
        return 0

def DataStart(builder): builder.StartObject(2)
def DataAddName(builder, name): builder.PrependUOffsetTRelativeSlot(0, flatbuffers.number_types.UOffsetTFlags.py_type(name), 0)
def DataAddValue(builder, value): builder.PrependInt16Slot(1, value, 0)
def DataEnd(builder): return builder.EndObject()
