######################## Internal Library  Definitions #################
BASE_DIR= .
MODULE_DIR= $(BASE_DIR)
MODULE_NAME = ovltest
CXX = c++
LD = c++
CXXFLAGS = -W -Wall

######################## Compilation dependency #########################

SOURCES = GameData.cpp \
	  Parser.cpp \
	  FileParser.cpp \
          PrototypeManager.cpp \
          RulesCollection.cpp \
          EntitiesCollection.cpp \
          BasicCollection.cpp \
          RulesCollectionExplicit.cpp \
          EntitiesCollectionExplicit.cpp \
	  TerrainRule.cpp \
	  DataManipulator.cpp \
	  DataHandler.cpp \
	  UnitEntity.cpp \
	  turn.cpp \
	  TitleRule.cpp \
#	  SkillRule.cpp \
#	  ItemRule.cpp \
#	  RaceRule.cpp \
#	  MovementModeRule.cpp \
#	  LocationRule.cpp \
#	  FactionRule.cpp \
#	  GameRule.cpp \
#	  Order.cpp \
#	  Turn.cpp \
#	  ovlTest.cpp \

OBJECTS := $(patsubst %.cpp,%.o,$(wildcard $(SOURCES) ) )

############################### Rules  #######################################
%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@


default  : $(MODULE_NAME)


$(MODULE_NAME)	: $(OBJECTS) 
	  $(LD) -o $(MODULE_NAME) $(OBJECTS) 


clean   :
	$(RM) $(MODULE_DIR)/*.o
	$(RM) depend*


############################### Dependencies list ############################

depend$(MODULE_NAME):
		 $(CXX) -MM  $(CXXFLAGS)  $(wildcard $(SOURCES) ) > $@

include	depend$(MODULE_NAME)

