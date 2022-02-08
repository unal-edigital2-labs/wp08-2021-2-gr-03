from migen import *
from migen.genlib.cdc import MultiReg
from litex.soc.interconnect.csr import *
from litex.soc.interconnect.csr_eventmanager import *

<<<<<<< HEAD


class mt(Module, AutoCSR):
    def __init__(self, IN):
            self.clk = ClockSignal()
            self.movimiento = CSRStorage(3)
            self.IN = IN


            self.specials += Instance("motores",
                i_clk = self.clk,
                i_movimiento = self.movimiento.storage,
                o_IN = self.IN)
=======
# A - AVANCE
# R - RETROCESO
# P - PAUSA
# GD - GIRO DERECHA
# GI - GIRO IZQUIERDA

class Motores(Module, AutoCSR):
    def __init__(self, A, R, P, GD, GI):
            self.clk = ClockSignal()
            self.state = CSRStorage(3)
            self.A = A
            self.R = R
            self.P = P
            self.GD = GD
            self.GI = GI

            self.specials += Instance("motores",
                i_clk = self.clk,
                i_state = self.state.storage,
                o_A = self.A,
                o_R = self.R,
                o_left = self.P,
                o_left = self.GD,
                o_left = self.GI)

>>>>>>> 4eb3175963b5bfa75e42dc1ae14aebca4303fc3a
