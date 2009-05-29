######################## Internal Library  Definitions #################
BASE_DIR= .
MODULE_DIR= ../gamedata
MODULE_NAME = overlord
CXX = c++
LD = c++
CXXFLAGS = -DDEBUG -ggdb -W -Wall -Wno-unused -Wno-sign-compare $(INCLUDES)
# -O3
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
           -I$(BASE_DIR)/combat               \
           -I$(BASE_DIR)/combat/orders        \
           -I$(BASE_DIR)/combat/actions       \
           -I$(BASE_DIR)/process               \
           -I$(BASE_DIR)/process/orders        \
           -I$(BASE_DIR)/process/conflicts     \
           -I$(BASE_DIR)/report/patterns       \
           -I$(BASE_DIR)/report                \
           -I$(BASE_DIR)/misc                  \
           -I$(BASE_DIR)/implementation/actions \
           -I$(BASE_DIR)/implementation/combat

######################## Compilation dependency #########################

SOURCES = turn.cpp \
          DataManipulator.cpp \
          GameConfig.cpp \
          Global.cpp \
          combat/BasicCombatEngine.cpp \
          combat/BasicCombatManager.cpp \
          combat/BasicCombatRequest.cpp \
          combat/CombatManager.cpp \
          combat/CombatTactics.cpp \
          combat/GuardingRequest.cpp \
          combat/PillageRequest.cpp \
	  combat/BattleUnit.cpp \
	  combat/BattleInstance.cpp \
	  combat/CombatEngine.cpp \
	  combat/CombatOrderProcessor.cpp \
	  combat/BattleField.cpp \
	  combat/BattleEntity.cpp \
	  combat/orders/ParryCombatOrder.cpp \
	  combat/orders/MoveCombatOrder.cpp \
	  combat/orders/FleeCombatOrder.cpp \
	  combat/orders/CombatOrderPrototype.cpp \
	  combat/orders/ChargeCombatOrder.cpp \
	  combat/orders/AdvanceCombatOrder.cpp \
	  combat/orders/MeleeCombatOrder.cpp \
	  combat/orders/FireCombatOrder.cpp \
	  combat/orders/FlankCombatOrder.cpp \
	  combat/orders/RangedCombatOrder.cpp \
	  combat/orders/RetreatCombatOrder.cpp \
	  combat/orders/StandCombatOrder.cpp \
	  combat/orders/SupportCombatOrder.cpp \
	  combat/orders/SweepCombatOrder.cpp \
	  combat/orders/UseCombatOrder.cpp \
	  combat/orders/ApplyCombatOrder.cpp \
	  combat/actions/MeleeCombatAction.cpp \
	  combat/actions/RangedCombatAction.cpp \
	  combat/actions/SpecialCombatAction.cpp \
          datatypes/attributes/BasicAttribute.cpp \
          datatypes/attributes/EnchantmentAttribute.cpp \
          datatypes/attributes/InventoryAttribute.cpp \
          datatypes/attributes/OwnershipPolicyAttribute.cpp \
          datatypes/attributes/PermissionAttribute.cpp \
          datatypes/attributes/MovementBonusAttribute.cpp \
          datatypes/attributes/SkillBonusAttribute.cpp \
          datatypes/attributes/SkillBonusComboAttribute.cpp \
          datatypes/attributes/SkillsAttribute.cpp \
          datatypes/attributes/TitlesAttribute.cpp \
          datatypes/AbstractData.cpp \
          datatypes/BasicData.cpp \
          datatypes/GameData.cpp \
          datatypes/IntegerData.cpp \
          datatypes/PrototypeManager.cpp \
          datatypes/RationalNumber.cpp \
          datatypes/StringData.cpp \
          datatypes/collections/BasicCollection.cpp \
          datatypes/collections/BasicEntitiesCollection.cpp \
          datatypes/collections/BasicRulesCollection.cpp \
          datatypes/collections/BasicVarietiesCollection.cpp \
          datatypes/collections/DataStorageHandler.cpp \
          datatypes/collections/EntitiesCollection.cpp \
          datatypes/collections/MovementMode.cpp \
          datatypes/collections/OrderPrototypesCollection.cpp \
          datatypes/collections/RulesCollection.cpp \
          datatypes/collections/VarietiesCollection.cpp \
          datatypes/conditions/BasicCondition.cpp \
          datatypes/conditions/ObservationCondition.cpp \
          datatypes/conditions/SkillCondition.cpp \
          datatypes/elements/BonusElement.cpp \
          datatypes/elements/ConstructionWorksElement.cpp \
          datatypes/elements/EnchantmentElement.cpp \
          datatypes/elements/InventoryElement.cpp \
          datatypes/elements/ItemElement.cpp \
	  datatypes/elements/MeleeAttackElement.cpp \
          datatypes/elements/MovementElement.cpp \
          datatypes/elements/MovementBonusElement.cpp \
          datatypes/elements/RaceElement.cpp \
          datatypes/elements/ReportElement.cpp \
          datatypes/elements/ResourceElement.cpp \
          datatypes/elements/SkillElement.cpp \
          datatypes/elements/SkillUseElement.cpp \
          datatypes/elements/StanceElement.cpp \
          datatypes/elements/TitleElement.cpp \
          datatypes/elements/ToolUseElement.cpp \
          datatypes/elements/TravelElement.cpp \
          datatypes/elements/TreeBonusElement.cpp \
          datatypes/elements/FactionPermissionElement.cpp \
          datatypes/elements/PermissionElement.cpp \
          datatypes/entities/TokenEntity.cpp \
          datatypes/entities/ConstructionEntity.cpp \
          datatypes/entities/EffectEntity.cpp \
          datatypes/entities/Entity.cpp \
          datatypes/entities/FactionEntity.cpp \
          datatypes/entities/LocationEntity.cpp \
          datatypes/entities/UnitEntity.cpp \
          datatypes/misc/BasicExit.cpp \
          datatypes/misc/EntityStatistics.cpp \
          datatypes/misc/ExplicitExit.cpp \
          datatypes/misc/NewEntityPlaceholder.cpp \
          datatypes/misc/OwnershipPolicy.cpp \
          datatypes/rules/RuleIndex.cpp \
          datatypes/rules/ActionRule.cpp \
          datatypes/rules/ConstructionRule.cpp \
          datatypes/rules/CreatureRaceRule.cpp \
          datatypes/rules/EnchantmentRule.cpp \
          datatypes/rules/FollowerRaceRule.cpp \
          datatypes/rules/ItemRule.cpp \
          datatypes/rules/LeaderRaceRule.cpp \
          datatypes/rules/RaceRule.cpp \
          datatypes/rules/Rule.cpp \
          datatypes/rules/SkillRule.cpp \
          datatypes/rules/TerrainRule.cpp \
          datatypes/rules/TitleRule.cpp \
          datatypes/rules/WeatherRule.cpp \
          datatypes/rules/SeasonRule.cpp \
          datatypes/strategies/ActionUsingStrategy.cpp \
          datatypes/strategies/BasicLearningStrategy.cpp \
          datatypes/strategies/BasicProductionStrategy.cpp \
          datatypes/strategies/BasicUsingStrategy.cpp \
          datatypes/strategies/BuildUsingStrategy.cpp \
          datatypes/strategies/CombatActionStrategy.cpp \
          datatypes/strategies/CombatUsingStrategy.cpp \
          datatypes/strategies/ConstructionUsingStrategy.cpp \
          datatypes/strategies/CraftUsingStrategy.cpp \
          datatypes/strategies/CreatureLearningStrategy.cpp \
          datatypes/strategies/EnchantmentUsingStrategy.cpp \
          datatypes/strategies/HarvestUsingStrategy.cpp \
          datatypes/strategies/MagicLearningStrategy.cpp \
          datatypes/strategies/MarketStrategy.cpp \
          datatypes/strategies/NormalLearningStrategy.cpp \
          datatypes/strategies/Strategy.cpp \
          datatypes/strategies/SummonUsingStrategy.cpp \
          datatypes/varieties/ConstructionWorksVariety.cpp \
          datatypes/varieties/DirectionVariety.cpp \
          datatypes/varieties/EquipmentSlotVariety.cpp \
          datatypes/varieties/MovementVariety.cpp \
          datatypes/varieties/StanceVariety.cpp \
          datatypes/varieties/Variety.cpp \
          datatypes/varieties/CombatFileVariety.cpp \
          datatypes/varieties/CombatMoveVariety.cpp \
          datatypes/varieties/CombatRankVariety.cpp \
          datatypes/varieties/CombatStanceVariety.cpp \
          datatypes/varieties/CombatTargetVariety.cpp \
          implementation/actions/PickpocketActionRule.cpp \
          implementation/combat/HeavinessCombatAction.cpp \
          implementation/combat/EnchantCombatAction.cpp \
          implementation/combat/SummonCombatAction.cpp \
          misc/FileParser.cpp \
          misc/LineParser.cpp \
          misc/ReportPrinter.cpp \
          misc/dgChannelBuf.cpp \
          process/conflicts/BasicRequest.cpp \
          process/conflicts/BasicCompetitiveRequest.cpp \
          process/conflicts/BasicConflict.cpp \
          process/conflicts/EvenConflict.cpp \
          process/conflicts/LocalMarketRequest.cpp \
          process/conflicts/LocalRecruitOffer.cpp \
          process/conflicts/MarketConflict.cpp \
          process/conflicts/MarketRequest.cpp \
          process/conflicts/NewRecruitRequest.cpp \
          process/conflicts/RecruitRequest.cpp \
          process/conflicts/ResourceCompetitiveRequest.cpp \
          process/conflicts/TeachingOffer.cpp \
          process/conflicts/WagesCompetitiveRequest.cpp \
          process/OrderLine.cpp \
          process/ProcessingMode.cpp \
          process/OrderProcessor.cpp \
          process/CombatOrderLine.cpp \
          process/orders/TacticOrder.cpp \
          process/orders/AcceptOrder.cpp \
          process/orders/AtOrder.cpp \
          process/orders/AttackOrder.cpp \
          process/orders/BasicOrderSynchronizationRequest.cpp \
          process/orders/BestowOrder.cpp \
          process/orders/BuildOrder.cpp \
          process/orders/BuyOrder.cpp \
          process/orders/CancelOrder.cpp \
          process/orders/CaravanOrder.cpp \
          process/orders/ChristenOrder.cpp \
          process/orders/ClaimOrder.cpp \
          process/orders/CombatOrder.cpp \
          process/orders/DayOrder.cpp \
          process/orders/DescribeOrder.cpp \
          process/orders/DisbandOrder.cpp \
          process/orders/DropOrder.cpp \
          process/orders/EjectOrder.cpp \
          process/orders/EmailOrder.cpp \
          process/orders/EnterOrder.cpp \
          process/orders/EquipOrder.cpp \
          process/orders/ExitOrder.cpp \
          process/orders/ForgetOrder.cpp \
          process/orders/GetOrder.cpp \
          process/orders/GiveOrder.cpp \
          process/orders/GuardOrder.cpp \
          process/orders/HaveOrder.cpp \
          process/orders/LeaderOrder.cpp \
          process/orders/LeadingOrder.cpp \
          process/orders/LeaveOrder.cpp \
          process/orders/MarchOrder.cpp \
          process/orders/MergeOrder.cpp \
          process/orders/MoveOrder.cpp \
          process/orders/NameOrder.cpp \
          process/orders/OathOrder.cpp \
          process/orders/OrderPrototype.cpp \
          process/orders/PasswordOrder.cpp \
          process/orders/PatrolOrder.cpp \
          process/orders/PillageOrder.cpp \
          process/orders/PromoteOrder.cpp \
          process/orders/RecruitOrder.cpp \
          process/orders/ReshowOrder.cpp \
          process/orders/ResignOrder.cpp \
          process/orders/RetreatOrder.cpp \
          process/orders/SeeOrder.cpp \
          process/orders/SellOrder.cpp \
          process/orders/SettingOrder.cpp \
          process/orders/SizeOrder.cpp \
          process/orders/SkillOrder.cpp \
          process/orders/SplitOrder.cpp \
          process/orders/StackOrder.cpp \
          process/orders/StanceOrder.cpp \
          process/orders/StayOrder.cpp \
          process/orders/StudyOrder.cpp \
          process/orders/SwapOrder.cpp \
          process/orders/SwapOrderRequest.cpp \
          process/orders/SynchroOrder.cpp \
          process/orders/SynchroOrderRequest.cpp \
          process/orders/TargetOrder.cpp \
          process/orders/TeachOrder.cpp \
          process/orders/UnstackOrder.cpp \
          process/orders/UseOrder.cpp \
          process/orders/WaitOrder.cpp \
          process/orders/WithdrawOrder.cpp \
          process/orders/WorkOrder.cpp \
          process/orders/YieldOrder.cpp \
          report/patterns/BinaryMessage.cpp \
	  report/patterns/CombatParticipantMessage.cpp \
          report/patterns/CombatAttackMessage.cpp \
          report/patterns/CombatLootMessage.cpp \
          report/patterns/QuartenaryMessage.cpp \
          report/patterns/QuintenaryMessage.cpp \
          report/patterns/ReportMessage.cpp \
          report/patterns/SimpleMessage.cpp \
          report/patterns/TertiaryMessage.cpp \
          report/patterns/UnaryMessage.cpp \
          report/patterns/EndLineMessage.cpp \
          report/CombatReport.cpp \
          report/ReportPattern.cpp \
          report/ReportRecord.cpp \
          report/TextsForReports.cpp \

