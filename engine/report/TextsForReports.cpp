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
#include "Reporter.h"
// Generic
Reporter *	invalidOrderReporter     = new Reporter("",": this is not "," order.");
Reporter *	invalidParameterReporter  = new Reporter("",": invalid  Parameter <","> (", " expected).");
Reporter *	unknownParameterReporter  = new Reporter("Warning! Unknown parameter "," .");
Reporter *	missingParameterReporter  = new Reporter("",": missing  Parameter (", " expected).");
Reporter *	invaliDirectionReporter   = new Reporter(""," direction is wrong or location is not connected.");
// UnitEntity
Reporter * withdrawReporter = new Reporter(""," withdraws "," from faction funds.");
Reporter * borrowReporter = new Reporter(""," borrows "," from ",".");
Reporter * lendReporter = new Reporter(""," lends "," to ","");
Reporter * newLevelReporter = new Reporter(""," reached level "," in ",".");
Reporter * maxLevelReporter = new Reporter(""," reached maximum level in ",". No further study is possible.");
Reporter * cantMoveReporter = new Reporter("",": "," can't move.");
Reporter * overloadReporter = new Reporter(""," overloaded: weight "," capacity ", "",".");
Reporter * noMovementAbilityReporter = new Reporter(""," have no ability to move toward ",".");
Reporter * departLeaderReporter = new Reporter(""," departs from "," to ",". Movementt will take  "," days.");
Reporter * departFollowerReporter = new Reporter(""," departs from "," to "," following ",". Movementt will take  "," days.");
Reporter * departPublicReporter = new Reporter(""," departs to ",".");
Reporter * enterPublicReporter = new Reporter(""," enters location.");
Reporter * enterPrivateReporter = new Reporter("Enters ",".");
Reporter * leavePublicReporter = new Reporter(""," leaves location.");
Reporter * leavePrivateReporter = new Reporter("Leaves ",".");
Reporter * arrivePublicReporter = new Reporter(""," arrives.");
Reporter * arrivePrivateReporter = new Reporter("Arrives to ",".");
Reporter * consumeFoodReporter = new Reporter("Consumes "," food.");
Reporter * upkeepReporter = new Reporter("Pays "," for upkeep.");
Reporter * cannotPayUpkeepReporter = new Reporter("Have not enough money to pay upkeep.");

// Accept
Reporter *  acceptOwnReporter         = new Reporter("Warning: "," belongs to your faction. Accept has no sence.");
Reporter *	acceptReporter            = new Reporter("Accepting ",".");
// Attack
Reporter *	AttackReporter                  = new Reporter(""," attacks ",".");
Reporter *	ownUnitAttackReporter           = new Reporter("Can't attack your own unit ",".");
Reporter *	ownFactionAttackReporter        = new Reporter("Can't attack your own faction ",".");
Reporter *	ownConstructionAttackReporter   = new Reporter("Can't attack your own construction ",".");
// Bestow
Reporter * publicBestowReporter= new Reporter(""," bestows title of "," to ",".");
Reporter * privateBestowReporter= new Reporter("Bestows title of "," to ",".");
Reporter * receiveTitleReporter= new Reporter("Receives title of "," from ",".");
Reporter * noBestowTitleReporter    = new Reporter("No ", "title available to bestow at ",".");
Reporter * noBestowTitleConditionReporter    = new Reporter("" ," doesn't have "," which is nescessary to hold ",".");
Reporter * noOwnBestowTitleReporter    = new Reporter    ("Should own the title of "," to bestow it.");
// Build
Reporter * newBuidingStartedReporter    = new Reporter("New "," " ," started.");
Reporter * buidingFinishedReporter    = new Reporter(""," " ," was finished.");
Reporter * constructionStartedReporter    = new Reporter(""," construction works started.");
Reporter * buildingDestroyedReporter    = new Reporter(" "," destroyed.");
// Buy/Sell/Recruit
Reporter * cantTradeReporter = new Reporter("" ," cannot trade.");
Reporter * buyReporter       = new Reporter("" ," buys ", " for ", " spending "," coins.");
Reporter * sellReporter       = new Reporter("" ," sells ", " for ", " gaining ", " coins.");
Reporter * recruiterReporter    = new Reporter("" ," recruits ", " for ", " spending "," coins.");
Reporter * recruitedReporter    = new Reporter("" ," were recruited to ", ".");
Reporter * newRecruitReporter   = new Reporter("New unit " ," of ", " was created.");
// Cancel
Reporter * publicCancelTitleReporter = new Reporter(""," cancels title of ",".");
Reporter * privateCancelTitleReporter = new Reporter("Cancels title of ",".");
Reporter * cannotCancelReporter= new Reporter    ("Have no rights to cancel a title of ",".");
Reporter * noCancelOwnReporter = new Reporter    ("Cannot cancel your own title of ",".");
Reporter * noCancelTitleReporter    = new Reporter("No ", "title available to cancel at ",".");
// Caravan
Reporter * caravanLocationAddedReporter= new Reporter("Warning! Can't find  ",
                    " in caravan list. Will try to add it as start location.");
