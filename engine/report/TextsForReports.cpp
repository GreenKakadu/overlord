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
Reporter *	invalidOrderReporter     = new Reporter("",": this is not "," order.");
Reporter *	invalidParameterReporter  = new Reporter("",": invalid  Parameter < ","> (", " expected).");
Reporter *	unknownParameterReporter  = new Reporter("Warning! Unknown parameter "," .");
Reporter *	missingParameterReporter  = new Reporter("",": missing  Parameter (", " expected).");
Reporter *  acceptOwnReporter         = new Reporter("Warning: "," belongs to your faction. Accept has no sence.");
Reporter *	acceptReporter            = new Reporter("Accepting",".");
Reporter *	unequipReporter           = new Reporter(""," unequips ","");
Reporter *	equipReporter             = new Reporter(""," equips ","");
Reporter *	unequipableReporter       = new Reporter(""," can't be equiped.");
Reporter *	equipSkillReporter        = new Reporter(""," required to equip","");

Reporter *	giveRejectedReporter      = new Reporter("","rejects gifts ");
Reporter *	giveReporter              = new Reporter("","gives "," "," to ","");
Reporter *	receiveReporter           = new Reporter("","receives "," "," from ","");
 
Reporter *	invaliDirectionReporter   = new Reporter(""," direction is wrong or location is not connected.");

Reporter *	stackingUnacceptableReporter   = new Reporter(""," refuses to allow stacking of  ", "." );
Reporter *	stackReporter             = new Reporter(""," stacked under ", "" );
Reporter *	unstackReporter           = new Reporter(""," unstacked.");                                            

Reporter * productionReporter         = new Reporter(""," produces ",".");
Reporter * harvestReporter            = new Reporter(""," harvests ",".");
Reporter * invalidReporter            = new Reporter("Invalid USE order for ",".");
Reporter * notAvailableReporter       = new Reporter(" "," is not available at ",".");
Reporter * notEnoughResourcesReporter = new Reporter("Has not enough "," to produce",".");
Reporter * unusableSkillReporter      = new Reporter("Skill "," can't be used.");
Reporter * cannotStudyReporter        = new Reporter(""," can't study ");
Reporter * raceErrorReporter          = new Reporter(""," can't study ","");
Reporter * requirementErrorReporter   = new Reporter(""," doesn't know skills necessary to study ","");
Reporter * teachingErrorReporter      = new Reporter(""," needs teacher  to study ","");
Reporter * maxLevelErrorReporter      = new Reporter(""," is already at maximum level of ","");
Reporter * paymentErrorReporter       = new Reporter(""," has not enough money to pay for studying ","");
Reporter * learningStartedReporter    = new Reporter(""," started to learn ","");
//Reporter * teachingRequiredReporter   = new Reporter("" ," needs teaching to study ", ".");
//Reporter * followerLevelLimitReporter = new Reporter("" ," can't study  ", " above ", "level.");
Reporter * followerSkillLimitReporter = new Reporter("" ," can't study second basic skill.");
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

Reporter * withdrawRestrictedReporter = new Reporter("Things may be only withdrawn in cities.");
Reporter * withdrawInvalidReporter = new Reporter("Cann't withdraw any ",".");
Reporter * withdrawUnitReporter = new Reporter("Withdraws",".");
Reporter * withdrawFactionReporter = new Reporter(""," are withdrawn from faction funds by","." );
Reporter * withdrawFundEmptyReporter = new Reporter("Faction funds of "," are empty.");

Reporter * unableRecruitReporter = new Reporter("","is not able to recruit.");
Reporter * recruitInvalidReporter  = new Reporter("Cannot recruit to ",".");
Reporter * recruitForeignUnitReporter  = new Reporter("Cannot recruit to foreign unit ",".");
Reporter * recruitMaxUnitSizeReporter  = new Reporter("", " cannot have more than ",".");
Reporter * recruitMixedRaceReporter  = new Reporter("Race mismatch in ", ".");
Reporter * unrecruitableRaceReporter = new Reporter("Cannot recruit ", ".");

Reporter * cantTradeReporter = new Reporter("" ," cannot trade.");
Reporter * buyReporter       = new Reporter("" ," buys ", " for ", " spending "," coins.");
Reporter * sellReporter       = new Reporter("" ," sells ", " for ", " gaining ", " coins.");
Reporter * recruiterReporter    = new Reporter("" ," recruits ", " for ", " spending "," coins.");
Reporter * recruitedReporter    = new Reporter("" ," were recruited to ", ".");
Reporter * newRecruitReporter   = new Reporter("New unit " ," of ", " was created.");

Reporter * disbandReporter    = new Reporter("" ," disbanded.");
Reporter * skillLossReporter  = new Reporter(" Skill level in  ","  decreased to "," due to merging.");

Reporter * mergeRaceMismatchReporter    = new Reporter("May only merge units of the  same race.");
Reporter * mergeRaceErrorReporter       = new Reporter("" ," can't be merged.");
Reporter * mergeFactionMismatchReporter = new Reporter(" Can merge only in your faction units.");
Reporter * mergeReporter                = new Reporter("" ," merged " ," into" ,".");

Reporter * mergeSkillReporter  = new Reporter("Transfered figures from " ," loose their experience in " ,".");
Reporter * splitterReporter    = new Reporter("" ," splits ", " to ", ".");
Reporter * newSplitReporter    = new Reporter("New unit " ," of ", " was created.");


Reporter * teachingReporter    = new Reporter("Teaching ", ".");




