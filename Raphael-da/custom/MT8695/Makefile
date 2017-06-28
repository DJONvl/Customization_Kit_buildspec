ifeq ($(DA_PL),yes)
O_TARGET	:= $(.BBCHIP)_custom_PL.o
obj-y 		:= DA_PL/DA_PL_lib.o
subdir-y 	:= DA_PL
else
O_TARGET	:= $(.BBCHIP)_custom_BR.o
obj-y 		:= DA_BR/DA_BR_lib.o
subdir-y 	:= DA_BR
endif

obj-y 		+= nand/nand_lib.o
subdir-y	+= nand

obj-y     += cust_key.o



include $(TOPDIR)/Rules.make