OBJECTS := $(patsubst %.cpp,%.o,$(wildcard $(SOURCES) ) )
############################### Rules  #######################################
%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@


default  : $(MODULE_NAME)


$(MODULE_NAME)	: $(OBJECTS) 
	  $(LD) -o $(MODULE_DIR)/$(MODULE_NAME) $(OBJECTS) 


clean   :
	$(RM) $(OBJECTS)
#	$(RM) $(MODULE_DIR)/*.o
	$(RM) depend*
############################### Explicit Targets  #######################################
Global.o : Global.cpp
	$(CXX) -c $(CXXFLAGS) Global.cpp
DataManipulator.o : DataManipulator.cpp 
	$(CXX) -c $(CXXFLAGS)  DataManipulator.cpp
GameConfig.o : GameConfig.cpp 
	$(CXX) -c $(CXXFLAGS) GameConfig.cpp 
BasicCombatEngine.o : 
	$(CXX) -c $(CXXFLAGS) combat/BasicCombatEngine.cpp  
BasicCombatManager.o:
	$(CXX) -c $(CXXFLAGS) combat/BasicCombatManager.cpp
BasicCombatRequest.o:
	$(CXX) -c $(CXXFLAGS) combat/BasicCombatRequest.cpp 
CombatManager.o :
	$(CXX) -c $(CXXFLAGS) combat/CombatManager.cpp 
