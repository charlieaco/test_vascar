APPNAME=TCP_server

INCLUDE =-I/usr/local/include
CPP=g++
CPPFLAGS=$(INCLUDE) -g -ggdb -O2 -rdynamic -std=c++11 -fexceptions -finline-functions -Wdeprecated -Wdeprecated-declarations


IDIR=.
ODIR=.
LDIR =-L/usr/local/lib

LIBS=$(LDIR) -lpthread 

_DEPS = CThread.hpp CThreadServer.hpp CThreadObserver.hpp CData.hpp CConnectionMgr.hpp CThreadObserver.hpp 

	
	
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o  CConnectionMgr.o 


       
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CPPFLAGS)

${APPNAME}: $(OBJ)
	$(CPP) -o $@ $^ $(CPPFLAGS) $(LIBS)

package:
	./buildpkg.sh ${APPNAME}

.PHONY: clean

clean: 
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ ${APPNAME}

all: ${APPNAME} 
