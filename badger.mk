badger_INC_DIRS := $(shell find $(BADGER_DIR)/include -type d -not -path "*/.svn*")
badger_INCLUDE  := $(foreach i, $(badger_INC_DIRS), -I$(i))

libmesh_INCLUDE := $(badger_INCLUDE) $(libmesh_INCLUDE)

badger_LIB := $(BADGER_DIR)/libbadger-$(METHOD).la

badger_APP := $(BADGER_DIR)/badger-$(METHOD)

# source files
badger_srcfiles    := $(shell find $(BADGER_DIR)/src -name "*.C" -not -name main.C)
badger_csrcfiles   := $(shell find $(BADGER_DIR)/src -name "*.c")
badger_fsrcfiles   := $(shell find $(BADGER_DIR)/src -name "*.f")
badger_f90srcfiles := $(shell find $(BADGER_DIR)/src -name "*.f90")

# object files
badger_objects := $(patsubst %.C, %.$(obj-suffix), $(badger_srcfiles))
badger_objects += $(patsubst %.c, %.$(obj-suffix), $(badger_csrcfiles))
badger_objects += $(patsubst %.f, %.$(obj-suffix), $(badger_fsrcfiles))
badger_objects += $(patsubst %.f90, %.$(obj-suffix), $(badger_f90srcfiles))

# plugin files
badger_plugfiles    := $(shell find $(BADGER_DIR)/plugins/ -name "*.C" 2>/dev/null)
badger_cplugfiles   := $(shell find $(BADGER_DIR)/plugins/ -name "*.c" 2>/dev/null)
badger_fplugfiles   := $(shell find $(BADGER_DIR)/plugins/ -name "*.f" 2>/dev/null)
badger_f90plugfiles := $(shell find $(BADGER_DIR)/plugins/ -name "*.f90" 2>/dev/null)

# plugins
badger_plugins := $(patsubst %.C, %-$(METHOD).plugin, $(badger_plugfiles))
badger_plugins += $(patsubst %.c, %-$(METHOD).plugin, $(badger_cplugfiles))
badger_plugins += $(patsubst %.f, %-$(METHOD).plugin, $(badger_fplugfiles))
badger_plugins += $(patsubst %.f90, %-$(METHOD).plugin, $(badger_f90plugfiles))

# badger main
badger_main_src    := $(BADGER_DIR)/src/main.C
badger_app_objects := $(patsubst %.C, %.$(obj-suffix), $(badger_main_src))

# dependency files
badger_deps := $(patsubst %.C, %.$(obj-suffix).d, $(badger_srcfiles)) \
              $(patsubst %.c, %.$(obj-suffix).d, $(badger_csrcfiles)) \
              $(patsubst %.C, %.$(obj-suffix).d, $(badger_main_src))

# If building shared libs, make the plugins a dependency, otherwise don't.
ifeq ($(libmesh_shared),yes)
  badger_plugin_deps := $(badger_plugins)
else
  badger_plugin_deps :=
endif

all:: $(badger_LIB)

$(badger_LIB): $(badger_objects) $(badger_plugin_deps)
	@echo "Linking "$@"..."
	@$(libmesh_LIBTOOL) --tag=CXX $(LIBTOOLFLAGS) --mode=link --quiet \
	  $(libmesh_CXX) $(libmesh_CXXFLAGS) -o $@ $(badger_objects) $(libmesh_LIBS) $(libmesh_LDFLAGS) $(EXTERNAL_FLAGS) -rpath $(BADGER_DIR)
	@$(libmesh_LIBTOOL) --mode=install --quiet install -c $(badger_LIB) $(BADGER_DIR)

# include BADGER dep files
-include $(badger_deps)

# how to build BADGER application
ifeq ($(APPLICATION_NAME),badger)
all:: badger

badger: $(badger_APP)

$(badger_APP): $(moose_LIB) $(elk_MODULES) $(badger_LIB) $(badger_app_objects)
	@echo "Linking "$@"..."
	@$(libmesh_LIBTOOL) --tag=CXX $(LIBTOOLFLAGS) --mode=link --quiet \
          $(libmesh_CXX) $(libmesh_CXXFLAGS) -o $@ $(badger_app_objects) $(badger_LIB) $(elk_MODULES) $(moose_LIB) $(libmesh_LIBS) $(libmesh_LDFLAGS) $(ADDITIONAL_LIBS)

endif

#
# Maintenance
#
delete_list := $(badger_APP) $(badger_LIB) $(BADGER_DIR)/libbadger-$(METHOD).*

cleanall:: 
	make -C $(BADGER_DIR) clean 

###############################################################################
# Additional special case targets should be added here
