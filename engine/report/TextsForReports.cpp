/***************************************************************************
                          TextsForReports.cpp
                             -------------------
    begin                : Thu May 22 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "ReportPattern.h"
// Generic
ReportPattern *	invalidParameterReporter  = new ReportPattern("invalidParameterReporter");
ReportPattern *	unknownParameterReporter  = new ReportPattern("unknownParameterReporter");
ReportPattern *	missingParameterReporter  = new ReportPattern("missingParameterReporter");
ReportPattern *	invaliDirectionReporter   = new ReportPattern("invaliDirectionReporter");
// UnitEntity
ReportPattern * withdrawReporter = new ReportPattern("withdrawReporter");
ReportPattern * borrowReporter = new ReportPattern("borrowReporter");
ReportPattern * lendReporter = new ReportPattern("lendReporter");
ReportPattern * newLevelReporter = new ReportPattern("newLevelReporter");
ReportPattern * maxLevelReporter = new ReportPattern("maxLevelReporter");
ReportPattern * cantMoveReporter = new ReportPattern("cantMoveReporter");
ReportPattern * overloadReporter = new ReportPattern("overloadReporter");
ReportPattern * noMovementAbilityReporter = new ReportPattern("noMovementAbilityReporter");
ReportPattern * departLeaderReporter = new ReportPattern("departLeaderReporter");
ReportPattern * departFollowerReporter = new ReportPattern("departFollowerReporter");
ReportPattern * departPublicReporter = new ReportPattern("departPublicReporter");
ReportPattern * enterPublicReporter = new ReportPattern("enterPublicReporter");
ReportPattern * enterPrivateReporter = new ReportPattern("enterPrivateReporter");
ReportPattern * leavePublicReporter = new ReportPattern("leavePublicReporter");
ReportPattern * leavePrivateReporter = new ReportPattern("leavePrivateReporter");
ReportPattern * arrivePublicReporter = new ReportPattern("arrivePublicReporter");
ReportPattern * arrivePrivateReporter = new ReportPattern("arrivePrivateReporter");
ReportPattern * consumeFoodReporter = new ReportPattern("consumeFoodReporter");
ReportPattern * upkeepReporter = new ReportPattern("upkeepReporter");
ReportPattern * cannotPayUpkeepReporter = new ReportPattern("cannotPayUpkeepReporter");

// Accept
ReportPattern *  acceptOwnReporter         = new ReportPattern("acceptOwnReporter");
ReportPattern *	acceptReporter            = new ReportPattern("acceptReporter");
// Attack
ReportPattern *	AttackReporter                  = new ReportPattern("AttackReporter");
ReportPattern *	AttackedReporter                = new ReportPattern("AttackedReporter");
ReportPattern *	ownUnitAttackReporter           = new ReportPattern("ownUnitAttackReporter");
ReportPattern *	ownFactionAttackReporter        = new ReportPattern("ownFactionAttackReporter");
ReportPattern *	ownConstructionAttackReporter   = new ReportPattern("ownConstructionAttackReporter");
ReportPattern *	attackParticipatonReporter      = new ReportPattern("attackParticipatonReporter");
ReportPattern *	defenseParticipatonReporter     = new ReportPattern("defenseParticipatonReporter");
// Bestow
ReportPattern * publicBestowReporter= new ReportPattern("publicBestowReporter");
ReportPattern * privateBestowReporter= new ReportPattern("privateBestowReporter");
ReportPattern * receiveTitleReporter= new ReportPattern("receiveTitleReporter");
ReportPattern * noBestowTitleReporter    = new ReportPattern("noBestowTitleReporter");
ReportPattern * noBestowTitleConditionReporter    = new ReportPattern("noBestowTitleConditionReporter");
ReportPattern * noOwnBestowTitleReporter    = new ReportPattern    ("noOwnBestowTitleReporter");
// Build
ReportPattern * newBuidingStartedReporter    = new ReportPattern("newBuidingStartedReporter");
ReportPattern * buidingFinishedReporter    = new ReportPattern("buidingFinishedReporter");
ReportPattern * constructionStartedReporter    = new ReportPattern("constructionStartedReporter");
ReportPattern * buildingDestroyedReporter    = new ReportPattern("buildingDestroyedReporter");
ReportPattern * constructionWorksCompletedReporter = new ReportPattern("constructionWorksCompletedReporter");
ReportPattern * constructionWorksAddedReporter = new ReportPattern("constructionWorksAddedReporter");
// Buy/Sell/Recruit
ReportPattern * cantTradeReporter = new ReportPattern("cantTradeReporter");
ReportPattern * buyReporter       = new ReportPattern("buyReporter");
ReportPattern * sellReporter       = new ReportPattern("sellReporter");
ReportPattern * recruiterReporter    = new ReportPattern("recruiterReporter");
ReportPattern * recruitedReporter    = new ReportPattern("recruitedReporter");
ReportPattern * newRecruitReporter   = new ReportPattern("newRecruitReporter");
ReportPattern * cannotPayRecruitReporter  = new ReportPattern( "cannotPayRecruitReporter");
// Cancel
ReportPattern * publicCancelTitleReporter = new ReportPattern("publicCancelTitleReporter");
ReportPattern * privateCancelTitleReporter = new ReportPattern("privateCancelTitleReporter");
ReportPattern * cannotCancelReporter= new ReportPattern    ("cannotCancelReporter");
ReportPattern * noCancelOwnReporter = new ReportPattern    ("noCancelOwnReporter");
ReportPattern * noCancelTitleReporter    = new ReportPattern("noCancelTitleReporter");
// Caravan
ReportPattern * caravanLocationAddedReporter= new ReportPattern("caravanLocationAddedReporter");
// Claim
ReportPattern * noTitleReporter    = new ReportPattern("noTitleReporter");
ReportPattern * noTitleConditionReporter    = new ReportPattern("noTitleConditionReporter");
ReportPattern * cantPayForTitleReporter    = new ReportPattern("cantPayForTitleReporter");
ReportPattern * noFreeTitleReporter    = new ReportPattern("noFreeTitleReporter");
ReportPattern * contestTitleReporter    = new ReportPattern("contestTitleReporter");
ReportPattern * failedContestTitleReporter    = new ReportPattern("failedContestTitleReporter");
ReportPattern * successContestTitleReporter    = new ReportPattern("successContestTitleReporter");
ReportPattern * claimTitleReporter    = new ReportPattern("claimTitleReporter");
ReportPattern * reclaimTitleReporter    = new ReportPattern("reclaimTitleReporter");
ReportPattern * cantContestFromAllyReporter = new ReportPattern("cantContestFromAllyReporter");
// Christen
ReportPattern * cannotChristenReporter   = new ReportPattern("cannotChristenReporter");
ReportPattern * publicChristenReporter   = new ReportPattern("publicChristenReporter");
ReportPattern * privateChristenReporter  = new ReportPattern("privateChristenReporter");
// Combat
ReportPattern * combatSettingsReporter  = new ReportPattern("combatSettings");
// Disband
ReportPattern * disbandReporter    = new ReportPattern("disbandReporter");
ReportPattern * skillLossReporter  = new ReportPattern("skillLossReporter");
// Drop
ReportPattern *	privateDropItemsReporter = new ReportPattern("privateDropItemsReporter");
ReportPattern *	publicDropItemsReporter = new ReportPattern("publicDropItemsReporter");
// Eject
ReportPattern *	ejectReporter             = new ReportPattern("ejectReporter");
// e-mail
ReportPattern * changeEmailReporter= new ReportPattern("changeEmailReporter");
// Enter
ReportPattern * followerEnterReporter    = new ReportPattern("followerEnterReporter");
ReportPattern * leaderEnterReporter    = new ReportPattern("leaderEnterReporter");
ReportPattern * protectedEnterReporter    = new ReportPattern("protectedEnterReporter");
// Equip
ReportPattern *	unequipReporter           = new ReportPattern("unequipReporter");
ReportPattern *	equipReporter             = new ReportPattern("equipReporter");
ReportPattern *	unequipableReporter       = new ReportPattern("unequipableReporter");
ReportPattern *	equipSkillReporter        = new ReportPattern("equipSkillReporter");
ReportPattern *	maxEquipmentPerSlotReporter 	  = new ReportPattern("maxEquipmentPerSlotReporter");
// Exit
ReportPattern * followerExitReporter    = new ReportPattern("followerExitReporter");
ReportPattern * leaderExitReporter    = new ReportPattern("leaderExitReporter");
// Get
ReportPattern *	privateGetItemsReporter = new ReportPattern("privateGetItemsReporter");
ReportPattern *	publicGetItemsReporter = new ReportPattern("publicGetItemsReporter");
// Give
ReportPattern *	giveRejectedReporter      = new ReportPattern("giveRejectedReporter");
ReportPattern *	giveReporter              = new ReportPattern("giveReporter");
ReportPattern *	receiveReporter           = new ReportPattern("receiveReporter");
// Forget
ReportPattern * forgetReport= new ReportPattern("forgetReport");
// Merge
ReportPattern * mergeRaceMismatchReporter    = new ReportPattern("mergeRaceMismatchReporter");
ReportPattern * mergeRaceErrorReporter       = new ReportPattern("mergeRaceErrorReporter");
ReportPattern * mergeFactionMismatchReporter = new ReportPattern("mergeFactionMismatchReporter");
ReportPattern * mergeReporter                = new ReportPattern("mergeReporter");
ReportPattern * mergeSkillReporter  = new ReportPattern("mergeSkillReporter");
// Name
ReportPattern * changeNameReporter = new ReportPattern("changeNameReporter");
// Oath
ReportPattern *	oathRejectedReporter      = new ReportPattern("oathRejectedReporter");
ReportPattern *	oathReporter      = new ReportPattern("oathReporter");
ReportPattern *	oathCantReporter = new ReportPattern("oathCantReporter");
// Promote
ReportPattern *	promoteReporter = new ReportPattern("promoteReporter");
ReportPattern *	promotedReporter = new ReportPattern("promotedReporter");
// Password
ReportPattern * changePasswordReporter= new ReportPattern("changePasswordReporter");
// Recruit
ReportPattern * unableRecruitReporter = new ReportPattern("unableRecruitReporter");
ReportPattern * recruitInvalidReporter  = new ReportPattern("recruitInvalidReporter");
ReportPattern * recruitForeignUnitReporter  = new ReportPattern("recruitForeignUnitReporter");
ReportPattern * recruitMaxUnitSizeReporter  = new ReportPattern("recruitMaxUnitSizeReporter");
ReportPattern * recruitMixedRaceReporter  = new ReportPattern("recruitMixedRaceReporter");
ReportPattern * unrecruitableRaceReporter = new ReportPattern("unrecruitableRaceReporter");
// Resign
ReportPattern * resignReporter = new ReportPattern("resignReporter");
// Retreat
ReportPattern * retreatPublicReporter= new ReportPattern("retreatPublicReporter");
ReportPattern * retreatGroupReporter= new ReportPattern("retreatGroupReporter");
ReportPattern * retreatPrivateReporter= new ReportPattern("retreatPrivateReporter");
// Split
ReportPattern * splitterReporter    = new ReportPattern("splitterReporter");
ReportPattern * newSplitReporter    = new ReportPattern("newSplitReporter");
// Stack
ReportPattern *	stackingUnacceptableReporter   = new ReportPattern("stackingUnacceptableReporter" );
ReportPattern *	stackReporter             = new ReportPattern("stackReporter" );
ReportPattern *	cantStackSelfReporter    = new ReportPattern("cantStackSelfReporter");
// Stay
ReportPattern *	stayReporter             = new ReportPattern("stayReporter" );
// Study
ReportPattern * cannotStudyReporter        = new ReportPattern("cannotStudyReporter");
ReportPattern * raceErrorReporter          = new ReportPattern("raceErrorReporter");
ReportPattern * itemRequiredReporter       = new ReportPattern("itemRequiredReporter");
ReportPattern * requirementErrorReporter   = new ReportPattern("requirementErrorReporter");
ReportPattern * teachingErrorReporter      = new ReportPattern("teachingErrorReporter");
ReportPattern * maxLevelErrorReporter      = new ReportPattern("maxLevelErrorReporter");
ReportPattern * paymentErrorReporter       = new ReportPattern("paymentErrorReporter");
ReportPattern * learningStartedReporter    = new ReportPattern("learningStartedReporter");
ReportPattern * followerSkillLimitReporter = new ReportPattern("followerSkillLimitReporter");
ReportPattern * elementalSkillLimitReporter= new ReportPattern("elementalSkillLimitReporter");
// Tactics
ReportPattern * tacticsSettings  = new ReportPattern("tacticsSettings");
// Target
ReportPattern * TargetReporter = new ReportPattern("targetReporter");
// Teach
ReportPattern * unknownTactics  = new ReportPattern("unknownTactics");
// Teach
ReportPattern * teachingReporter    = new ReportPattern("teachingReporter");
// Unstack
ReportPattern *	unstackReporter           = new ReportPattern("unstackReporter");
// Use
ReportPattern * productionReporter         = new ReportPattern("productionReporter");
ReportPattern * startUseReporter         = new ReportPattern("startUseReporter");
ReportPattern * harvestReporter            = new ReportPattern("harvestReporter");
ReportPattern * invalidReporter            = new ReportPattern("invalidReporter");
ReportPattern * notAvailableReporter       = new ReportPattern("notAvailableReporter");
ReportPattern * notEnoughResourcesReporter = new ReportPattern("notEnoughResourcesReporter");
ReportPattern * unusableSkillReporter      = new ReportPattern("unusableSkillReporter");
ReportPattern * unusableBuildingSkillReporter      = new ReportPattern("unusableBuildingSkillReporter");
//ReportPattern * newSummonedReporter = new ReportPattern("newSummonedReporter");
//ReportPattern * summonerReporter = new ReportPattern("summonerReporter");
ReportPattern * summoningReporter =  new ReportPattern("summoningReporter");
// Withdraw
ReportPattern * withdrawRestrictedReporter = new ReportPattern("withdrawRestrictedReporter");
ReportPattern * withdrawInvalidReporter = new ReportPattern("withdrawInvalidReporter");
ReportPattern * withdrawUnitReporter = new ReportPattern("withdrawUnitReporter");
ReportPattern * withdrawFactionReporter = new ReportPattern("withdrawFactionReporter");
ReportPattern * withdrawFundEmptyReporter = new ReportPattern("withdrawFundEmptyReporter");
// Yield
ReportPattern * noOwnYieldTitleReporter    = new ReportPattern    ("noOwnYieldTitleReporter");
ReportPattern * noYieldTitleReporter    = new ReportPattern("noYieldTitleReporter");
ReportPattern * publicYieldTitleReporter = new ReportPattern("publicYieldTitleReporter");
ReportPattern * privateYieldTitleReporter = new ReportPattern("privateYieldTitleReporter");
ReportPattern * cannotOathYourselfReporter = new ReportPattern("cannotOathYourselfReporter");
//==========================================================================

ReportPattern * taxCollectedReporter = new ReportPattern("taxCollectedReporter");
//================================= COMBAT ===============================
ReportPattern * combatStartReporter = new ReportPattern("combatStartReporter");
ReportPattern * combatStart2Reporter = new ReportPattern("combatStart2Reporter");
ReportPattern * combatEndReporter = new ReportPattern("combatEndReporter");
ReportPattern * combatAttackersReporter = new ReportPattern("combatAttackersReporter");
ReportPattern * combatDefendersReporter = new ReportPattern("combatDefendersReporter");
ReportPattern * listReporter= new ReportPattern("listReporter");
ReportPattern * combatUseReporter = new ReportPattern("combatUseReporter");
ReportPattern * combatSummonReporter  = new ReportPattern("combatSummonReporter");
ReportPattern * combatCastReporter  = new ReportPattern("combatCastReporter");
ReportPattern * combatApplyReporter  = new ReportPattern("combatApplyReporter");

//==========================================================================
ReportPattern * privateEnchantmentReporter = new ReportPattern("privateEnchantmentReporter");
ReportPattern * privateEnchanterReporter = new ReportPattern("privateEnchanterReporter");
ReportPattern * publicEnchantmentReporter = new ReportPattern("publicEnchantmentReporter");
ReportPattern * privateActionReporter  = new ReportPattern("privateActionReporter");
ReportPattern * publicActionReporter   = new ReportPattern("publicActionReporter");
//==================== PickPocket

ReportPattern * pickPocketNoTargetReporter = new ReportPattern("pickPocketNoTargetReporter");
ReportPattern * pickPocketEmptyPocketReporter = new ReportPattern("pickPocketEmptyPocketReporter");
ReportPattern * pickPocketStealingFailureReporter = new ReportPattern("pickPocketStealingFailureReporter");
ReportPattern * pickPocketStealingPrivateReporter = new ReportPattern("pickPocketStealingPrivateReporter" );
ReportPattern * pickPocketStealingTargetReporter = new ReportPattern( "pickPocketStealingTargetReporter");
//ReportPattern * pickPocket Reporter = new ReportPattern( );
ReportPattern * drowningReporter = new ReportPattern( "DrowningReporter");

//==================== Ritual
ReportPattern * ritualSourceReporter = new ReportPattern("ritualSourceReporter");
ReportPattern * ritualTargetReporter = new ReportPattern("ritualTargetReporter");
//==================== Plague
 ReportPattern * plagueStartReporter = new ReportPattern("plagueStartReporter");
 ReportPattern * plagueReporter = new ReportPattern("plagueReporter");
 ReportPattern * plagueStopReporter = new ReportPattern("plagueStopReporter");
ReportPattern * plagueDeathReporter = new ReportPattern("plagueDeathReporter");
ReportPattern * plagueDeathReporter2 = new ReportPattern("plagueDeathReporter2");
//==================== Terraform
ReportPattern * terraformReport = new ReportPattern(" terraformReport");
ReportPattern * terraformReport2 = new ReportPattern(" terraformReport2");
//==================== Transform
ReportPattern * transformReport = new ReportPattern(" transformReport");
        //==================== Assassinate
ReportPattern * assassineEscapededReport2   = new ReportPattern("assassineEscapededReport2");
ReportPattern * assassineEscapededReport    = new ReportPattern("assassineEscapededReport");
ReportPattern * assassineCapturedReport3    = new ReportPattern("assassineCapturedReport3");
ReportPattern * assassineCapturedReport2    = new ReportPattern("assassineCapturedReport2");
ReportPattern * assassineCapturedReport     = new ReportPattern("assassineCapturedReport");
ReportPattern * assassinationSuccessReport  = new ReportPattern("assassinationSuccessReport");
ReportPattern * assassinationSuccessReport2 = new ReportPattern("assassinationSuccessReport2");
ReportPattern * assassinationSuccessReport3 = new ReportPattern("assassinationSuccessReport3");
ReportPattern * assassinationSuccessReport4 = new ReportPattern("assassinationSuccessReport4");
ReportPattern * assassinNoTargetReport = new ReportPattern("assassinNoTargetReport");
//==================== Create Portal
ReportPattern * portalFailureReport  = new ReportPattern("portalFailureReport");
ReportPattern * portalCreatedReport  = new ReportPattern("portalCreatedReport");
ReportPattern * portalAlreadyExistReport  = new ReportPattern("portalAlreadyExistReport");
//==================== Weather
ReportPattern * weatherChangedReport = new ReportPattern("weatherChangedReport");
ReportPattern * weatherChangedReport2 = new ReportPattern("weatherChangedReport2");

