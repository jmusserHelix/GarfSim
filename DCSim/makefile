OBJDIR = $(DCsim_HOME)/Object
SRCDIR = $(DCsim_HOME)/Source
INCDIR = $(DCsim_HOME)/Include
LIBDIR = $(DCsim_HOME)/Library

HEADERS = $(wildcard $(INCDIR)/*.hh)

SOURCES = $(wildcard $(SRCDIR)/*.cc)

OBJECTS = $(subst $(SRCDIR),$(OBJDIR),$(SOURCES:.cc=.o))


TARGETS = $(OBJECTS)

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

# Linking flags
LDFLAGS = `root-config --glibs` `root-config --ldflags`-lGeom \
	-lgfortran -lm

all:	$(OBJECTS)
	@echo Creating library libDCsim...
	@ar rcs $(LIBDIR)/libDCsim.a $(OBJECTS) 
	@ranlib $(LIBDIR)/libDCsim.a
	@touch $(OBJDIR)/last_updated_on
	@echo Finished.

.PHONY:	heed

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

$(OBJDIR)/DCsim.o: \
	$(SRCDIR)/DCsim.cc \
	$(INCDIR)/DCsim.hh 
	@echo $@
	@$(CXX) $(CFLAGS) $< -o $@
