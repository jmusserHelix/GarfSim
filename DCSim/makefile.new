OBJDIR = $(DCsim_HOME)/Object
SRCDIR = $(DCsim_HOME)/Source
INCDIR = $(DCsim_HOME)/Include
LIBDIR = $(DCsim_HOME)/Library
TMPDIR = $(DCsim_HOME)/Temp

HEADERS = $(wildcard $(INCDIR)/*.hh)

SOURCES = $(wildcard $(SRCDIR)/*.cc)

#OBJECTS = $(subst $(SRCDIR),$(OBJDIR),$(SOURCES:.cc=.o))

TARGETS = $(OBJECTS)

LIBS = -lGeom  -lgfortran -lm

# Fortran compiler
FC = gfortran

# Compilation flags
CFLAGS = -Wall -Wextra -pedantic -ansi -Wabi -Wno-long-long -Woverloaded-virtual \
	 `root-config --cflags` \
        -fpic -fno-common -c \
	-I$(INCDIR) -I$(HEEDDIR) -DINS_CRETURN 

FFLAGS = -fpic -c

# Optimization flags
# CFLAGS += -Os
# FFLAGS += -Os
CFLAGS += -O2
FFLAGS += -O2

# Debug flags
# CFLAGS += -g
# FFLAGS += -g
# Profiling flag
 CFLAGS += -pg

###############################################################################
# ROOT Fun
ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs) 
ROOTGLIBS     = $(shell root-config --glibs) 
ROOTLDFLAGS     = $(shell root-config --ldflags) 

# Linking flags

LDFLAGS = $(ROOTGLIBS)$(ROOTLDFLAGS) $(LIBS)


###############################################################################
# Objects needed for the application. Automagically Collected.

CLASSES          = $(patsubst Source/%.cc, %, $(shell grep -L ClassImp Source/*.cc))
ROOTCLASSES      = $(patsubst Source/%.cc, %, $(shell grep -l ClassImp Source/*.cc))
 $(GUICLASSES)

###############################################################################
OBJECTS         = $(addsuffix .o, $(CLASSES)) 
DICT_OBJ        = $(addsuffix Dict.o, $(ROOTCLASSES))
BUILDOBJS       = $(addprefix $(OBJDIR)/,$(OBJECTS)) 
DICT_LIB        = $(addsuffix .so, $(addprefix $(OBJDIR)/lib,$(ROOTCLASSES)))

#LDFLAGS = `root-config --glibs` `root-config --ldflags`-lGeom \
	-lgfortran -lm

all:	$(OBJECTS)
	@echo Creating library libDCsim...
	@ar rcs $(LIBDIR)/libDCsim.a $(OBJECTS) 
	@ranlib $(LIBDIR)/libDCsim.a
	@touch $(OBJDIR)/last_updated_on
	@echo Finished.

.PHONY:	heed

lib:    $(DICT_LIB) $(DICT_OBJ) $(OBJECTS)
	@test -d $(LIBDIR) || mkdir $(LIBDIR)
	$(CXX) $(CXXFLAGS) -shared $(LDFLAGS) $(addprefix $(OBJDIR)/,$(DICT_OBJ))  -o $(LIBDIR)/libDCsim.so
	@ar rcs $(LIBDIR)/libDCsim.a $(OBJECTS) 
	@ranlib $(LIBDIR)/libDCsim.a

installdirs : 
	@if [ ! -d $(DCsim_HOME)/Library/$(BFARCH) ] ; then mkdir -p $(DCsim_HOME)/Library/$(BFARCH); \
	    echo "   >>>> Create $(DCsim_HOME)/Library/$(BFARCH)"; fi
	@if [ ! -d $(OBJDIR) ]; then mkdir -p $(OBJDIR); \
	    echo "   >>>> Create $(OBJDIR)"; \
        else echo " $(OBJDIR) already exists"; fi

clean:
	@echo Removing object files...
	@$(RM) $(OBJDIR)/*.o
	@echo Removing libraries...
	@$(RM) $(LIBDIR)/*.a 

## Dependencies
DEP_FILE         = $(addsuffix .d, $(CLASSES))
DEP_FILE        += $(addsuffix .d, $(ROOTCLASSES))
DEPS            = $(addprefix $(OBJ_DIR)/,$(DEP_FILE))
-include        $(DEPS) $(OBJ_DIR)/main.d


#######################################
# Implicit Targets

tmp/lib%.so:    %Dict.cxx %.cc
		$(CXX) $(CFLAGS) -shared $(LDFLAGS) $< -o $@

%Dict.o:        %Dict.cxx %.cc
		@test -d $(OBJDIR) || mkdir $(OBJDIR)
		$(CXX) $(CFLAGS) -c $< -o $(OBJDIR)/$@

%Dict.cxx:      %.cc %.hh
		util/LinkDefMaker.pl $@ $^
		rootcint -f $@ -c $(CXXFLAGS)

%.o:            %.cc %.hh

		@test -d $(OBJDIR) || mkdir $(OBJDIR)
		$(CXX) $(CFLAGS) -c $< -o $(OBJDIR)/$@

%.o:            %.cc 
		@test -d $(OBJDIR) || mkdir $(OBJDIR)
		$(CXX) $(CFLAGS) -c $< -o $(OBJDIR)/$@

%.o:            %.cxx %.h
		@test -d $(OBJDIR) || mkdir $(OBJDIR)
		$(CXX) $(CFLAGS) -c $< -o $(OBJDIR)/$@

%.o:            $(SRCDIR)/%.cc $(INCDIR)/%.hh
		@echo $@
		@test -d $(OBJDIR) || mkdir $(OBJDIR)
		$(CXX) $(CFLAGS) $< -o $@

$(OBJDIR)/%.d: %.cxx
		@test -d $(OBJDIR) || mkdir $(OBJDIR)
		$(CXX) -MM $(CFLAGS) $< > $@
		@perl -pi -e "s|(.*).o: |\$$1.o $@: |g" $@


#-include $(wildcard *.d)

# DO NOT DELETE
