/***************************************************************************
                          libraryWorkaround.h
 Here temporary declared global objects to provide symbol loading from libraries.
 ***************************************************************************/
#include "ResourceCompetitiveRequest.h"
#include "LocalMarketRequest.h"
#include "MarketConflict.h"
#include "RecruitRequest.h"
#include "NewRecruitRequest.h"
#include "LocalRecruitOffer.h"
#include "SkillUseElement.h"
#include "TitleElement.h"
#include "EnchantmentElement.h"

#include "EmailOrder.h"          
#include "PasswordOrder.h"  
#include "ResignOrder.h"       
#include "ReshowOrder.h"     
#include "AcceptOrder.h"     
#include "AtOrder.h"         
#include "AttackOrder.h"  	
#include "BestowOrder.h"    
#include "BuyOrder.h"       
#include "CancelOrder.h"    
#include "CaravanOrder.h"   
#include "ChristenOrder.h"   
#include "ClaimOrder.h"     
#include "CombatOrder.h"  		
#include "DayOrder.h"        
#include "DescribeOrder.h"   
#include "DisbandOrder.h"    
#include "DropOrder.h"      
#include "EjectOrder.h"     
#include "EnterOrder.h"
#include "ExitOrder.h"
#include "EquipOrder.h"     
#include "ForgetOrder.h"    
#include "GetOrder.h"       
#include "GiveOrder.h"      
#include "GuardOrder.h" 		
#include "HaveOrder.h"       
#include "LeaderOrder.h"     
#include "LeadingOrder.h"   
#include "LeaveOrder.h"      
#include "MarchOrder.h" 	 	
#include "MergeOrder.h"     
#include "MoveOrder.h"      
#include "NameOrder.h"       
#include "OathOrder.h"      
#include "PatrolOrder.h"   	
#include "PillageOrder.h"  	
#include "PromoteOrder.h"   
#include "RecruitOrder.h"   
#include "RetreatOrder.h"   
#include "SeeOrder.h"        
#include "SellOrder.h"      
#include "SettingOrder.h"   
#include "SizeOrder.h"      
#include "SkillOrder.h"      
#include "SplitOrder.h"     
#include "StackOrder.h"     
#include "StanceOrder.h"    
#include "StayOrder.h"      
#include "StudyOrder.h"     
#include "SynchroOrder.h"   
#include "SwapOrder.h"       
#include "TacticOrder.h"   	
#include "TargetOrder.h"     
#include "TeachOrder.h"     
#include "UnstackOrder.h"   
#include "UseOrder.h"       
#include "WaitOrder.h"      
#include "WithdrawOrder.h"   
#include "WorkOrder.h"      
#include "YieldOrder.h" 	   

OrderPrototypesCollection sampleOrderPrototypesCollection;
TravelElement sample(0,0,0,0,0);
IntegerData  IntegerDataSample(0);
SkillUseElement SkillUseElementSample(0,0,0);
RationalNumber rationalSample(0);
ResourceCompetitiveRequest rcrSample(0, 0, 0, rationalSample);
LocalMarketRequest lmrSample(0,0,0,BUY);
RecruitRequest rrSample(0,0,0,0,0,0);
NewRecruitRequest nrrSample(0,0,0,0,0,0);
LocalRecruitOffer lrrSample(0,0,0);
MarketConflict marketConflictSample(0);
TitleElement sampleTitleElement(0,0,0);
EnchantmentElement sampleEnchantmentElement(0,0);
// Orders
//std::cout << "Sample Order  instantiation  start \n";

EnchantmentsAttribute sampleEnchantmentsAttribute;

EmailOrder    sampleEmailOrder;     
PasswordOrder samplePasswordOrder; 
ResignOrder   sampleResignOrder; 
ReshowOrder   sampleReshowOrder;
AcceptOrder   sampleAcceptOrder;  
AtOrder       sampleAtOrder; 
AttackOrder   sampleAttackOrder;	
BestowOrder   sampleBestowOrder; 
BuyOrder      sampleBuyOrder; 
CancelOrder   sampleCancelOrder;
CaravanOrder  sampleCaravanOrder;
ChristenOrder sampleChristenOrder; 
ClaimOrder    sampleClaimOrder;
CombatOrder   sampleCombatOrder;		
DayOrder      sampleDayOrder;  
DescribeOrder sampleDescribeOrder; 
DisbandOrder  sampleDisbandOrder; 
DropOrder     sampleDropOrder;
EjectOrder    sampleEjectOrder;
EnterOrder    sampleEnterOrder;
ExitOrder     sampleExitOrder;
EquipOrder    sampleEquipOrder;
ForgetOrder   sampleForgetOrder;
GetOrder      sampleGetOrder; 
GiveOrder     sampleGiveOrder; 
GuardOrder    sampleGuardOrder;		
HaveOrder     sampleHaveOrder;     
LeaderOrder   sampleLeaderOrder;    
LeadingOrder  sampleLeadingOrder; 
LeaveOrder    sampleLeaveOrder;  
MarchOrder    sampleMarchOrder;	 	
MergeOrder    sampleMergeOrder;   
MoveOrder     sampleMoveOrder;   
NameOrder     sampleNameOrder;    
OathOrder     sampleOathOrder;
PatrolOrder   samplePatrolOrder;	
PillageOrder  samplePillageOrder;	
PromoteOrder  samplePromoteOrder; 
RecruitOrder  sampleRecruitOrder;
RetreatOrder  sampleRetreatOrder; 
SeeOrder      sampleSeeOrder; 
SellOrder     sampleSellOrder; 
SettingOrder  sampleSettingOrder; 
SizeOrder     sampleSizeOrder; 
SkillOrder    sampleSkillOrder;  
SplitOrder    sampleSplitOrder; 
StackOrder    sampleStackOrder; 
StanceOrder   sampleStanceOrder; 
StayOrder     sampleStayOrder; 
StudyOrder    sampleStudyOrder; 
SynchroOrder  sampleSynchroOrder; 
SwapOrder     sampleSwapOrder;  
TacticOrder   sampleTacticOrder;	
TargetOrder   sampleTargetOrder;  
TeachOrder    sampleTeachOrder;
UnstackOrder  sampleUnstackOrder;
UseOrder      sampleUseOrder;
WaitOrder     sampleWaitOrder;
WithdrawOrder sampleWithdrawOrder;
WorkOrder     sampleWorkOrder;
YieldOrder    sampleYieldOrder;	   

//=============================================================================
#include "PickpocketActionRule.h" 
PickpocketActionRule samplePickpocketAction = PickpocketActionRule("PICKPOCKET_ACTION",&sampleAction);




