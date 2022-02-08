from migen import *
from migen.genlib.cdc import MultiReg
from litex.soc.interconnect.csr import *
from litex.soc.interconnect.csr_eventmanager import *

class mt(Module, AutoCSR):
    def __init__(self, IN):
            self.clk = ClockSignal()
            self.movimiento = CSRStorage(3)
            self.IN = IN


            self.specials += Instance("motores",
                i_clk = self.clk,
                i_movimiento = self.movimiento.storage,
                o_IN = self.IN)
