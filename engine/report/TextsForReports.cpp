/***************************************************************************
                          TextsForReports.cpp
                             -------------------
    begin                : Thu May 22 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "ReportPattern.h"
// Generic
ReportPattern *	invalidOrderReporter     = new ReportPattern("",": this is not "," order.");
ReportPattern *	invalidParameterReporter  = new ReportPattern("",": invalid  Parameter <","> (", " expected).");
ReportPattern *	unknownParameterReporter  = new ReportPattern("Warning! Unknown parameter "," .");
ReportPattern *	missingParameterReporter  = new ReportPattern("",": missing  Parameter (", " expected).");
ReportPattern *	invaliDirectionReporter   = new ReportPattern(""," direction is wrong or location is not connected.");
// UnitEntity
ReportPattern * withdrawReporter = new ReportPattern(""," withdraws "," from faction funds.");
ReportPattern * borrowReporter = new ReportPattern(""," borrows "," from ",".");
ReportPattern * lendReporter = new ReportPattern(""," lends "," to ","");
ReportPattern * newLevelReporter = new ReportPattern(""," reached level "," in ",".");
ReportPattern * maxLevelReporter = new ReportPattern(""," reached maximum level in ",". No further study is possible.");
ReportPattern * cantMoveReporter = new ReportPattern("",": "," can't move.");
ReportPattern * overloadReporter = new ReportPattern(""," overloaded: weight "," capacity ", "",".");
ReportPattern * noMovementAbilityReporter = new ReportPattern(""," have no ability to move toward ",".");
ReportPattern * departLeaderReporter = new ReportPattern(""," departs from "," to ",". Movementt will take  "," days.");
ReportPattern * departFollowerReporter = new ReportPattern(""," departs from "," to "," following ",". Movementt will take  "," days.");
ReportPattern * departPublicReporter = new ReportPattern(""," departs to ",".");
ReportPattern * enterPublicReporter = new ReportPattern(""," enters location.");
ReportPattern * enterPrivateReporter = new ReportPattern("Enters ",".");
ReportPattern * leavePublicReporter = new ReportPattern(""," leaves location.");
ReportPattern * leavePrivateReporter = new ReportPattern("Leaves ",".");
ReportPattern * arrivePublicReporter = new ReportPattern(""," arrives.");
ReportPattern * arrivePrivateReporter = new ReportPattern("Arrives to ",".");
ReportPattern * consumeFoodReporter = new ReportPattern("Consumes "," food.");
ReportPattern * upkeepReporter = new ReportPattern("Pays "," for upkeep.");
ReportPattern * cannotPayUpkeepReporter = new ReportPattern("Have not enough money to pay upkeep.");

// Accept
ReportPattern *  acceptOwnReporter         = new ReportPattern("Warning: "," belongs to your faction. Accept has no sence.");
ReportPattern *	acceptReporter            = new ReportPattern("Accepting ",".");
// Attack
ReportPattern *	AttackReporter                  = new ReportPattern(""," attacks ",".");
ReportPattern *	ownUnitAttackReporter           = new ReportPattern("Can't attack your own unit ",".");
ReportPattern *	ownFactionAttackReporter        = new ReportPattern("Can't attack your own faction ",".");
ReportPattern *	ownConstructionAttackReporter   = new ReportPattern("Can't attack your own construction ",".");
// Bestow
ReportPattern * publicBestowReporter= new ReportPattern(""," bestows title of "," to ",".");
ReportPattern * privateBestowReporter= new ReportPattern("Bestows title of "," to ",".");
ReportPattern * receiveTitleReporter= new ReportPattern("Receives title of "," from ",".");
ReportPattern * noBestowTitleReporter    = new ReportPattern("No ", "title available to bestow at ",".");
ReportPattern * noBestowTitleConditionReporter    = new ReportPattern("" ," doesn't have "," which is nescessary to hold ",".");
ReportPattern * noOwnBestowTitleReporter    = new ReportPattern    ("Should own the title of "," to bestow it.");
// Build
ReportPattern * newBuidingStartedReporter    = new ReportPattern("New "," " ," started.");
ReportPattern * buidingFinishedReporter    = new ReportPattern(""," " ," was finished.");
ReportPattern * constructionStartedReporter    = new ReportPattern(""," construction works started.");
ReportPattern * buildingDestroyedReporter    = new ReportPattern(" "," destroyed.");
// Buy/Sell/Recruit
ReportPattern * cantTradeReporter = new ReportPattern("" ," cannot trade.");
ReportPattern * buyReporter       = new ReportPattern("" ," buys ", " for ", " spending "," coins.");
ReportPattern * sellReporter       = new ReportPattern("" ," sells ", " for ", " gaining ", " coins.");
ReportPattern * recruiterReporter    = new ReportPattern("" ," recruits ", " for ", " spending "," coins.");
ReportPattern * recruitedReporter    = new ReportPattern("" ," were recruited to ", ".");
ReportPattern * newRecruitReporter   = new ReportPattern("New unit " ," of ", " was created.");
// Cancel
ReportPattern * publicCancelTitleReporter = new ReportPattern(""," cancels title of ",".");
ReportPattern * privateCancelTitleReporter = new ReportPattern("Cancels title of ",".");
ReportPattern * cannotCancelReporter= new ReportPattern    ("Have no rights to cancel a title of ",".");
ReportPattern * noCancelOwnReporter = new ReportPattern    ("Cannot cancel your own title of ",".");
ReportPattern * noCancelTitleReporter    = new ReportPattern("No ", "title available to cancel at ",".");
// Caravan
ReportPattern * caravanLocationAddedReporter= new ReportPattern("Warning! Can't find  ",
                    " in caravan list. Will try to add it as start location.");
// Claim
ReportPattern * noTitleReporter    = new ReportPattern("No ", "title available to claim at ",".");
ReportPattern * noTitleConditionReporter    = new ReportPattern("" ," doesn't have "," which is nescessary to claim ",".");
ReportPattern * cantPayForTitleReporter    = new ReportPattern("Doesn't have enough money to claim "," title.");
ReportPattern * noFreeTitleReporter    = new ReportPattern(""," title at ", " already belongs to ",".");
ReportPattern * contestTitleReporter    = new ReportPattern(""," contests "," title at "," from ",".");
ReportPattern * failedContestTitleReporter    = new ReportPattern(""," loses contest to "," with "," days of ", " against ",".");
ReportPattern * successContestTitleReporter    = new ReportPattern(""," wins contest from "," with "," days of "," against ",".");
ReportPattern * claimTitleReporter    = new ReportPattern("", " claims "," title  at ",".");
// Christen
ReportPattern * cannotChristenReporter   = new ReportPattern("Can't christen "," that not belongs to you.");
ReportPattern * publicChristenReporter   = new ReportPattern(""," christened ", " as ",".");
ReportPattern * privateChristenReporter  = new ReportPattern(""," was christened as ",".");
// Disband
ReportPattern * disbandReporter    = new ReportPattern("" ," disbanded.");
ReportPattern * skillLossReporter  = new ReportPattern(" Skill level in  ","  decreased to "," due to merging.");
// Drop
ReportPattern *	privateDropItemsReporter = new ReportPattern("Drops ",".");
ReportPattern *	publicDropItemsReporter = new ReportPattern(""," drops ",".");
// Eject
ReportPattern *	ejectReporter             = new ReportPattern(""," ejected from the stack.");
// e-mail
ReportPattern * changeEmailReporter= new ReportPattern("E-mail address was changed to ",".");
// Enter
ReportPattern * followerEnterReporter    = new ReportPattern("Entering "," following ",".");
ReportPattern * leaderEnterReporter    = new ReportPattern("Entering ",".");
ReportPattern * protectedEnterReporter    = new ReportPattern(""," is protected from entrance.");
// Equip
ReportPattern *	unequipReporter           = new ReportPattern(""," unequips ","");
ReportPattern *	equipReporter             = new ReportPattern(""," equips ","");
ReportPattern *	unequipableReporter       = new ReportPattern(""," can't be equiped.");
ReportPattern *	equipSkillReporter        = new ReportPattern(""," required to equip ","");
// Exit
ReportPattern * followerExitReporter    = new ReportPattern("Exiting from "," following ",".");
ReportPattern * leaderExitReporter    = new ReportPattern("Exiting from ",".");
// Get
ReportPattern *	privateGetItemsReporter = new ReportPattern("Gets "," from ",".");
ReportPattern *	publicGetItemsReporter = new ReportPattern(""," were taken by ",".");
// Give
ReportPattern *	giveRejectedReporter      = new ReportPattern(""," rejects gifts ");
ReportPattern *	giveReporter              = new ReportPattern(""," gives "," "," to ","");
ReportPattern *	receiveReporter           = new ReportPattern(""," receives "," "," from ","");
// Forget
ReportPattern * forgetReport= new ReportPattern("Forgets all it knowledge in ",".");
// Merge
ReportPattern * mergeRaceMismatchReporter    = new ReportPattern("May only merge units of the  same race.");
ReportPattern * mergeRaceErrorReporter       = new ReportPattern("" ," can't be merged.");
ReportPattern * mergeFactionMismatchReporter = new ReportPattern(" Can merge only in your faction units.");
ReportPattern * mergeReporter                = new ReportPattern("" ," merged " ," into" ,".");
ReportPattern * mergeSkillReporter  = new ReportPattern("Transfered figures from " ," loose their experience in " ,".");
// Name
ReportPattern * changeNameReporter = new ReportPattern("Name was changed to ",".");
// Oath
ReportPattern *	oathRejectedReporter      = new ReportPattern(""," rejects oath ");
ReportPattern *	oathReporter      = new ReportPattern(""," declares oath to ",".");
// Promote
ReportPattern *	promoteReporter = new ReportPattern("Promotes ",".");
ReportPattern *	promotedReporter = new ReportPattern("Promoted by ",".");
// Password
ReportPattern * changePasswordReporter= new ReportPattern("Password was changed to ",".");
// Recruit
ReportPattern * unableRecruitReporter = new ReportPattern(""," is not able to recruit.");
ReportPattern * recruitInvalidReporter  = new ReportPattern("Cannot recruit to ",".");
ReportPattern * recruitForeignUnitReporter  = new ReportPattern("Cannot recruit to foreign unit ",".");
ReportPattern * recruitMaxUnitSizeReporter  = new ReportPattern("", " cannot have more than ",".");
ReportPattern * recruitMixedRaceReporter  = new ReportPattern("Race mismatch in ", ".");
ReportPattern * unrecruitableRaceReporter = new ReportPattern("Cannot recruit ", ".");
// Retreat
ReportPattern * retreatPublicReporter= new ReportPattern(""," retreats back to ",".");
ReportPattern * retreatGroupReporter= new ReportPattern("Retreats back to "," following ",".");
ReportPattern * retreatPrivateReporter= new ReportPattern("Retreats back to ",".");
// Split
ReportPattern * splitterReporter    = new ReportPattern("" ," splits ", " to ", ".");
ReportPattern * newSplitReporter    = new ReportPattern("New unit " ," of ", " was created.");
// Stack
ReportPattern *	stackingUnacceptableReporter   = new ReportPattern(""," refuses to allow stacking of  ", "." );
ReportPattern *	stackReporter             = new ReportPattern(""," stacked under ", "" );
// Stay
ReportPattern *	stayReporter             = new ReportPattern(""," stays while stack starts moving." );
// Study
ReportPattern * cannotStudyReporter        = new ReportPattern(""," can't study ");
ReportPattern * raceErrorReporter          = new ReportPattern(""," can't study ","");
ReportPattern * itemRequiredReporter       = new ReportPattern(""," needeed to study ","");
ReportPattern * requirementErrorReporter   = new ReportPattern(""," doesn't know skills necessary to study ","");
ReportPattern * teachingErrorReporter      = new ReportPattern(""," needs teacher  to study ","");
ReportPattern * maxLevelErrorReporter      = new ReportPattern(""," is already at maximum level of ","");
ReportPattern * paymentErrorReporter       = new ReportPattern(""," has not enough money to pay for studying ","");
ReportPattern * learningStartedReporter    = new ReportPattern(""," started to learn ","");
ReportPattern * followerSkillLimitReporter = new ReportPattern("" ," can't study second basic skill.");
// Teach
ReportPattern * teachingReporter    = new ReportPattern("Teaching ", ".");
ReportPattern * teachingReporter2    = new ReportPattern("Teaching "," ", ".");
// Unstack
ReportPattern *	unstackReporter           = new ReportPattern(""," unstacked.");
// Use
ReportPattern * productionReporter         = new ReportPattern(""," produces ",".");
ReportPattern * harvestReporter            = new ReportPattern(""," harvests ",".");
ReportPattern * invalidReporter            = new ReportPattern("Invalid USE order for ",".");
ReportPattern * notAvailableReporter       = new ReportPattern(" "," is not available at ",".");
ReportPattern * notEnoughResourcesReporter = new ReportPattern("Has not enough "," to produce ",".");
ReportPattern * unusableSkillReporter      = new ReportPattern("Skill "," can't be used.");
// Withdraw
ReportPattern * withdrawRestrictedReporter = new ReportPattern("Things may be only withdrawn in cities.");
ReportPattern * withdrawInvalidReporter = new ReportPattern("Cann't withdraw any ",".");
ReportPattern * withdrawUnitReporter = new ReportPattern("Withdraws ",".");
ReportPattern * withdrawFactionReporter = new ReportPattern(""," are withdrawn from faction funds by ","." );
ReportPattern * withdrawFundEmptyReporter = new ReportPattern("Faction funds of "," are empty.");
// Yield
ReportPattern * noOwnYieldTitleReporter    = new ReportPattern    ("Should own the title of "," to yield it.");
ReportPattern * noYieldTitleReporter    = new ReportPattern("No ", " title available to yield at ",".");
ReportPattern * publicYieldTitleReporter = new ReportPattern(""," yields title of ",".");
ReportPattern * privateYieldTitleReporter = new ReportPattern("Yields title of ",".");
ReportPattern * cannotOathYourselfReporter = new ReportPattern("Can't oath to yourself");
//


ReportPattern * privateEnchantmentReporter = new ReportPattern(""," enchanted with "," by ",".");
ReportPattern * privateEnchanterReporter = new ReportPattern(""," enchants "," with ",".");
ReportPattern * publicEnchantmentReporter = new ReportPattern(""," produces ",".");
ReportPattern * privateActionReporter  = new ReportPattern(""," produces ",".");
ReportPattern * publicActionReporter   = new ReportPattern(""," produces ",".");
//==================== PickPocket

ReportPattern * pickPocketNoTargetReporter = new ReportPattern("Failed to find ",".");
ReportPattern * pickPocketEmptyPocketReporter = new ReportPattern("Failed to find anything in pockets of ",".");
ReportPattern * pickPocketStealingFailureReporter = new ReportPattern("Failed to steal from ",".");
ReportPattern * pickPocketStealingPrivateReporter = new ReportPattern("Stole "," from ","." );
ReportPattern * pickPocketStealingTargetReporter = new ReportPattern( "Hep! Somebody stole some "," from us!");
//ReportPattern * pickPocket Reporter = new ReportPattern( );


