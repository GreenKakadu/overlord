######################## Internal Library  Definitions #################
BASE_DIR= .
MODULE_DIR= $(BASE_DIR)
MODULE_NAME = overlord
CXX = c++
LD = c++
CXXFLAGS = -O3 -W -Wall -Wno-unused -Wno-sign-compare $(INCLUDES)
######################## Include Directories   #########################
INCLUDES = -I$(BASE_DIR)                       \
           -I$(BASE_DIR)/datatypes             \
           -I$(BASE_DIR)/datatypes/rules       \
           -I$(BASE_DIR)/datatypes/attributes  \
           -I$(BASE_DIR)/datatypes/entities    \
           -I$(BASE_DIR)/datatypes/varieties   \
           -I$(BASE_DIR)/datatypes/elements    \
           -I$(BASE_DIR)/datatypes/conditions  \
           -I$(BASE_DIR)/datatypes/collections \
           -I$(BASE_DIR)/datatypes/misc        \
           -I$(BASE_DIR)/datatypes/strategies  \
           -I$(BASE_DIR)/process               \
           -I$(BASE_DIR)/process/orders        \
           -I$(BASE_DIR)/process/conflicts     \
           -I$(BASE_DIR)/report/patterns       \
           -I$(BASE_DIR)/report                \
           -I$(BASE_DIR)/misc                  \
           -I$(BASE_DIR)/implementation/actions

######################## Compilation dependency #########################

