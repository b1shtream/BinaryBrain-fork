import binarybrain as bb
import binarybrain.core as core
from binarybrain.frame_buffer import FrameBuffer

def identity_layer():
    return core.PassThrough_fp32.create()

def Xor(buf1: FrameBuffer, buf2: FrameBuffer) -> FrameBuffer:
    return FrameBuffer.from_core(core.XorOp(buf1.get_core(), buf2.get_core()))

def Or(buf1: FrameBuffer, buf2: FrameBuffer) -> FrameBuffer:
    return FrameBuffer.from_core(core.OrOp(buf1.get_core(), buf2.get_core()))

def XorPrintBuffer(buf1: FrameBuffer, buf2: FrameBuffer) -> FrameBuffer:
    core.XorPrintBuffer(buf1.get_core(), buf2.get_core())









