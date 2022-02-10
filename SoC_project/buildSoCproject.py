#!/usr/bin/env python3

from migen import *
from migen.genlib.io import CRG
from migen.genlib.cdc import MultiReg

import nexys4ddr as tarjeta
#import c4e6e10 as tarjeta

from litex.soc.integration.soc_core import *
from litex.soc.integration.builder import *
from litex.soc.interconnect.csr import *

from litex.soc.cores import gpio
from litex.soc.cores import uart
from litex.soc.cores import bitbang
from module import rgbled
from module import sevensegment
from module import vgacontroller

#New modules
from module import servo
from module import motores
from module import infrarrojo
from module import ultrasonido

# BaseSoC ------------------------------------------------------------------------------------------

class BaseSoC(SoCCore):
	def __init__(self):
		platform = tarjeta.Platform()

		# Verilog sources
		platform.add_source("module/verilog/servo.v")
		platform.add_source("module/verilog/motores.v")
		platform.add_source("module/verilog/infrarrojo.v")
		platform.add_source("module/verilog/ultrasonido.v")
		platform.add_source("module/verilog/divFreq.v")


		# SoC with CPU
		SoCCore.__init__(self, platform,
 			cpu_type="picorv32",
#			cpu_type="vexriscv",
			clk_freq=100e6,
			integrated_rom_size=0x8000,
			integrated_sram_size=0x4000,
			csr_paging=0x800,
			csr_ordering= "big",
			integrated_main_ram_size=18*1024)

		# Clock Reset Generation
		self.submodules.crg = CRG(platform.request("clk"), ~platform.request("cpu_reset"))

		# Leds
		SoCCore.add_csr(self,"leds")
		user_leds = Cat(*[platform.request("led", i) for i in range(10)])
		self.submodules.leds = gpio.GPIOOut(user_leds)
		
		# Switchs
		SoCCore.add_csr(self,"switchs")
		user_switchs = Cat(*[platform.request("sw", i) for i in range(8)])
		self.submodules.switchs = gpio.GPIOIn(user_switchs)
		
		# Buttons
		SoCCore.add_csr(self,"buttons")
		user_buttons = Cat(*[platform.request("btn%c" %c) for c in ['c','r','l']])
		self.submodules.buttons = gpio.GPIOIn(user_buttons)
		
		# 7segments Display
		SoCCore.add_csr(self,"display")
		display_segments = Cat(*[platform.request("display_segment", i) for i in range(8)])
		display_digits = Cat(*[platform.request("display_digit", i) for i in range(8)])
		self.submodules.display = sevensegment.SevenSegment(display_segments,display_digits)

		# RGB leds
		SoCCore.add_csr(self,"ledRGB_1")
		self.submodules.ledRGB_1 = rgbled.RGBLed(platform.request("ledRGB",1))
		
		SoCCore.add_csr(self,"ledRGB_2")
		self.submodules.ledRGB_2 = rgbled.RGBLed(platform.request("ledRGB",2))
		
				
		# VGA
		SoCCore.add_csr(self,"vga_cntrl")
		vga_red = Cat(*[platform.request("vga_red", i) for i in range(4)])
		vga_green = Cat(*[platform.request("vga_green", i) for i in range(4)])
		vga_blue = Cat(*[platform.request("vga_blue", i) for i in range(4)])
		self.submodules.vga_cntrl = vgacontroller.VGAcontroller(platform.request("hsync"),platform.request("vsync"), vga_red, vga_green, vga_blue)

		# --------- New drivers --------- #
		#Motores
		SoCCore.add_csr(self, "mt_driver")
		IN = Cat(*[platform.request("IN", i) for i in range(4)])
		self.submodules.mt_driver = motores.mt(IN)

		# Infrarrojo
		SoCCore.add_csr(self, "ir_driver")
		#Definicion de pines I/O
		self.submodules.ir_driver = infrarrojo.ir(platform.request("iR"), platform.request(
			"iRC"), platform.request("iC"), platform.request("iLC"), platform.request("iL"))
		
		#Servo
		SoCCore.add_csr(self, "servo_driver")
		self.submodules.servo_driver = servo.servoUS(platform.request("pwm"))

		#Ultrasonido
		SoCCore.add_csr(self,"us_driver")
		self.submodules.us_driver = ultrasonido.us(platform.request("echo"),platform.request("trig"))

		#UART BLUETHOOT
		SoCCore.add_csr(self, "uart_bt_phy")
		SoCCore.add_csr(self, "uart_bt")
		self.submodules.uart_bt_phy = uart.UARTPHY(
			pads=platform.request("uart_bt"),
			clk_freq=self.sys_clk_freq,
			baudrate=9600)

		self.submodules.uart_bt = ResetInserter()(uart.UART(self.uart_bt_phy,
                                                    tx_fifo_depth=16,
                                                    rx_fifo_depth=16))
		self.csr.add("uart_bt_phy", use_loc_if_exists=True)
		self.csr.add("uart_bt", use_loc_if_exists=True)
		if hasattr(self.cpu, "interrupt"):
			self.irq.add("uart_bt", use_loc_if_exists=True)
		else:
			self.add_constant("UART_POLLING")

		
		#I2C
		SoCCore.add_csr(self,"i2c_master")
		self.submodules.i2c_master = bitbang.I2CMaster()
		


# Build --------------------------------------------------------------------------------------------
if __name__ == "__main__":
	builder = Builder(BaseSoC(), csr_csv="Soc_MemoryMap.csv")
	builder.build()