SOURCES = turn.cpp \
	  DataManipulator.cpp \
          datatypes/rules/ConstructionRule.cpp \
	  			datatypes/rules/Rule.cpp \
	  			datatypes/rules/TerrainRule.cpp \
	  			datatypes/rules/TitleRule.cpp \
	  			datatypes/rules/LeaderRaceRule.cpp \
    			datatypes/rules/CreatureRaceRule.cpp \
    			datatypes/rules/FollowerRaceRule.cpp \
	  			datatypes/rules/RaceRule.cpp \
	  			datatypes/rules/ItemRule.cpp \
          datatypes/rules/RuleIndex.cpp \
          datatypes/rules/SkillRule.cpp \
          datatypes/entities/ConstructionEntity.cpp \
          datatypes/entities/Entity.cpp \
          datatypes/entities/UnitEntity.cpp \
          datatypes/entities/LocationEntity.cpp \
          datatypes/entities/FactionEntity.cpp \
          datatypes/varieties/ConstructionWorksVariety.cpp \
          datatypes/varieties/Variety.cpp \
          datatypes/varieties/MovementVariety.cpp \
          datatypes/varieties/DirectionVariety.cpp \
          datatypes/varieties/StanceVariety.cpp \
          datatypes/varieties/EquipmentSlotVariety.cpp \
          datatypes/elements/ConstructionWorksElement.cpp \
          datatypes/elements/SkillUseElement.cpp \
          datatypes/elements/TitleElement.cpp \
          datatypes/elements/ToolUseElement.cpp \
          datatypes/elements/TreeBonusElement.cpp \
          datatypes/elements/ReportElement.cpp \
          datatypes/elements/InventoryElement.cpp \
          datatypes/elements/StanceElement.cpp \
          datatypes/elements/SkillElement.cpp \
          datatypes/elements/ItemElement.cpp \
          datatypes/elements/BonusElement.cpp \
          datatypes/elements/MovementElement.cpp \
          datatypes/elements/TravelElement.cpp  \
          datatypes/elements/RaceElement.cpp  \
          datatypes/elements/ResourceElement.cpp \
          datatypes/conditions/BasicCondition.cpp \
          datatypes/conditions/SkillCondition.cpp   \
          datatypes/conditions/ObservationCondition.cpp   \
          datatypes/collections/OrderPrototypesCollection.cpp \
          datatypes/collections/EntitiesCollection.cpp    \
          datatypes/collections/MovementMode.cpp    \
          datatypes/collections/BasicRulesCollection.cpp \
          datatypes/collections/BasicCollection.cpp  \
          datatypes/collections/RulesCollection.cpp  \
          datatypes/collections/BasicEntitiesCollection.cpp   \
          datatypes/collections/BasicVarietiesCollection.cpp   \
          datatypes/collections/VarietiesCollection.cpp   \
          datatypes/collections/DataStorageHandler.cpp   \
          datatypes/AbstractData.cpp   \
          datatypes/ItemElementData.cpp   \
          datatypes/IntegerData.cpp   \
          datatypes/BasicData.cpp   \
          datatypes/GameData.cpp   \
          datatypes/PrototypeManager.cpp   \
          datatypes/misc/BasicExit.cpp \
          datatypes/misc/EntityStatistics.cpp   \
          datatypes/misc/ExplicitExit.cpp   \
          datatypes/misc/NewEntityPlaceholder.cpp   \
          datatypes/strategies/BasicLearningStrategy.cpp \
          datatypes/strategies/BasicProductionStrategy.cpp \
          datatypes/strategies/ConstructionUsingStrategy.cpp \
          datatypes/strategies/SummonUsingStrategy.cpp   \
          datatypes/strategies/Strategy.cpp   \
          datatypes/strategies/CreatureLearningStrategy.cpp   \
          datatypes/strategies/MagicLearningStrategy.cpp   \
          datatypes/strategies/NormalLearningStrategy.cpp   \
          datatypes/strategies/CombatUsingStrategy.cpp   \
          datatypes/strategies/HarvestUsingStrategy.cpp   \
          datatypes/strategies/BasicUsingStrategy.cpp   \
          datatypes/strategies/BuildUsingStrategy.cpp   \
          datatypes/strategies/CraftUsingStrategy.cpp   \
          datatypes/strategies/MarketStrategy.cpp   \
          datatypes/Rational.cpp   \
          datatypes/StringData.cpp   \
          datatypes/SkillLevelElementData.cpp   \
          datatypes/RaceElementData.cpp   \
          process/orders/AcceptOrder.cpp   \
          process/orders/AtOrder.cpp   \
          process/orders/AttackOrder.cpp   \
          process/orders/BasicOrderSynchronizationRequest.cpp   \
          process/orders/BestowOrder.cpp   \
          process/orders/BuyOrder.cpp   \
          process/orders/CancelOrder.cpp   \
          process/orders/CaravanOrder.cpp   \
          process/orders/ChristenOrder.cpp   \
          process/orders/ClaimOrder.cpp   \
          process/orders/CombatOrder.cpp   \
          process/orders/DayOrder.cpp   \
          process/orders/DescribeOrder.cpp   \
          process/orders/DisbandOrder.cpp   \
          process/orders/DropOrder.cpp   \
          process/orders/EjectOrder.cpp   \
          process/orders/EmailOrder.cpp   \
          process/orders/EnterOrder.cpp   \
          process/orders/EquipOrder.cpp   \
          process/orders/ExitOrder.cpp   \
          process/orders/ForgetOrder.cpp   \
          process/orders/GetOrder.cpp   \
          process/orders/GiveOrder.cpp   \
          process/orders/GuardOrder.cpp   \
          process/orders/HaveOrder.cpp   \
          process/orders/LeaderOrder.cpp   \
          process/orders/LeadingOrder.cpp   \
          process/orders/LeaveOrder.cpp   \
          process/orders/MarchOrder.cpp   \
          process/orders/MergeOrder.cpp   \
          process/orders/MoveOrder.cpp   \
          process/orders/NameOrder.cpp   \
          process/orders/OathOrder.cpp   \
          process/orders/OrderPrototype.cpp   \
          process/orders/PasswordOrder.cpp   \
          process/orders/PatrolOrder.cpp   \
          process/orders/PillageOrder.cpp   \
          process/orders/PromoteOrder.cpp   \
          process/orders/RecruitOrder.cpp   \
          process/orders/ReshowOrder.cpp   \
          process/orders/ResignOrder.cpp   \
          process/orders/RetreatOrder.cpp   \
          process/orders/SeeOrder.cpp   \
          process/orders/SellOrder.cpp   \
          process/orders/SettingOrder.cpp   \
          process/orders/SizeOrder.cpp   \
          process/orders/SkillOrder.cpp   \
          process/orders/SplitOrder.cpp   \
          process/orders/StackOrder.cpp   \
          process/orders/StanceOrder.cpp   \
          process/orders/StayOrder.cpp   \
          process/orders/StudyOrder.cpp   \
          process/orders/SwapOrder.cpp   \
          process/orders/SwapOrderRequest.cpp   \
          process/orders/SynchroOrder.cpp   \
          process/orders/SynchroOrderRequest.cpp   \
          process/orders/TacticOrder.cpp   \
          process/orders/TargetOrder.cpp   \
          process/orders/TeachOrder.cpp   \
          process/orders/TestOrder.cpp   \
          process/orders/UnstackOrder.cpp   \
          process/orders/UseOrder.cpp   \
          process/orders/WaitOrder.cpp   \
          process/orders/WithdrawOrder.cpp   \
          process/orders/WorkOrder.cpp   \
          process/orders/YieldOrder.cpp   \
          process/conflicts/BasicCompetitiveRequest.cpp \
          process/conflicts/BasicConflict.cpp \
          process/conflicts/ResourceCompetitiveRequest.cpp   \
          process/conflicts/EvenConflict.cpp   \
          process/conflicts/WagesCompetitiveRequest.cpp   \
          process/conflicts/MarketConflict.cpp   \
          process/conflicts/MarketRequest.cpp   \
          process/conflicts/LocalMarketRequest.cpp   \
          process/conflicts/RecruitRequest.cpp   \
          process/conflicts/LocalRecruitOffer.cpp   \
          process/conflicts/NewRecruitRequest.cpp   \
          process/conflicts/TeachingOffer.cpp   \
          process/ProcessingMode.cpp   \
          process/OrderLine.cpp   \
          report/patterns/BinaryPattern.cpp \
          report/patterns/QuartenaryPattern.cpp    \
          report/patterns/TertiaryPattern.cpp   \
          report/patterns/UnaryPattern.cpp   \
          report/patterns/ReportPattern.cpp   \
          report/patterns/SimplePattern.cpp   \
          report/patterns/QuintenaryPattern.cpp   \
          report/Reporter.cpp   \
          report/ReportRecord.cpp   \
          report/TextsForReports.cpp   \
          GameInfo.cpp   \
          misc/FileParser.cpp   \
          misc/Parser.cpp   \
          misc/ReportPrinter.cpp   \
          misc/dgChannelBuf.cpp   \
          Global.cpp    \
					datatypes/attributes/BasicAttribute.cpp
					datatypes/attributes/EnchantmentAttribute.cpp
					implementation/actions/PickpocketActionRule.cpp \
          datatypes/elements/EnchantmentElement.cpp \
          datatypes/entities/EffectEntity.cpp \
          datatypes/entities/TokenEntity.cpp \
          datatypes/rules/ActionRule.cpp \
          datatypes/rules/EnchantmentRule.cpp \
          datatypes/strategies/ActionUsingStrategy.cpp   \
          datatypes/strategies/EnchantmentUsingStrategy.cpp   \

OBJECTS := $(patsubst %.cpp,%.o,$(wildcard $(SOURCES) ) )

############################### Rules  #######################################
%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@


default  : $(MODULE_NAME)


$(MODULE_NAME)	: $(OBJECTS) 
	  $(LD) -o $(MODULE_NAME) $(OBJECTS) 


clean   :
	$(RM) $(OBJECTS)
#	$(RM) $(MODULE_DIR)/*.o
	$(RM) depend*


############################### Dependencies list ############################

depend$(MODULE_NAME):
		 $(CXX) -MM  $(CXXFLAGS)  $(wildcard $(SOURCES) ) > $@

include	depend$(MODULE_NAME)

