######################## Compilation dependency #########################
ENGINE_HEADERS =    GameConfig.h \
    $${ENGINE_DIR}/GameFacade.h \
    $${ENGINE_DIR}/misc/dgChannelBuf.h \
    $${ENGINE_DIR}/misc/SystemTypes.h \
    $${ENGINE_DIR}/misc/Symbols.h \
    $${ENGINE_DIR}/misc/ReportPrinter.h \
    $${ENGINE_DIR}/misc/OverlordTypes.h \
    $${ENGINE_DIR}/misc/LineParser.h \
    $${ENGINE_DIR}/misc/FileParser.h \
    $${ENGINE_DIR}/datatypes/StringData.h \
    $${ENGINE_DIR}/datatypes/RationalNumber.h \
    $${ENGINE_DIR}/datatypes/PrototypeManager.h \
    $${ENGINE_DIR}/datatypes/IntegerData.h \
    $${ENGINE_DIR}/datatypes/GameData.h \
    $${ENGINE_DIR}/datatypes/AbstractData.h \
    $${ENGINE_DIR}/datatypes/elements/WeatherElement.h \
    $${ENGINE_DIR}/datatypes/elements/TreeBonusElement.h \
    $${ENGINE_DIR}/datatypes/elements/TravelElement.h \
    $${ENGINE_DIR}/datatypes/elements/ToolUseElement.h \
    $${ENGINE_DIR}/datatypes/elements/TitleElement.h \
    $${ENGINE_DIR}/datatypes/elements/StanceElement.h \
    $${ENGINE_DIR}/datatypes/elements/SkillUseElement.h \
    $${ENGINE_DIR}/datatypes/elements/SkillLevelElement.h \
    $${ENGINE_DIR}/datatypes/elements/SkillElement.h \
    $${ENGINE_DIR}/datatypes/elements/ResourceElement.h \
    $${ENGINE_DIR}/datatypes/elements/ReportElement.h \
    $${ENGINE_DIR}/datatypes/elements/RaceElement.h \
    $${ENGINE_DIR}/datatypes/elements/PermissionElement.h \
    $${ENGINE_DIR}/datatypes/elements/MovementElement.h \
    $${ENGINE_DIR}/datatypes/elements/MovementBonusElement.h \
    $${ENGINE_DIR}/datatypes/elements/MeleeAttackElement.h \
    $${ENGINE_DIR}/datatypes/elements/ItemElement.h \
    $${ENGINE_DIR}/datatypes/elements/InventoryElement.h \
    $${ENGINE_DIR}/datatypes/elements/FactionPermissionElement.h \
    $${ENGINE_DIR}/datatypes/elements/EnchantmentElement.h \
    $${ENGINE_DIR}/datatypes/elements/EventElement.h \
    $${ENGINE_DIR}/datatypes/elements/Element.h \
    $${ENGINE_DIR}/datatypes/elements/ConstructionWorksElement.h \
    $${ENGINE_DIR}/datatypes/elements/BonusElement.h \
    $${ENGINE_DIR}/datatypes/rules/WeatherRule.h \
    $${ENGINE_DIR}/datatypes/rules/TitleRule.h \
    $${ENGINE_DIR}/datatypes/rules/TerrainRule.h \
    $${ENGINE_DIR}/datatypes/rules/SkillRule.h \
    $${ENGINE_DIR}/datatypes/rules/SeasonRule.h \
    $${ENGINE_DIR}/datatypes/rules/RuleIndex.h \
    $${ENGINE_DIR}/datatypes/rules/Rule.h \
    $${ENGINE_DIR}/datatypes/rules/RaceRule.h \
    $${ENGINE_DIR}/datatypes/rules/LeaderRaceRule.h \
    $${ENGINE_DIR}/datatypes/rules/ItemRule.h \
    $${ENGINE_DIR}/datatypes/rules/FollowerRaceRule.h \
    $${ENGINE_DIR}/datatypes/rules/EventRule.h \
    $${ENGINE_DIR}/datatypes/rules/EnchantmentRule.h \
    $${ENGINE_DIR}/datatypes/rules/EffectRule.h \
    $${ENGINE_DIR}/datatypes/rules/CreatureRaceRule.h \
    $${ENGINE_DIR}/datatypes/rules/ConstructionRule.h \
    $${ENGINE_DIR}/datatypes/rules/ActionRule.h \
    $${ENGINE_DIR}/datatypes/entities/UnitEntity.h \
    $${ENGINE_DIR}/datatypes/entities/TokenEntity.h \
    $${ENGINE_DIR}/datatypes/entities/LocationEntity.h \
    $${ENGINE_DIR}/datatypes/entities/FactionEntity.h \
    $${ENGINE_DIR}/datatypes/entities/Entity.h \
    $${ENGINE_DIR}/datatypes/entities/EffectEntity.h \
    $${ENGINE_DIR}/datatypes/entities/ConstructionEntity.h \
    $${ENGINE_DIR}/datatypes/events/Event.h \
    $${ENGINE_DIR}/datatypes/strategies/SummonUsingStrategy.h \
    $${ENGINE_DIR}/datatypes/strategies/Strategy.h \
    $${ENGINE_DIR}/datatypes/strategies/NormalLearningStrategy.h \
    $${ENGINE_DIR}/datatypes/strategies/MarketStrategy.h \
    $${ENGINE_DIR}/datatypes/strategies/MagicLearningStrategy.h \
    $${ENGINE_DIR}/datatypes/strategies/HarvestUsingStrategy.h \
    $${ENGINE_DIR}/datatypes/strategies/EnchantmentUsingStrategy.h \
    $${ENGINE_DIR}/datatypes/strategies/CreatureLearningStrategy.h \
    $${ENGINE_DIR}/datatypes/strategies/CraftUsingStrategy.h \
    $${ENGINE_DIR}/datatypes/strategies/ConstructionUsingStrategy.h \
    $${ENGINE_DIR}/datatypes/strategies/CombatUsingStrategy.h \
    $${ENGINE_DIR}/datatypes/strategies/CombatActionStrategy.h \
    $${ENGINE_DIR}/datatypes/strategies/BuildUsingStrategy.h \
    $${ENGINE_DIR}/datatypes/strategies/BasicUsingStrategy.h \
    $${ENGINE_DIR}/datatypes/strategies/BasicProductionStrategy.h \
    $${ENGINE_DIR}/datatypes/strategies/BasicLearningStrategy.h \
    $${ENGINE_DIR}/datatypes/strategies/ActionUsingStrategy.h \
    $${ENGINE_DIR}/datatypes/attributes/TitlesAttribute.h \
    $${ENGINE_DIR}/datatypes/attributes/SkillsAttribute.h \
    $${ENGINE_DIR}/datatypes/attributes/SkillBonusComboAttribute.h \
    $${ENGINE_DIR}/datatypes/attributes/SkillBonusAttribute.h \
    $${ENGINE_DIR}/datatypes/attributes/PermissionAttribute.h \
    $${ENGINE_DIR}/datatypes/attributes/OwnershipPolicyAttribute.h \
    $${ENGINE_DIR}/datatypes/attributes/MovementBonusAttribute.h \
    $${ENGINE_DIR}/datatypes/attributes/InventoryAttribute.h \
    $${ENGINE_DIR}/datatypes/attributes/EnchantmentAttribute.h \
    $${ENGINE_DIR}/datatypes/attributes/BasicAttribute.h \
    $${ENGINE_DIR}/datatypes/collections/VarietiesCollection.h \
    $${ENGINE_DIR}/datatypes/collections/RulesCollection.h \
    $${ENGINE_DIR}/datatypes/collections/OrderPrototypesCollection.h \
    $${ENGINE_DIR}/datatypes/collections/MovementMode.h \
    $${ENGINE_DIR}/datatypes/collections/EntitiesCollection.h \
    $${ENGINE_DIR}/datatypes/collections/DataStorageHandler.h \
    $${ENGINE_DIR}/datatypes/collections/BasicVarietiesCollection.h \
    $${ENGINE_DIR}/datatypes/collections/BasicRulesCollection.h \
    $${ENGINE_DIR}/datatypes/collections/BasicEntitiesCollection.h \
          datatypes/collections/BasicEventsCollection.h \
    $${ENGINE_DIR}/datatypes/collections/BasicCollection.h \
    $${ENGINE_DIR}/datatypes/conditions/StanceCondition.h \
    $${ENGINE_DIR}/datatypes/conditions/SkillCondition.h \
    $${ENGINE_DIR}/datatypes/conditions/ObservationCondition.h \
    $${ENGINE_DIR}/datatypes/conditions/CompositeCondition.h \
    $${ENGINE_DIR}/datatypes/conditions/BasicCondition.h \
    $${ENGINE_DIR}/datatypes/misc/TimeStamp.h \
    $${ENGINE_DIR}/datatypes/misc/OwnershipPolicy.h \
    $${ENGINE_DIR}/datatypes/misc/NewEntityPlaceholder.h \
    $${ENGINE_DIR}/datatypes/misc/ExplicitExit.h \
    $${ENGINE_DIR}/datatypes/misc/EntityStatistics.h \
    $${ENGINE_DIR}/datatypes/misc/BasicExit.h \
    $${ENGINE_DIR}/datatypes/varieties/Variety.h \
    $${ENGINE_DIR}/datatypes/varieties/StanceVariety.h \
    $${ENGINE_DIR}/datatypes/varieties/MovementVariety.h \
    $${ENGINE_DIR}/datatypes/varieties/EquipmentSlotVariety.h \
    $${ENGINE_DIR}/datatypes/varieties/DirectionVariety.h \
    $${ENGINE_DIR}/datatypes/varieties/ConstructionWorksVariety.h \
    $${ENGINE_DIR}/datatypes/varieties/CombatTargetVariety.h \
    $${ENGINE_DIR}/datatypes/varieties/CombatStanceVariety.h \
    $${ENGINE_DIR}/datatypes/varieties/CombatRankVariety.h \
    $${ENGINE_DIR}/datatypes/varieties/CombatMoveVariety.h \
    $${ENGINE_DIR}/datatypes/varieties/CombatFileVariety.h \
    $${ENGINE_DIR}/combat/PillageRequest.h \
    $${ENGINE_DIR}/combat/GuardingRequest.h \
    $${ENGINE_DIR}/combat/CombatTactics.h \
    $${ENGINE_DIR}/combat/CombatOrderProcessor.h \
    $${ENGINE_DIR}/combat/CombatManager.h \
    $${ENGINE_DIR}/combat/CombatEngine.h \
    $${ENGINE_DIR}/combat/BattleUnit.h \
    $${ENGINE_DIR}/combat/BattleInstance.h \
    $${ENGINE_DIR}/combat/BattleField.h \
    $${ENGINE_DIR}/combat/BattleEntity.h \
    $${ENGINE_DIR}/combat/BasicCombatRequest.h \
    $${ENGINE_DIR}/combat/BasicCombatManager.h \
    $${ENGINE_DIR}/combat/BasicCombatEngine.h \
    $${ENGINE_DIR}/combat/actions/SpecialCombatAction.h \
    $${ENGINE_DIR}/combat/actions/RangedCombatAction.h \
    $${ENGINE_DIR}/combat/actions/MeleeCombatAction.h \
    $${ENGINE_DIR}/combat/orders/UseCombatOrder.h \
    $${ENGINE_DIR}/combat/orders/SweepCombatOrder.h \
    $${ENGINE_DIR}/combat/orders/SupportCombatOrder.h \
    $${ENGINE_DIR}/combat/orders/StandCombatOrder.h \
    $${ENGINE_DIR}/combat/orders/RetreatCombatOrder.h \
    $${ENGINE_DIR}/combat/orders/RangedCombatOrder.h \
    $${ENGINE_DIR}/combat/orders/ParryCombatOrder.h \
    $${ENGINE_DIR}/combat/orders/MoveCombatOrder.h \
    $${ENGINE_DIR}/combat/orders/MeleeCombatOrder.h \
    $${ENGINE_DIR}/combat/orders/FleeCombatOrder.h \
    $${ENGINE_DIR}/combat/orders/FlankCombatOrder.h \
    $${ENGINE_DIR}/combat/orders/FireCombatOrder.h \
    $${ENGINE_DIR}/combat/orders/CombatOrderPrototype.h \
    $${ENGINE_DIR}/combat/orders/ChargeCombatOrder.h \
    $${ENGINE_DIR}/combat/orders/ApplyCombatOrder.h \
    $${ENGINE_DIR}/combat/orders/AdvanceCombatOrder.h \
    $${ENGINE_DIR}/implementation/actions/TransformAction.h \
    $${ENGINE_DIR}/implementation/actions/TerraformAction.h \
    $${ENGINE_DIR}/implementation/actions/RitualActionRule.h \
    $${ENGINE_DIR}/implementation/actions/PickpocketActionRule.h \
    $${ENGINE_DIR}/implementation/actions/InfectionActionRule.h \
    $${ENGINE_DIR}/implementation/actions/CreatePortalAction.h \
    $${ENGINE_DIR}/implementation/actions/BuryActionRule.h \
    $${ENGINE_DIR}/implementation/actions/AssassinateAction.h \
    $${ENGINE_DIR}/implementation/actions/AffectWeatherAction.h \
    $${ENGINE_DIR}/implementation/combat/SummonCombatAction.h \
    $${ENGINE_DIR}/implementation/combat/HeavinessCombatAction.h \
    $${ENGINE_DIR}/implementation/combat/EnchantCombatAction.h \
    $${ENGINE_DIR}/implementation/effects/PlagueEffect.h \
    $${ENGINE_DIR}/process/ProcessingMode.h \
    $${ENGINE_DIR}/process/OrderProcessor.h \
    $${ENGINE_DIR}/process/OrderLine.h \
    $${ENGINE_DIR}/process/CombatOrderLine.h \
    $${ENGINE_DIR}/report/TurnReport.h \
    $${ENGINE_DIR}/report/TextsForReports.h \
    $${ENGINE_DIR}/report/ReportRecord.h \
    $${ENGINE_DIR}/report/ReportPattern.h \
    $${ENGINE_DIR}/report/reporting.h \
    $${ENGINE_DIR}/report/CombatReport.h \
    $${ENGINE_DIR}/report/patterns/UnaryMessage.h \
    $${ENGINE_DIR}/report/patterns/TertiaryMessage.h \
    $${ENGINE_DIR}/report/patterns/SimpleMessage.h \
    $${ENGINE_DIR}/report/patterns/ReportMessage.h \
    $${ENGINE_DIR}/report/patterns/QuintenaryMessage.h \
    $${ENGINE_DIR}/report/patterns/QuartenaryMessage.h \
    $${ENGINE_DIR}/report/patterns/EndLineMessage.h \
    $${ENGINE_DIR}/report/patterns/CombatParticipantMessage.h \
    $${ENGINE_DIR}/report/patterns/CombatLootMessage.h \
    $${ENGINE_DIR}/report/patterns/CombatAttackMessage.h \
    $${ENGINE_DIR}/report/patterns/BinaryMessage.h \
    $${ENGINE_DIR}/process/conflicts/WagesCompetitiveRequest.h \
    $${ENGINE_DIR}/process/conflicts/TeachingOffer.h \
    $${ENGINE_DIR}/process/conflicts/ResourceCompetitiveRequest.h \
    $${ENGINE_DIR}/process/conflicts/RecruitRequest.h \
    $${ENGINE_DIR}/process/conflicts/NewRecruitRequest.h \
    $${ENGINE_DIR}/process/conflicts/MarketRequest.h \
    $${ENGINE_DIR}/process/conflicts/MarketConflict.h \
    $${ENGINE_DIR}/process/conflicts/LocalRecruitOffer.h \
    $${ENGINE_DIR}/process/conflicts/LocalMarketRequest.h \
    $${ENGINE_DIR}/process/conflicts/EvenConflict.h \
    $${ENGINE_DIR}/process/conflicts/BasicRequest.h \
    $${ENGINE_DIR}/process/conflicts/BasicConflict.h \
    $${ENGINE_DIR}/process/conflicts/BasicCompetitiveRequest.h \
          $${ENGINE_DIR}/process/orders/TacticOrder.h \
          $${ENGINE_DIR}/process/orders/AcceptOrder.h \
          $${ENGINE_DIR}/process/orders/AtOrder.h \
          $${ENGINE_DIR}/process/orders/AttackOrder.h \
          $${ENGINE_DIR}/process/orders/BasicOrderSynchronizationRequest.h \
          $${ENGINE_DIR}/process/orders/BestowOrder.h \
          $${ENGINE_DIR}/process/orders/BuyOrder.h \
          $${ENGINE_DIR}/process/orders/CancelOrder.h \
          $${ENGINE_DIR}/process/orders/CaravanOrder.h \
          $${ENGINE_DIR}/process/orders/ChristenOrder.h \
          $${ENGINE_DIR}/process/orders/ClaimOrder.h \
          $${ENGINE_DIR}/process/orders/CombatOrder.h \
          $${ENGINE_DIR}/process/orders/DayOrder.h \
          $${ENGINE_DIR}/process/orders/DescribeOrder.h \
          $${ENGINE_DIR}/process/orders/DisbandOrder.h \
          $${ENGINE_DIR}/process/orders/DropOrder.h \
          $${ENGINE_DIR}/process/orders/EjectOrder.h \
          $${ENGINE_DIR}/process/orders/EmailOrder.h \
          $${ENGINE_DIR}/process/orders/EnterOrder.h \
          $${ENGINE_DIR}/process/orders/EquipOrder.h \
          $${ENGINE_DIR}/process/orders/ExitOrder.h \
          $${ENGINE_DIR}/process/orders/ForgetOrder.h \
          $${ENGINE_DIR}/process/orders/GetOrder.h \
          $${ENGINE_DIR}/process/orders/GiveOrder.h \
          $${ENGINE_DIR}/process/orders/GuardOrder.h \
          $${ENGINE_DIR}/process/orders/HaveOrder.h \
          $${ENGINE_DIR}/process/orders/LeaderOrder.h \
          $${ENGINE_DIR}/process/orders/LeadingOrder.h \
          $${ENGINE_DIR}/process/orders/LeaveOrder.h \
          $${ENGINE_DIR}/process/orders/MarchOrder.h \
          $${ENGINE_DIR}/process/orders/MergeOrder.h \
          $${ENGINE_DIR}/process/orders/MoveOrder.h \
          $${ENGINE_DIR}/process/orders/NameOrder.h \
          $${ENGINE_DIR}/process/orders/OathOrder.h \
          $${ENGINE_DIR}/process/orders/OrderPrototype.h \
          $${ENGINE_DIR}/process/orders/PasswordOrder.h \
          $${ENGINE_DIR}/process/orders/PatrolOrder.h \
          $${ENGINE_DIR}/process/orders/PillageOrder.h \
          $${ENGINE_DIR}/process/orders/PromoteOrder.h \
          $${ENGINE_DIR}/process/orders/RecruitOrder.h \
          $${ENGINE_DIR}/process/orders/ReshowOrder.h \
          $${ENGINE_DIR}/process/orders/ResignOrder.h \
          $${ENGINE_DIR}/process/orders/RetreatOrder.h \
          $${ENGINE_DIR}/process/orders/SeeOrder.h \
          $${ENGINE_DIR}/process/orders/SellOrder.h \
          $${ENGINE_DIR}/process/orders/SettingOrder.h \
          $${ENGINE_DIR}/process/orders/SizeOrder.h \
          $${ENGINE_DIR}/process/orders/SkillOrder.h \
          $${ENGINE_DIR}/process/orders/SplitOrder.h \
          $${ENGINE_DIR}/process/orders/StackOrder.h \
          $${ENGINE_DIR}/process/orders/StanceOrder.h \
          $${ENGINE_DIR}/process/orders/StayOrder.h \
          $${ENGINE_DIR}/process/orders/StudyOrder.h \
          $${ENGINE_DIR}/process/orders/SwapOrder.h \
          $${ENGINE_DIR}/process/orders/SwapOrderRequest.h \
          $${ENGINE_DIR}/process/orders/SynchroOrder.h \
          $${ENGINE_DIR}/process/orders/SynchroOrderRequest.h \
          $${ENGINE_DIR}/process/orders/TargetOrder.h \
          $${ENGINE_DIR}/process/orders/TeachOrder.h \
          $${ENGINE_DIR}/process/orders/UnstackOrder.h \
          $${ENGINE_DIR}/process/orders/UseOrder.h \
          $${ENGINE_DIR}/process/orders/WaitOrder.h \
          $${ENGINE_DIR}/process/orders/WithdrawOrder.h \
          $${ENGINE_DIR}/process/orders/WorkOrder.h \
          $${ENGINE_DIR}/process/orders/YieldOrder.h
