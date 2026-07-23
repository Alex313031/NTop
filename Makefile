# NTop — MinGW-w64 Makefile
#
# Builds with a stock MinGW-w64 toolchain; no Visual Studio or GN required.
#
#   32-bit build                            make CROSS=i686
#   64-bit build:                           make CROSS=x86_64
#   ANSI instead of Unicode:                make UNICODE=0
#   Debug build:                            make DEBUG=1
#
# On native Windows, run make from an MSYS2 or Git-Bash shell so the
# rm used by the clean target is available.

CROSS   ?= i686
CC      := $(CROSS)-w64-mingw32-gcc
WINDRES := $(CROSS)-w64-mingw32-windres

UNICODE  ?= 1
DEBUG    ?= 0
NTOP_VER ?= 0.3.22

SRCDIR  := src
TARGET  := NTop.exe
OBJECTS := ntop.o util.o vi.o ntop.res.o

# Target Windows XP and later. PSAPI_VERSION=1 keeps GetProcessMemoryInfo an
# import from psapi.dll (present on every Windows version); the default on
# newer SDKs resolves it to K32GetProcessMemoryInfo, which is Win7+ only.
CPPFLAGS += -DNTOP_VER=$(NTOP_VER) -D_WIN32_WINNT=0x0501 -DWINVER=0x0501 -DPSAPI_VERSION=1
CFLAGS   += -Wall -mfpmath=sse -msse2 -static-libgcc
LDLIBS   := -lkernel32 -ladvapi32 -luser32 -lpsapi

# Keep the PE subsystem version at 5.01 so the binary still loads on XP.
LDFLAGS  += -static-libgcc -Wl,--subsystem,console:5.01

ifeq ($(UNICODE),1)
  # -municode selects the wide (wmain) CRT startup that _tmain expands to
  CPPFLAGS += -DUNICODE -D_UNICODE
  CFLAGS   += -municode
  LDFLAGS  += -municode
else
  CFLAGS   += -mconsole
  LDFLAGS  += -mconsole
endif

ifeq ($(DEBUG),1)
  CFLAGS += -Og -g2
else
  CFLAGS  += -O3 -g0
  LDFLAGS += -s
endif

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

%.o: $(SRCDIR)/%.c $(SRCDIR)/ntop.h $(SRCDIR)/util.h $(SRCDIR)/vi.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

ntop.res.o: $(SRCDIR)/ntop.rc $(SRCDIR)/ntop.ico
	$(WINDRES) -O coff -o $@ $<

clean:
	$(RM) $(TARGET) $(OBJECTS)

.PHONY: all clean
