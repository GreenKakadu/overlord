ENGINE_DIR = ../../engine
PBM_COMMON_DIR = ../../PbmCommon
QT += network
CONFIG += qt \
    release \
    stl
CONFIG -= build_all
QMAKE_CXXFLAGS_WARN_ON = -Wno-unused \
    -Wno-sign-compare
QMAKE_CXXFLAGS += -std=c++0x
VPATH += $${ENGINE_DIR}
VPATH += $${PBM_COMMON_DIR}
win32:DEFINES += NOLRAND48
win32:DEFINES += WINGETCWD
INCLUDEPATH += $${ENGINE_DIR} \
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
RESOURCES += ./images.qrc \
    ./localIcons.qrc \
    ./../../PbmCommon/icons.qrc
include($${ENGINE_DIR}/sources.make)
include($${ENGINE_DIR}/headers.make)
HEADERS += ContextManager.h \
    OrderWindow.h \
    dataInterface/AttackOrderDialog.h \
    dataInterface/BestowTitleOrderDialog.h \
    dataInterface/BuyItemOrderDialog.h \
    dataInterface/ClaimTitleOrderDialog.h \
    dataInterface/EnterConstructionOrderDialog.h \
    dataInterface/EquipItemOrderDialog.h \
    dataInterface/GiveItemOrderDialog.h \
    dataInterface/LearnSkillOrderDialog.h \
    dataInterface/MoveToLocationOrderDialog.h \
    dataInterface/OathOrderDialog.h \
    dataInterface/OrderDialog.h \
    dataInterface/OvlComboBox.h \
    dataInterface/OvlElementComboBox.h \
    dataInterface/RecruitOrderDialog.h \
    dataInterface/ReleaseTitleOrderDialog.h \
    dataInterface/RevokeTitleOrderDialog.h \
    dataInterface/StanceOrderDialog.h \
    dataInterface/TargetOrderDialog.h \
    dataInterface/TeachOrderDialog.h \
    dataInterface/UnitOrderDialog.h \
    dataInterface/UseSkillOrderDialog.h \
    ClickableLabel.h \
    ClickableLine.h \
    ConnectionController.h \
    DataManipulator.h \
    DataView.h \
    FactionView.h \
    Global.h \
    ListViewer.h \
    LocationView.h \
    MainClientWindow.h \
    MapView.h \
    UnitView.h \
    ViewManager.h \
    hex.h \
    InfoWindow.h \
    $${PBM_COMMON_DIR}/Common.h \
    $${PBM_COMMON_DIR}/Led.h \
    $${PBM_COMMON_DIR}/PbmMessage.h \
    InnerLocation.h \
    CustomStyles.h \
    PopupMenuWindow.h \
    $${ENGINE_HEADERS}
SOURCES += ContextManager.cpp \
    OrderWindow.cpp \
    dataInterface/AttackOrderDialog.cpp \
    dataInterface/BestowTitleOrderDialog.cpp \
    dataInterface/BuyItemOrderDialog.cpp \
    dataInterface/ClaimTitleOrderDialog.cpp \
    dataInterface/EnterConstructionOrderDialog.cpp \
    dataInterface/EquipItemOrderDialog.cpp \
    dataInterface/GiveItemOrderDialog.cpp \
    dataInterface/LearnSkillOrderDialog.cpp \
    dataInterface/MoveToLocationOrderDialog.cpp \
    dataInterface/OathOrderDialog.cpp \
    dataInterface/OrderDialog.cpp \
    dataInterface/OvlComboBox.cpp \
    dataInterface/OvlElementComboBox.cpp \
    dataInterface/RecruitOrderDialog.cpp \
    dataInterface/ReleaseTitleOrderDialog.cpp \
    dataInterface/RevokeTitleOrderDialog.cpp \
    dataInterface/StanceOrderDialog.cpp \
    dataInterface/TargetOrderDialog.cpp \
    dataInterface/TeachOrderDialog.сpp \
    dataInterface/UnitOrderDialog.cpp \
    dataInterface/UseSkillOrderDialog.cpp \
    main.cpp \
    hex.cpp \
    ClickableLabel.cpp \
    ClickableLine.cpp \
    ConnectionController.cpp \
    DataView.cpp \
    FactionView.cpp \
    ListViewer.cpp \
    LocationView.cpp \
    MainClientWindow.cpp \
    MapView.cpp \
    UnitView.cpp \
    ViewManager.cpp \
    $${PBM_COMMON_DIR}/Common.cpp \
    $${PBM_COMMON_DIR}/Led.cpp \
    $${PBM_COMMON_DIR}/PbmMessage.cpp \
    InfoWindow.cpp \
    InnerLocation.cpp \
    CustomStyles.cpp \
    PopupMenuWindow.cpp \
    $${ENGINE_SOURCES}
contains(QT_CONFIG, opengl):QT += opengl
FORMS = ConfigWin.ui \
    MainClientWindow.ui \
    askReportDialog.ui
OTHER_FILES += PbmClient.rc