CombatTactics.o :
	$(CXX) -c $(CXXFLAGS) combat/CombatTactics.cpp 
GuardingRequest.o :
	$(CXX) -c $(CXXFLAGS) combat/GuardingRequest.cpp 
PillageRequest.o :
	$(CXX) -c $(CXXFLAGS) combat/PillageRequest.cpp 
BattleUnit.o :
	$(CXX) -c $(CXXFLAGS) combat/BattleUnit.cpp 
BattleInstance.o :
	$(CXX) -c $(CXXFLAGS) combat/BattleInstance.cpp 
CombatEngine.o :
	$(CXX) -c $(CXXFLAGS) combat/CombatEngine.cpp 
CombatOrderProcessor.o :
	$(CXX) -c $(CXXFLAGS) combat/CombatOrderProcessor.cpp 
BattleField.o :
	$(CXX) -c $(CXXFLAGS) combat/BattleField.cpp
BattleEntity.o : 
	$(CXX) -c $(CXXFLAGS) combat/BattleEntity.cpp 
ParryCombatOrder.o :
	$(CXX) -c $(CXXFLAGS) combat/orders/ParryCombatOrder.cpp 
MoveCombatOrder.o :
	$(CXX) -c $(CXXFLAGS) combat/orders/MoveCombatOrder.cpp 
