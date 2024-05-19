#-------------------------------------------------------------------------------
.SUFFIXES:
#-------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

TOPDIR ?= $(CURDIR)

#-------------------------------------------------------------------------------
# APP_NAME sets the long name of the application.
# APP_SHORTNAME sets the short name of the application.
# APP_AUTHOR sets the author of the application.
# APP_VERSION sets the version of the application.
#-------------------------------------------------------------------------------
APP_NAME		:=  Wii U Account Swap
APP_SHORTNAME	:=  Wii U Account Swap
APP_AUTHOR		:=  Nightkingale
APP_VERSION		:=	v2.0.2

include $(DEVKITPRO)/wut/share/wut_rules

#-------------------------------------------------------------------------------
# DEBUG sets the debug flag for the application.
# This should be 0 for release builds, and 1 for development/workflow builds.
# * The version string will be appended with the git hash.
# * Compiling will produce verbose logs.
#-------------------------------------------------------------------------------
DEBUG := 0

# This appends the git hash to the version string.
ifeq ($(DEBUG), 1)
	GIT_HASH := $(shell git rev-parse --short HEAD)
	APP_VERSION := $(APP_VERSION)-$(GIT_HASH)
endif

#-------------------------------------------------------------------------------
# TARGET is the name of the output.
# BUILD is the directory where object files & intermediate files will be placed.
# SOURCES is a list of directories containing source code.
# DATA is a list of directories containing data files.
# INCLUDES is a list of directories containing header files.
# CONTENT is the path to the bundled folder that will be mounted as /vol/content/.
# ICON is the game icon, leave blank to use default rule.
# TV_SPLASH is the image displayed during bootup on the TV, leave blank to use default rule.
# DRC_SPLASH is the image displayed during bootup on the DRC, leave blank to use default rule.
#-------------------------------------------------------------------------------
TARGET		:=	Wii_U_Account_Swap
BUILD		:=	build
SOURCES		:=	source
DATA		:=	data
INCLUDES	:=	include
ICON		:=	assets/icon.png
TV_SPLASH	:=	assets/tv_splash.png
DRC_SPLASH	:=	assets/drc_splash.png

#-------------------------------------------------------------------------------
# options for code generation
#-------------------------------------------------------------------------------
CFLAGS	:=	-Wall -Wextra -Wundef -Wshadow -Wpointer-arith -Wcast-align \
			-O2 -fipa-pta -pipe -ffunction-sections \
			$(MACHDEP)

CFLAGS	+=	$(INCLUDE) -D__WIIU__ -D__WUT__ -DAPP_VERSION=\"$(APP_VERSION)\"

CXXFLAGS	:= $(CFLAGS)

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-g $(ARCH) $(RPXSPECS) -Wl,-Map,$(notdir $*.map)

LIBS := -lSDL2_mixer -lSDL2 -lSDL2_ttf -lfreetype -lharfbuzz -lfreetype -lpng -lbz2 \
		-lz -lmodplug -lmpg123 -logg -lmocha -lwut

#-------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level
# containing include and lib
#-------------------------------------------------------------------------------
LIBDIRS	:= $(PORTLIBS) $(WUT_ROOT) $(WUT_ROOT)/usr

#-------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#-------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#-------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)
export TOPDIR	:=	$(CURDIR)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

#-------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#-------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#-------------------------------------------------------------------------------
	export LD	:=	$(CC)
#-------------------------------------------------------------------------------
else
#-------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#-------------------------------------------------------------------------------
endif
#-------------------------------------------------------------------------------

export OFILES_BIN	:=	$(addsuffix .o,$(BINFILES))
export OFILES_SRC	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export OFILES 	:=	$(OFILES_BIN) $(OFILES_SRC)
export HFILES_BIN	:=	$(addsuffix .h,$(subst .,_,$(BINFILES)))

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

ifneq (,$(strip $(CONTENT)))
	export APP_CONTENT := $(TOPDIR)/$(CONTENT)
endif

ifneq (,$(strip $(ICON)))
	export APP_ICON := $(TOPDIR)/$(ICON)
else ifneq (,$(wildcard $(TOPDIR)/$(TARGET).png))
	export APP_ICON := $(TOPDIR)/$(TARGET).png
else ifneq (,$(wildcard $(TOPDIR)/icon.png))
	export APP_ICON := $(TOPDIR)/icon.png
endif

ifneq (,$(strip $(TV_SPLASH)))
	export APP_TV_SPLASH := $(TOPDIR)/$(TV_SPLASH)
else ifneq (,$(wildcard $(TOPDIR)/tv-splash.png))
	export APP_TV_SPLASH := $(TOPDIR)/tv-splash.png
else ifneq (,$(wildcard $(TOPDIR)/splash.png))
	export APP_TV_SPLASH := $(TOPDIR)/splash.png
endif

ifneq (,$(strip $(DRC_SPLASH)))
	export APP_DRC_SPLASH := $(TOPDIR)/$(DRC_SPLASH)
else ifneq (,$(wildcard $(TOPDIR)/drc-splash.png))
	export APP_DRC_SPLASH := $(TOPDIR)/drc-splash.png
else ifneq (,$(wildcard $(TOPDIR)/splash.png))
	export APP_DRC_SPLASH := $(TOPDIR)/splash.png
endif

.PHONY: $(BUILD) clean all

#-------------------------------------------------------------------------------
all: $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) -C $(BUILD) -f $(CURDIR)/Makefile V=$(DEBUG)

#-------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).wuhb $(TARGET).rpx $(TARGET).elf

#-------------------------------------------------------------------------------
else
.PHONY:	all

DEPENDS	:=	$(OFILES:.o=.d)

#-------------------------------------------------------------------------------
# main targets
#-------------------------------------------------------------------------------
all	:	$(OUTPUT).wuhb

$(OUTPUT).wuhb	:	$(OUTPUT).rpx
$(OUTPUT).rpx	:	$(OUTPUT).elf
$(OUTPUT).elf	:	$(OFILES)

$(OFILES_SRC)	: $(HFILES_BIN)

#-------------------------------------------------------------------------------
# you need a rule like this for each extension you use as binary data
#-------------------------------------------------------------------------------
%.bin.o	%_bin.h :	%.bin
#-------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#-------------------------------------------------------------------------------
%.ttf.o	%_ttf.h :	%.ttf
#-------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#-------------------------------------------------------------------------------
%.ogg.o	%_ogg.h :	%.ogg
#-------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#-------------------------------------------------------------------------------
endif
#-------------------------------------------------------------------------------