# Verbosity of the make system: 
# V=0 (or nothing): do not displays commands
# V>0 display commands
ifndef V
V=0
endif

ifeq ($(V),0)
CMDPREF=@
else
CMDPREF=
endif

CC      := arm-linux-gcc
CXX     := arm-linux-g++

HEADPATH  := \
-I./ \
-I./include -I./libs/xmlwrapper/src -I./libs/SharedBuffMgr/src -I./libs/MemMgr/src -I./libs/msgreader/src -I./libs/VideoCap/src -I./libs/MassMemAccess/src \
-I./libs/PBEngine/src -I./libs/freetype/src -I./libs/frameratectrl/src -I./libs/eptz/src -I./libs/Resize/src -I./libs/AutoScene/src \
-I./libs/ImgBackEndProcess/src -I./libs/VideoDisp/src -I./libs/png12/src -I./libs/z/src -I./libs/bitmaskresizer/src -I./libs/Mp4VEnc/src -I./libs/JPEGEnc/src \
-I./libs/H264Enc/src -I./libs/SVCEnc/src

LIBPATH   :=\
-L./libs/AutoScene/lib -L./libs/H264Enc/lib -L./libs/ImgBackEndProcess/lib -L./libs/JPEGEnc/lib -L./libs/MassMemAccess/lib -L./libs/MemMgr/lib \
-L./libs/Mp4VEnc/lib -L./libs/PBEngine/lib -L./libs/Resize/lib -L./libs/SharedBuffMgr/lib -L./libs/VideoCap/lib -L./libs/VideoDisp/lib \
-L./libs/bitmaskresizer/lib -L./libs/eptz/lib -L./libs/frameratectrl/lib -L./libs/freetype/lib -L./libs/msgreader/lib -L./libs/png12/lib \
-L./libs/xmlwrapper/lib -L./libs/z/lib -L./libs -L./libs/SVCEnc/lib

#-lvideodisp -lsvcenc
LIBFILES  :=\
-lautoscene -lh264enc -limgbackendprocess -ljpegenc -lmassmemaccess -lmemmgr -lmp4venc -lpbengine -lresize -lsharedbuffmgr -lvideocap \
-lbitmaskresizer -leptz -lframeratectrl -lfreetype -lmsgreader -lpng12 -lxmlwrapper -lz -lROISelector -lexpat -lm -lpthread -lrt 

#-D_SVC
CFLAG :=\
-Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration \
-O2 -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow \
-D__KERNEL__ -D_LINUX -D_LINUX_ARM -D__ASM_ARCH_PLATFORM_MOZART_H__ -D_VMA_IBPE -D_VMA_IRE -D_VMA_JEBE \
-D_VMA_MEBE -D_H4EE -D_PRE_NR_ -D_AUTOSCENE_ -D_VMA_IBPE_BACK_DOOR -D_JPEG_SNAPSHOT_ -D_DUMP_JPEG_FILE -D_1DRAM_VERSION_ 

CFLAG += -D_DEBUG

TARGET    := venc
SRCC	    := $(wildcard *.c)
COBJ	    := $(SRCC:.c=.o)
SRCXX	    := $(wildcard *.cpp)
CXXOBJ    := $(SRCXX:.cpp=.o)
OBJ	      := $(COBJ) $(CXXOBJ)
INSTALL_DIR = /opt/ASC884xA_5xA_SDK6.2_wk1420/squashfs-root-onvif/usr/sbin

$(TARGET) : $(OBJ)
	$(CMDPREF)$(CC) -o $@ -Wl,--start-group  $(HEADPATH) $(LIBPATH) $(LIBFILES) $(OBJ) -Wl,--end-group

$(COBJ):%.o:%.c 
	$(CMDPREF)$(CC) $(CFLAG) $(HEADPATH) -o $@ -c $<

$(CXXOBJ):%.o:%.cpp 
	$(CMDPREF)$(CXX) $(CFLAG) $(HEADPATH) -o $@ -c $<	

install:
	$(CMDPREF)cp -f $(TARGET) $(INSTALL_DIR)
	
.PHONY : clean
clean:
	-rm -f $(TARGET) $(OBJ)