// Claim
Reporter * noTitleReporter    = new Reporter("No ", "title available to claim at ",".");
Reporter * noTitleConditionReporter    = new Reporter("" ," doesn't have "," which is nescessary to claim ",".");
Reporter * cantPayForTitleReporter    = new Reporter("Doesn't have enough money to claim "," title.");
Reporter * noFreeTitleReporter    = new Reporter(""," title at ", " already belongs to ",".");
Reporter * contestTitleReporter    = new Reporter(""," contests "," title at "," from ",".");
Reporter * failedContestTitleReporter    = new Reporter(""," loses contest to "," with "," days of ", " against ",".");
Reporter * successContestTitleReporter    = new Reporter(""," wins contest from "," with "," days of "," against ",".");
Reporter * claimTitleReporter    = new Reporter("", " claims "," title  at ",".");
// Christen
Reporter * cannotChristenReporter   = new Reporter("Can't christen "," that not belongs to you.");
Reporter * publicChristenReporter   = new Reporter(""," christened ", " as ",".");
Reporter * privateChristenReporter  = new Reporter(""," was christened as ",".");
// Disband
Reporter * disbandReporter    = new Reporter("" ," disbanded.");
Reporter * skillLossReporter  = new Reporter(" Skill level in  ","  decreased to "," due to merging.");
// Drop
Reporter *	privateDropItemsReporter = new Reporter("Drops ",".");
Reporter *	publicDropItemsReporter = new Reporter(""," drops ",".");
// Eject
Reporter *	ejectReporter             = new Reporter(""," ejected from the stack.");
// e-mail
Reporter * changeEmailReporter= new Reporter("E-mail address was changed to ",".");
// Enter
Reporter * followerEnterReporter    = new Reporter("Entering "," following ",".");
Reporter * leaderEnterReporter    = new Reporter("Entering ",".");
Reporter * protectedEnterReporter    = new Reporter(""," is protected from entrance.");
// Equip
Reporter *	unequipReporter           = new Reporter(""," unequips ","");
Reporter *	equipReporter             = new Reporter(""," equips ","");
Reporter *	unequipableReporter       = new Reporter(""," can't be equiped.");
Reporter *	equipSkillReporter        = new Reporter(""," required to equip ","");
// Exit
Reporter * followerExitReporter    = new Reporter("Exiting from "," following ",".");
Reporter * leaderExitReporter    = new Reporter("Exiting from ",".");
// Get
Reporter *	privateGetItemsReporter = new Reporter("Gets "," from ",".");
Reporter *	publicGetItemsReporter = new Reporter(""," were taken by ",".");
// Give
Reporter *	giveRejectedReporter      = new Reporter(""," rejects gifts ");
Reporter *	giveReporter              = new Reporter(""," gives "," "," to ","");
Reporter *	receiveReporter           = new Reporter(""," receives "," "," from ","");
// Forget
Reporter * forgetReport= new Reporter("Forgets all it knowledge in ",".");
// Merge
Reporter * mergeRaceMismatchReporter    = new Reporter("May only merge units of the  same race.");
Reporter * mergeRaceErrorReporter       = new Reporter("" ," can't be merged.");
Reporter * mergeFactionMismatchReporter = new Reporter(" Can merge only in your faction units.");
Reporter * mergeReporter                = new Reporter("" ," merged " ," into" ,".");
Reporter * mergeSkillReporter  = new Reporter("Transfered figures from " ," loose their experience in " ,".");
// Name
Reporter * changeNameReporter = new Reporter("Name was changed to ",".");
// Oath
Reporter *	oathRejectedReporter      = new Reporter(""," rejects oath ");
Reporter *	oathReporter      = new Reporter(""," declares oath to ",".");
// Promote
Reporter *	promoteReporter = new Reporter("Promotes ",".");
Reporter *	promotedReporter = new Reporter("Promoted by ",".");
// Password
Reporter * changePasswordReporter= new Reporter("Password was changed to ",".");
// Recruit
Reporter * unableRecruitReporter = new Reporter(""," is not able to recruit.");
Reporter * recruitInvalidReporter  = new Reporter("Cannot recruit to ",".");
Reporter * recruitForeignUnitReporter  = new Reporter("Cannot recruit to foreign unit ",".");
Reporter * recruitMaxUnitSizeReporter  = new Reporter("", " cannot have more than ",".");
Reporter * recruitMixedRaceReporter  = new Reporter("Race mismatch in ", ".");
Reporter * unrecruitableRaceReporter = new Reporter("Cannot recruit ", ".");
// Retreat
Reporter * retreatPublicReporter= new Reporter(""," retreats back to ",".");
Reporter * retreatGroupReporter= new Reporter("Retreats back to "," following ",".");
Reporter * retreatPrivateReporter= new Reporter("Retreats back to ",".");
// Split
Reporter * splitterReporter    = new Reporter("" ," splits ", " to ", ".");
Reporter * newSplitReporter    = new Reporter("New unit " ," of ", " was created.");
// Stack
Reporter *	stackingUnacceptableReporter   = new Reporter(""," refuses to allow stacking of  ", "." );
Reporter *	stackReporter             = new Reporter(""," stacked under ", "" );
// Stay
Reporter *	stayReporter             = new Reporter(""," stays while stack starts moving." );
// Study
Reporter * cannotStudyReporter        = new Reporter(""," can't study ");
Reporter * raceErrorReporter          = new Reporter(""," can't study ","");
Reporter * itemRequiredReporter       = new Reporter(""," needeed to study ","");
Reporter * requirementErrorReporter   = new Reporter(""," doesn't know skills necessary to study ","");
Reporter * teachingErrorReporter      = new Reporter(""," needs teacher  to study ","");
Reporter * maxLevelErrorReporter      = new Reporter(""," is already at maximum level of ","");
Reporter * paymentErrorReporter       = new Reporter(""," has not enough money to pay for studying ","");
Reporter * learningStartedReporter    = new Reporter(""," started to learn ","");
Reporter * followerSkillLimitReporter = new Reporter("" ," can't study second basic skill.");
// Teach
Reporter * teachingReporter    = new Reporter("Teaching ", ".");
Reporter * teachingReporter2    = new Reporter("Teaching "," ", ".");
// Unstack
Reporter *	unstackReporter           = new Reporter(""," unstacked.");                                            
// Use
Reporter * productionReporter         = new Reporter(""," produces ",".");
Reporter * harvestReporter            = new Reporter(""," harvests ",".");
Reporter * invalidReporter            = new Reporter("Invalid USE order for ",".");
Reporter * notAvailableReporter       = new Reporter(" "," is not available at ",".");
Reporter * notEnoughResourcesReporter = new Reporter("Has not enough "," to produce ",".");
Reporter * unusableSkillReporter      = new Reporter("Skill "," can't be used.");
// Withdraw
Reporter * withdrawRestrictedReporter = new Reporter("Things may be only withdrawn in cities.");
Reporter * withdrawInvalidReporter = new Reporter("Cann't withdraw any ",".");
Reporter * withdrawUnitReporter = new Reporter("Withdraws ",".");
Reporter * withdrawFactionReporter = new Reporter(""," are withdrawn from faction funds by ","." );
Reporter * withdrawFundEmptyReporter = new Reporter("Faction funds of "," are empty.");
// Yield
Reporter * noOwnYieldTitleReporter    = new Reporter    ("Should own the title of "," to yield it.");
Reporter * noYieldTitleReporter    = new Reporter("No ", " title available to yield at ",".");
Reporter * publicYieldTitleReporter = new Reporter(""," yields title of ",".");
Reporter * privateYieldTitleReporter = new Reporter("Yields title of ",".");
Reporter * cannotOathYourselfReporter = new Reporter("Can't oath to yourself"); 
//


Reporter * privateEnchantmentReporter = new Reporter(""," enchanted with "," by ",".");
Reporter * privateEnchanterReporter = new Reporter(""," enchants "," with ",".");
Reporter * publicEnchantmentReporter = new Reporter(""," produces ",".");
Reporter * privateActionReporter  = new Reporter(""," produces ",".");
Reporter * publicActionReporter   = new Reporter(""," produces ",".");
//==================== PickPocket

Reporter * pickPocketNoTargetReporter = new Reporter("Failed to find ",".");
Reporter * pickPocketEmptyPocketReporter = new Reporter("Failed to find anything in pockets of ",".");
Reporter * pickPocketStealingFailureReporter = new Reporter("Failed to steal from ",".");
Reporter * pickPocketStealingPrivateReporter = new Reporter("Stole "," from ","." );
Reporter * pickPocketStealingTargetReporter = new Reporter( "Hep! Somebody stole some "," from us!");
//Reporter * pickPocket Reporter = new Reporter( );


