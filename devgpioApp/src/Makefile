TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library

LIBRARY_IOC += devledcontrol

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxxRecord
# install devgpio.dbd into <top>/dbd
DBD += devgpiocontrol.dbd

# specify all source files to be compiled and added to the library
devledcontrol_SRCS += devGpioMbbo.c devGpio.cpp devGpioManager.cpp GpioConst.cpp GpioIntHandler.cpp

devledcontrol_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

