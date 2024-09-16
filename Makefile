APP := irrgame
CC := clang++ -std=c++20
CCFLAGS := 


SRCDIR := src
OBJDIR := obj
SUBDIRS    := $(shell find $(SRCDIR) -type d)
OBJSUBDIRS := $(patsubst $(SRCDIR)%,$(OBJDIR)%,$(SUBDIRS))
ALLCPPS    := $(shell find $(SRCDIR) -type f -iname *.cpp)
ALLOBJS    := $(patsubst $(SRCDIR)%,$(OBJDIR)%,$(patsubst %.cpp,%.o,$(ALLCPPS)))

INCLUDEPATH := -I $(SRCDIR)
LIBPATH 	:=
UNAME       := $(shell uname)

#Config paths
ifeq ($(UNAME),Linux)
	INCLUDEPATH += -I /usr/include
	LIBPATH += 
else ifeq ($(UNAME),Darwin)
	INCLUDEPATH += -I /opt/homebrew/Cellar/irrlicht/1.8.5_1/include/
	LIBPATH += -L /opt/homebrew/Cellar/irrlicht/1.8.5_1/lib -lIrrlicht
else # windows
	CCFLAGS +=
endif

#Debugging
ifdef DEBUG
	CCFLAGS += -g
else ifdef SANITIZE
	CCFLAGS += -fsanitize=address -fno-omit-frame-pointer -O1 -g
else
	CCFLAGS += -O3
endif

# tarjet : dependences
#	rules
$(APP) : $(OBJSUBDIRS) $(ALLOBJS) $(ALLOBJS)
	$(CC) $(CCFLAGS) $(ALLOBJS) $(LIBPATH) -o $@

.PHONY : info clean cleanall

#compile
$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) $(CCFLAGS) -c $^ $(INCLUDEPATH) -o $@


$(OBJSUBDIRS) :
	mkdir -p $(OBJSUBDIRS)

cleanall : clean
	rm $(APP)

clean:
	rm -r $(OBJDIR)

info:
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCPPS))
	$(info $(ALLOBJS))