FleeCombatOrder.o :
	$(CXX) -c $(CXXFLAGS) combat/orders/FleeCombatOrder.cpp 
CombatOrderPrototype.o :
	$(CXX) -c $(CXXFLAGS) combat/orders/CombatOrderPrototype.cpp 
ChargeCombatOrder.o :
	$(CXX) -c $(CXXFLAGS) combat/orders/ChargeCombatOrder.cpp 
AdvanceCombatOrder.o :
	$(CXX) -c $(CXXFLAGS) combat/orders/AdvanceCombatOrder.cpp 
MeleeCombatOrder.o :
	$(CXX) -c $(CXXFLAGS) combat/orders/MeleeCombatOrder.cpp 
FireCombatOrder.o :
	$(CXX) -c $(CXXFLAGS) combat/orders/FireCombatOrder.cpp
FlankCombatOrder.o :
	$(CXX) -c $(CXXFLAGS) combat/orders/FlankCombatOrder.cpp 
RangedCombatOrder.o :
	$(CXX) -c $(CXXFLAGS) combat/orders/RangedCombatOrder.cpp
RetreatCombatOrder.o : 
	$(CXX) -c $(CXXFLAGS) combat/orders/RetreatCombatOrder.cpp 
StandCombatOrder.o :
	$(CXX) -c $(CXXFLAGS) combat/orders/StandCombatOrder.cpp 
SupportCombatOrder.o :
	$(CXX) -c $(CXXFLAGS) combat/orders/SupportCombatOrder.cpp 
SweepCombatOrder.o :
	$(CXX) -c $(CXXFLAGS) combat/orders/SweepCombatOrder.cpp 
UseCombatOrder.o :
	$(CXX) -c $(CXXFLAGS) combat/orders/UseCombatOrder.cpp 
ApplyCombatOrder.o :
	$(CXX) -c $(CXXFLAGS) combat/orders/ApplyCombatOrder.cpp
MeleeCombatAction.o : 
	$(CXX) -c $(CXXFLAGS) combat/actions/MeleeCombatAction.cpp 
RangedCombatAction.o :
	$(CXX) -c $(CXXFLAGS) combat/actions/RangedCombatAction.cpp 
SpecialCombatAction.o :
	$(CXX) -c $(CXXFLAGS) combat/actions/SpecialCombatAction.cpp 
BasicAttribute.o :
	$(CXX) -c $(CXXFLAGS) datatypes/attributes/BasicAttribute.cpp 
EnchantmentAttribute.o :
	$(CXX) -c $(CXXFLAGS) datatypes/attributes/EnchantmentAttribute.cpp 
InventoryAttribute.o :
	$(CXX) -c $(CXXFLAGS) datatypes/attributes/InventoryAttribute.cpp 
OwnershipPolicyAttribute.o :
	$(CXX) -c $(CXXFLAGS) datatypes/attributes/OwnershipPolicyAttribute.cpp 
PermissionAttribute.o :
	$(CXX) -c $(CXXFLAGS) datatypes/attributes/PermissionAttribute.cpp 
MovementBonusAttribute.o :
	$(CXX) -c $(CXXFLAGS) datatypes/attributes/MovementBonusAttribute.cpp 
SkillBonusAttribute.o :
	$(CXX) -c $(CXXFLAGS) datatypes/attributes/SkillBonusAttribute.cpp 
