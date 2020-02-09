APPNAME=TCP_client

INCLUDE =-I/usr/local/include
CPP=g++
CPPFLAGS=$(INCLUDE) -g -ggdb -O2 -rdynamic -std=c++11 -fexceptions -finline-functions -Wdeprecated -Wdeprecated-declarations


IDIR=.
ODIR=.
LDIR =-L/usr/local/lib

LIBS=$(LDIR)  

#_DEPS = CThreadServer.hpp
_DEPS = CInformationManager.hpp CClientConnectionMgr.hpp CDataClient.hpp

	
	
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#_OBJ = main.o CThreadServer.o
_OBJ = CInformationManager.o CClientConnectionMgr.o  CDataClient.o main.o  


       
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
