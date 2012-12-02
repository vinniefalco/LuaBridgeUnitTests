# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(TARGET_ARCH),)
  TARGET_ARCH := -march=native
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_8068DBC5=1" -I /usr/include -I /usr/include/freetype2 -I ../Jucer/JuceLibraryCode -I ../../Extern/LuaBridge/Source -I ../../Extern/LuaUnityBuild/Source
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS) 
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_8068DBC5=1" -I /usr/include -I /usr/include/freetype2 -I ../Jucer/JuceLibraryCode -I ../../Extern/LuaBridge/Source -I ../../Extern/LuaUnityBuild/Source
  TARGET := LuaBridgeUnitTests
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_8068DBC5=1" -I /usr/include -I /usr/include/freetype2 -I ../Jucer/JuceLibraryCode -I ../../Extern/LuaBridge/Source -I ../../Extern/LuaUnityBuild/Source
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -Os
  CXXFLAGS += $(CFLAGS) 
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_8068DBC5=1" -I /usr/include -I /usr/include/freetype2 -I ../Jucer/JuceLibraryCode -I ../../Extern/LuaBridge/Source -I ../../Extern/LuaUnityBuild/Source
  TARGET := LuaBridgeUnitTests
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

OBJECTS := \
  $(OBJDIR)/LuaUnityBuild_5.1.5_28411761.o \
  $(OBJDIR)/LuaUnityBuild_5.2.1_5ceef49e.o \
  $(OBJDIR)/Main_90ebc5c2.o \
  $(OBJDIR)/SpeedTests_ecaa6aa3.o \
  $(OBJDIR)/BinaryData_e2bc246d.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking LuaBridgeUnitTests
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning LuaBridgeUnitTests
	-@rm -f $(OUTDIR)/$(TARGET)
	-@rm -rf $(OBJDIR)/*
	-@rm -rf $(OBJDIR)

strip:
	@echo Stripping LuaBridgeUnitTests
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/LuaUnityBuild_5.1.5_28411761.o: ../../Extern/LuaUnityBuild/Source/LuaUnityBuild/LuaUnityBuild_5.1.5.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LuaUnityBuild_5.1.5.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LuaUnityBuild_5.2.1_5ceef49e.o: ../../Extern/LuaUnityBuild/Source/LuaUnityBuild/LuaUnityBuild_5.2.1.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LuaUnityBuild_5.2.1.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Main.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SpeedTests_ecaa6aa3.o: ../../Source/SpeedTests.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SpeedTests.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryData_e2bc246d.o: ../Jucer/JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