SkillBonusComboAttribute.o :
	$(CXX) -c $(CXXFLAGS) datatypes/attributes/SkillBonusComboAttribute.cpp 
SkillsAttribute.o :
	$(CXX) -c $(CXXFLAGS) datatypes/attributes/SkillsAttribute.cpp 
TitlesAttribute.o :
	$(CXX) -c $(CXXFLAGS) datatypes/attributes/TitlesAttribute.cpp 
AbstractData.o :
	$(CXX) -c $(CXXFLAGS) datatypes/AbstractData.cpp 
BasicData.o :
	$(CXX) -c $(CXXFLAGS) datatypes/BasicData.cpp 
GameData.o :
	$(CXX) -c $(CXXFLAGS) datatypes/GameData.cpp 
IntegerData.o :
	$(CXX) -c $(CXXFLAGS) datatypes/IntegerData.cpp 
PrototypeManager.o :
	$(CXX) -c $(CXXFLAGS) datatypes/PrototypeManager.cpp 
RationalNumber.o :
	$(CXX) -c $(CXXFLAGS) datatypes/RationalNumber.cpp 
StringData.o :
	$(CXX) -c $(CXXFLAGS) datatypes/StringData.cpp 
BasicCollection.o :
	$(CXX) -c $(CXXFLAGS) datatypes/collections/BasicCollection.cpp 
BasicEntitiesCollection.o :
	$(CXX) -c $(CXXFLAGS) datatypes/collections/BasicEntitiesCollection.cpp 
BasicRulesCollection.o :
	$(CXX) -c $(CXXFLAGS) datatypes/collections/BasicRulesCollection.cpp
BasicVarietiesCollection.o :
	$(CXX) -c $(CXXFLAGS) datatypes/collections/BasicVarietiesCollection.cpp
DataStorageHandler.o :
	$(CXX) -c $(CXXFLAGS) datatypes/collections/DataStorageHandler.cpp
EntitiesCollection.o :
	$(CXX) -c $(CXXFLAGS) datatypes/collections/EntitiesCollection.cpp
MovementMode.o :
	$(CXX) -c $(CXXFLAGS) datatypes/collections/MovementMode.cpp
OrderPrototypesCollection.o :
	$(CXX) -c $(CXXFLAGS) datatypes/collections/OrderPrototypesCollection.cpp
RulesCollection.o :
	$(CXX) -c $(CXXFLAGS) datatypes/collections/RulesCollection.cpp
VarietiesCollection.o :
	$(CXX) -c $(CXXFLAGS) datatypes/collections/VarietiesCollection.cpp
BasicCondition.o :
	$(CXX) -c $(CXXFLAGS) datatypes/conditions/BasicCondition.cpp
ObservationCondition.o :
	$(CXX) -c $(CXXFLAGS) datatypes/conditions/ObservationCondition.cpp
SkillCondition.o :
	$(CXX) -c $(CXXFLAGS) datatypes/conditions/SkillCondition.cpp
BonusElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/BonusElement.cpp
ConstructionWorksElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/ConstructionWorksElement.cpp
EnchantmentElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/EnchantmentElement.cpp
InventoryElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/InventoryElement.cpp
ItemElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/ItemElement.cpp
MeleeAttackElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/MeleeAttackElement.cpp
MovementElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/MovementElement.cpp
MovementBonusElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/MovementBonusElement.cpp
RaceElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/RaceElement.cpp
ReportElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/ReportElement.cpp
ResourceElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/ResourceElement.cpp
SkillElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/SkillElement.cpp
SkillUseElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/SkillUseElement.cpp
StanceElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/StanceElement.cpp
TitleElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/TitleElement.cpp
ToolUseElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/ToolUseElement.cpp
TravelElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/TravelElement.cpp
TreeBonusElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/TreeBonusElement.cpp
FactionPermissionElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/FactionPermissionElement.cpp
PermissionElement.o :
	$(CXX) -c $(CXXFLAGS) datatypes/elements/PermissionElement.cpp
TokenEntity.o :
	$(CXX) -c $(CXXFLAGS) datatypes/entities/TokenEntity.cpp
ConstructionEntity.o :
	$(CXX) -c $(CXXFLAGS) datatypes/entities/ConstructionEntity.cpp
EffectEntity.o :
	$(CXX) -c $(CXXFLAGS) datatypes/entities/EffectEntity.cpp
Entity.o :
	$(CXX) -c $(CXXFLAGS) datatypes/entities/Entity.cpp
FactionEntity.o :
	$(CXX) -c $(CXXFLAGS) datatypes/entities/FactionEntity.cpp
LocationEntity.o :
	$(CXX) -c $(CXXFLAGS) datatypes/entities/LocationEntity.cpp
UnitEntity.o :
	$(CXX) -c $(CXXFLAGS) datatypes/entities/UnitEntity.cpp
BasicExit.o :
	$(CXX) -c $(CXXFLAGS) datatypes/misc/BasicExit.cpp
