ENGINE_DIR =  ../../engine
PBM_COMMON_DIR = ../PbmCommon
QT +=  network
CONFIG +=  qt release stl
CONFIG -=  build_all
QMAKE_CXXFLAGS_WARN_ON = -Wno-unused  -Wno-sign-compare
QMAKE_CXXFLAGS += -std=c++0x
VPATH +=  $${ENGINE_DIR}
VPATH +=  $${PBM_COMMON_DIR}
win32:DEFINES +=  NOLRAND48
win32:DEFINES +=  WINGETCWD
INCLUDEPATH +=  \
        $${ENGINE_DIR} \
        $${ENGINE_DIR}/combat \
        $${ENGINE_DIR}/combat/actions \
        $${ENGINE_DIR}/combat/orders \
        $${ENGINE_DIR}/datatypes \
        $${ENGINE_DIR}/datatypes/attributes \
        $${ENGINE_DIR}/datatypes/collections \
        $${ENGINE_DIR}/datatypes/conditions \
        $${ENGINE_DIR}/datatypes/elements \
        $${ENGINE_DIR}/datatypes/entities \
        $${ENGINE_DIR}/datatypes/events \
        $${ENGINE_DIR}/datatypes/misc \
        $${ENGINE_DIR}/datatypes/rules \
        $${ENGINE_DIR}/datatypes/strategies \
        $${ENGINE_DIR}/datatypes/varieties \
        $${ENGINE_DIR}/implementation/actions \
        $${ENGINE_DIR}/implementation/combat \
        $${ENGINE_DIR}/implementation/effects \
        $${ENGINE_DIR}/misc \
        $${ENGINE_DIR}/process \
        $${ENGINE_DIR}/process/conflicts \
        $${ENGINE_DIR}/process/orders \
        $${ENGINE_DIR}/report \
        $${ENGINE_DIR}/report/patterns \
        dataInterface \
	/usr/include \
	/usr/include/Qt
RESOURCES +=  ./images.qrc ./localIcons.qrc $${PBM_COMMON_DIR}/icons.qrc
include($${ENGINE_DIR}/sources.make)
include($${ENGINE_DIR}/headers.make)
HEADERS +=  \
	ClickableLabel.h \
	ClickableLine.h \
	ConnectionController.h \
	ContextManager.h \
	CustomStyles.h \
	DataManipulator.h \
	DataView.h \
	FactionView.h \
	hex.h \
	InfoWindow.h \
	InnerLocation.h \
	ListViewer.h \
	LocationView.h \
	MainClientWindow.h \
	MapView.h \
	OrderWindow.h \
	PopupMenuWindow.h \
	UnitView.h \
	ViewManager.h \
	dataInterface/OvlComboBox.h \
	dataInterface/OvlElementComboBox.h \
	dataInterface/OrderDialog.h \
	dataInterface/BestowTitleOrderDialog.h \
	dataInterface/BuyItemOrderDialog.h \
	dataInterface/ClaimTitleOrderDialog.h \
	dataInterface/EquipItemOrderDialog.h \
	dataInterface/GiveItemOrderDialog.h \
	dataInterface/MoveToLocationOrderDialog.h \
	dataInterface/OathOrderDialog.h \
	dataInterface/ReleaseTitleOrderDialog.h \
	dataInterface/UnitOrderDialog.h \
	dataInterface/UseSkillOrderDialog.h \
        $${PBM_COMMON_DIR}/Common.h \
	$${PBM_COMMON_DIR}/Led.h \
	$${PBM_COMMON_DIR}/PbmMessage.h 

HEADERS += $${ENGINE_HEADERS}


SOURCES +=  \
        main.cpp \
        ClickableLabel.cpp \
	ClickableLine.cpp \
	ConnectionController.cpp \
	ContextManager.cpp \
	CustomStyles.cpp \
	DataView.cpp \
	FactionView.cpp \
	hex.cpp \
	InfoWindow.cpp \ 
	InnerLocation.cpp \
	ListViewer.cpp \
	LocationView.cpp \
	MainClientWindow.cpp \
	MapView.cpp \
	OrderWindow.cpp \
	PopupMenuWindow.cpp \
	UnitView.cpp \
	ViewManager.cpp \
	dataInterface/OvlComboBox.cpp \
	dataInterface/OvlElementComboBox.cpp \
	dataInterface/OrderDialog.cpp \
	dataInterface/BestowTitleOrderDialog.cpp \
	dataInterface/BuyItemOrderDialog.cpp \
	dataInterface/ClaimTitleOrderDialog.cpp \
	dataInterface/EquipItemOrderDialog.cpp \
	dataInterface/GiveItemOrderDialog.cpp \
	dataInterface/MoveToLocationOrderDialog.cpp \
	dataInterface/OathOrderDialog.cpp \
	dataInterface/ReleaseTitleOrderDialog.cpp \
	dataInterface/UnitOrderDialog.cpp \
	dataInterface/UseSkillOrderDialog.cpp \
        $${PBM_COMMON_DIR}/Common.cpp \
        $${PBM_COMMON_DIR}/Led.cpp \
        $${PBM_COMMON_DIR}/PbmMessage.cpp 

SOURCES += 	$${ENGINE_SOURCES}
contains(QT_CONFIG, opengl):QT +=  opengl
FORMS =  ConfigWin.ui MainClientWindow.ui askReportDialog.ui
OTHER_FILES +=  PbmClient.rc