EntityStatistics.o :
	$(CXX) -c $(CXXFLAGS) datatypes/misc/EntityStatistics.cpp
ExplicitExit.o :
	$(CXX) -c $(CXXFLAGS) datatypes/misc/ExplicitExit.cpp
NewEntityPlaceholder.o :
	$(CXX) -c $(CXXFLAGS) datatypes/misc/NewEntityPlaceholder.cpp
OwnershipPolicy.o :
	$(CXX) -c $(CXXFLAGS) datatypes/misc/OwnershipPolicy.cpp
RuleIndex.o :
	$(CXX) -c $(CXXFLAGS) datatypes/rules/RuleIndex.cpp
ActionRule.o :
	$(CXX) -c $(CXXFLAGS) datatypes/rules/ActionRule.cpp
ConstructionRule.o :
	$(CXX) -c $(CXXFLAGS) datatypes/rules/ConstructionRule.cpp
CreatureRaceRule.o :
	$(CXX) -c $(CXXFLAGS) datatypes/rules/CreatureRaceRule.cpp
EnchantmentRule.o :
	$(CXX) -c $(CXXFLAGS) datatypes/rules/EnchantmentRule.cpp
FollowerRaceRule.o :
	$(CXX) -c $(CXXFLAGS) datatypes/rules/FollowerRaceRule.cpp
ItemRule.o :
	$(CXX) -c $(CXXFLAGS) datatypes/rules/ItemRule.cpp
LeaderRaceRule.o :
	$(CXX) -c $(CXXFLAGS) datatypes/rules/LeaderRaceRule.cpp
RaceRule.o :
	$(CXX) -c $(CXXFLAGS) datatypes/rules/RaceRule.cpp
Rule.o :
	$(CXX) -c $(CXXFLAGS) datatypes/rules/Rule.cpp
SkillRule.o :
	$(CXX) -c $(CXXFLAGS) datatypes/rules/SkillRule.cpp
TerrainRule.o :
	$(CXX) -c $(CXXFLAGS) datatypes/rules/TerrainRule.cpp
TitleRule.o :
	$(CXX) -c $(CXXFLAGS) datatypes/rules/TitleRule.cpp
WeatherRule.o :
	$(CXX) -c $(CXXFLAGS) datatypes/rules/WeatherRule.cpp
SeasonRule.o :
	$(CXX) -c $(CXXFLAGS) datatypes/rules/SeasonRule.cpp
ActionUsingStrategy.o :
	$(CXX) -c $(CXXFLAGS) datatypes/strategies/ActionUsingStrategy.cpp
BasicLearningStrategy.o :
	$(CXX) -c $(CXXFLAGS) datatypes/strategies/BasicLearningStrategy.cpp
BasicProductionStrategy.o :
	$(CXX) -c $(CXXFLAGS) datatypes/strategies/BasicProductionStrategy.cpp
BasicUsingStrategy.o :
	$(CXX) -c $(CXXFLAGS) datatypes/strategies/BasicUsingStrategy.cpp
BuildUsingStrategy.o :
	$(CXX) -c $(CXXFLAGS) datatypes/strategies/BuildUsingStrategy.cpp
CombatActionStrategy.o :
	$(CXX) -c $(CXXFLAGS) datatypes/strategies/CombatActionStrategy.cpp
CombatUsingStrategy.o :
	$(CXX) -c $(CXXFLAGS) datatypes/strategies/CombatUsingStrategy.cpp
ConstructionUsingStrategy.o :
	$(CXX) -c $(CXXFLAGS) datatypes/strategies/ConstructionUsingStrategy.cpp
CraftUsingStrategy.o :
	$(CXX) -c $(CXXFLAGS) datatypes/strategies/CraftUsingStrategy.cpp
CreatureLearningStrategy.o :
	$(CXX) -c $(CXXFLAGS) datatypes/strategies/CreatureLearningStrategy.cpp
EnchantmentUsingStrategy.o :
	$(CXX) -c $(CXXFLAGS) datatypes/strategies/EnchantmentUsingStrategy.cpp
HarvestUsingStrategy.o :
	$(CXX) -c $(CXXFLAGS) datatypes/strategies/HarvestUsingStrategy.cpp
MagicLearningStrategy.o :
	$(CXX) -c $(CXXFLAGS) datatypes/strategies/MagicLearningStrategy.cpp
MarketStrategy.o :
	$(CXX) -c $(CXXFLAGS) datatypes/strategies/MarketStrategy.cpp
NormalLearningStrategy.o :
	$(CXX) -c $(CXXFLAGS) datatypes/strategies/NormalLearningStrategy.cpp
Strategy.o :
	$(CXX) -c $(CXXFLAGS) datatypes/strategies/Strategy.cpp
SummonUsingStrategy.o :
	$(CXX) -c $(CXXFLAGS) datatypes/strategies/SummonUsingStrategy.cpp
ConstructionWorksVariety.o :
	$(CXX) -c $(CXXFLAGS) datatypes/varieties/ConstructionWorksVariety.cpp
DirectionVariety.o :
	$(CXX) -c $(CXXFLAGS) datatypes/varieties/DirectionVariety.cpp
EquipmentSlotVariety.o :
	$(CXX) -c $(CXXFLAGS) datatypes/varieties/EquipmentSlotVariety.cpp
MovementVariety.o :
	$(CXX) -c $(CXXFLAGS) datatypes/varieties/MovementVariety.cpp
StanceVariety.o :
	$(CXX) -c $(CXXFLAGS) datatypes/varieties/StanceVariety.cpp
Variety.o :
	$(CXX) -c $(CXXFLAGS) datatypes/varieties/Variety.cpp
CombatFileVariety.o :
	$(CXX) -c $(CXXFLAGS) datatypes/varieties/CombatFileVariety.cpp
CombatMoveVariety.o :
	$(CXX) -c $(CXXFLAGS) datatypes/varieties/CombatMoveVariety.cpp
CombatRankVariety.o :
	$(CXX) -c $(CXXFLAGS) datatypes/varieties/CombatRankVariety.cpp
CombatStanceVariety.o :
	$(CXX) -c $(CXXFLAGS) datatypes/varieties/CombatStanceVariety.cpp
CombatTargetVariety.o :
	$(CXX) -c $(CXXFLAGS) datatypes/varieties/CombatTargetVariety.cpp
PickpocketActionRule.o :
	$(CXX) -c $(CXXFLAGS) implementation/actions/PickpocketActionRule.cpp
HeavinessCombatAction.o :
	$(CXX) -c $(CXXFLAGS) implementation/combat/HeavinessCombatAction.cpp
EnchantCombatAction.o :
	$(CXX) -c $(CXXFLAGS) implementation/combat/EnchantCombatAction.cpp
SummonCombatAction.o :
	$(CXX) -c $(CXXFLAGS) implementation/combat/SummonCombatAction.cpp
FileParser.o :
	$(CXX) -c $(CXXFLAGS) misc/FileParser.cpp
LineParser.o :
	$(CXX) -c $(CXXFLAGS) misc/LineParser.cpp
ReportPrinter.o :
	$(CXX) -c $(CXXFLAGS) misc/ReportPrinter.cpp
dgChannelBuf.o :
	$(CXX) -c $(CXXFLAGS) misc/dgChannelBuf.cpp
BasicRequest.o :
	$(CXX) -c $(CXXFLAGS) process/conflicts/BasicRequest.cpp
BasicCompetitiveRequest.o :
	$(CXX) -c $(CXXFLAGS) process/conflicts/BasicCompetitiveRequest.cpp
BasicConflict.o :
	$(CXX) -c $(CXXFLAGS) process/conflicts/BasicConflict.cpp
EvenConflict.o :
	$(CXX) -c $(CXXFLAGS) process/conflicts/EvenConflict.cpp
LocalMarketRequest.o :
	$(CXX) -c $(CXXFLAGS) process/conflicts/LocalMarketRequest.cpp
LocalRecruitOffer.o :
	$(CXX) -c $(CXXFLAGS) process/conflicts/LocalRecruitOffer.cpp
MarketConflict.o :
	$(CXX) -c $(CXXFLAGS) process/conflicts/MarketConflict.cpp
MarketRequest.o :
	$(CXX) -c $(CXXFLAGS) process/conflicts/MarketRequest.cpp
NewRecruitRequest.o :
	$(CXX) -c $(CXXFLAGS) process/conflicts/NewRecruitRequest.cpp
RecruitRequest.o :
	$(CXX) -c $(CXXFLAGS) process/conflicts/RecruitRequest.cpp
ResourceCompetitiveRequest.o :
	$(CXX) -c $(CXXFLAGS) process/conflicts/ResourceCompetitiveRequest.cpp
TeachingOffer.o :
	$(CXX) -c $(CXXFLAGS) process/conflicts/TeachingOffer.cpp
WagesCompetitiveRequest.o :
	$(CXX) -c $(CXXFLAGS) process/conflicts/WagesCompetitiveRequest.cpp
OrderLine.o :
	$(CXX) -c $(CXXFLAGS) process/OrderLine.cpp
ProcessingMode.o :
	$(CXX) -c $(CXXFLAGS) process/ProcessingMode.cpp
OrderProcessor.o :
	$(CXX) -c $(CXXFLAGS) process/OrderProcessor.cpp
CombatOrderLine.o :
	$(CXX) -c $(CXXFLAGS) process/CombatOrderLine.cpp
TacticOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/TacticOrder.cpp
AcceptOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/AcceptOrder.cpp
AtOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/AtOrder.cpp
AttackOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/AttackOrder.cpp
BasicOrderSynchronizationRequest.o :
	$(CXX) -c $(CXXFLAGS) process/orders/BasicOrderSynchronizationRequest.cpp
BestowOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/BestowOrder.cpp
BuildOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/BuildOrder.cpp
BuyOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/BuyOrder.cpp
CancelOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/CancelOrder.cpp
CaravanOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/CaravanOrder.cpp
ChristenOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/ChristenOrder.cpp
ClaimOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/ClaimOrder.cpp
CombatOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/CombatOrder.cpp
DayOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/DayOrder.cpp
DescribeOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/DescribeOrder.cpp
DisbandOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/DisbandOrder.cpp
DropOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/DropOrder.cpp
EjectOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/EjectOrder.cpp
EmailOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/EmailOrder.cpp
EnterOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/EnterOrder.cpp
EquipOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/EquipOrder.cpp
ExitOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/ExitOrder.cpp
ForgetOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/ForgetOrder.cpp
GetOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/GetOrder.cpp
GiveOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/GiveOrder.cpp
GuardOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/GuardOrder.cpp
HaveOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/HaveOrder.cpp
LeaderOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/LeaderOrder.cpp
LeadingOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/LeadingOrder.cpp
LeaveOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/LeaveOrder.cpp
MarchOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/MarchOrder.cpp
MergeOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/MergeOrder.cpp
MoveOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/MoveOrder.cpp
NameOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/NameOrder.cpp
OathOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/OathOrder.cpp
OrderPrototype.o :
	$(CXX) -c $(CXXFLAGS) process/orders/OrderPrototype.cpp
PasswordOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/PasswordOrder.cpp
PatrolOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/PatrolOrder.cpp
PillageOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/PillageOrder.cpp
PromoteOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/PromoteOrder.cpp
RecruitOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/RecruitOrder.cpp
ReshowOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/ReshowOrder.cpp
ResignOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/ResignOrder.cpp
RetreatOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/RetreatOrder.cpp
SeeOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/SeeOrder.cpp
SellOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/SellOrder.cpp
SettingOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/SettingOrder.cpp
SizeOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/SizeOrder.cpp
SkillOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/SkillOrder.cpp
SplitOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/SplitOrder.cpp
StackOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/StackOrder.cpp
StanceOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/StanceOrder.cpp
StayOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/StayOrder.cpp
StudyOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/StudyOrder.cpp
SwapOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/SwapOrder.cpp
SwapOrderRequest.o :
	$(CXX) -c $(CXXFLAGS) process/orders/SwapOrderRequest.cpp
SynchroOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/SynchroOrder.cpp
SynchroOrderRequest.o :
	$(CXX) -c $(CXXFLAGS) process/orders/SynchroOrderRequest.cpp
TargetOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/TargetOrder.cpp
TeachOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/TeachOrder.cpp
UnstackOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/UnstackOrder.cpp
UseOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/UseOrder.cpp
WaitOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/WaitOrder.cpp
WithdrawOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/WithdrawOrder.cpp
WorkOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/WorkOrder.cpp
YieldOrder.o :
	$(CXX) -c $(CXXFLAGS) process/orders/YieldOrder.cpp
BinaryMessage.o :
	$(CXX) -c $(CXXFLAGS) report/patterns/BinaryMessage.cpp
CombatParticipantMessage.o :
	$(CXX) -c $(CXXFLAGS) report/patterns/CombatParticipantMessage.cpp
CombatAttackMessage.o :
	$(CXX) -c $(CXXFLAGS) report/patterns/CombatAttackMessage.cpp
CombatLootMessage.o :
	$(CXX) -c $(CXXFLAGS) report/patterns/CombatLootMessage.cpp
QuartenaryMessage.o :
	$(CXX) -c $(CXXFLAGS) report/patterns/QuartenaryMessage.cpp
QuintenaryMessage.o :
	$(CXX) -c $(CXXFLAGS) report/patterns/QuintenaryMessage.cpp
ReportMessage.o :
	$(CXX) -c $(CXXFLAGS) report/patterns/ReportMessage.cpp
SimpleMessage.o :
	$(CXX) -c $(CXXFLAGS) report/patterns/SimpleMessage.cpp
TertiaryMessage.o :
	$(CXX) -c $(CXXFLAGS) report/patterns/TertiaryMessage.cpp
UnaryMessage.o :
	$(CXX) -c $(CXXFLAGS) report/patterns/UnaryMessage.cpp
EndLineMessage.o :
	$(CXX) -c $(CXXFLAGS) report/patterns/EndLineMessage.cpp
CombatReport.o :
	$(CXX) -c $(CXXFLAGS) report/CombatReport.cpp
ReportPattern.o :
	$(CXX) -c $(CXXFLAGS) report/ReportPattern.cpp 
ReportRecord.o :
	$(CXX) -c $(CXXFLAGS) report/ReportRecord.cpp 
TextsForReports.o :
	$(CXX) -c $(CXXFLAGS) report/TextsForReports.cpp 
############################### Dependencies list ############################

depend$(MODULE_NAME):
		 $(CXX) -MM  $(CXXFLAGS)  $(wildcard $(SOURCES) ) > $@

include	depend$(MODULE_NAME)

