/***************************************************************************
                          CombatAttackMessage.cpp
                             -------------------
    begin                : Mar 22 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "CombatAttackMessage.h"
#include "BattleInstance.h"
#include "TokenEntity.h"
//extern const int BLOCK_SIZE;

CombatAttackMessage::CombatAttackMessage(int initiative, BattleInstance * attacker,
 int hits, BattleInstance * target, int damage, int killed, bool killedAll)
{
		initiative_ = initiative;
		attacker_= attacker->getOrigin();
		hits_= hits;
		target_= target->getOrigin();
		damage_= damage;
		killed_= killed;
		killedAll_ = killedAll;
}

// void * CombatAttackMessage::operator new(size_t size)
// {
//    return NEW<BinaryMessage>( size);
// }
// 
// 
// void  CombatAttackMessage::operator delete(void * deadObject, size_t size)
// {
//   DELETE_PATTERN<BinaryMessage>(deadObject, size);
// }


void CombatAttackMessage::printReport(ReportPrinter &out) const
{

	out <<initiative_<<": "<<attacker_->print();
	if(hits_ <= 0)
	{
		out << " fails to strike against "<<
		 target_->print()<<"."<<endl;
		return;
	}
	out<<" strike ";
	if(hits_ == 1)
		out <<"once";
	else
		out <<hits_ << " times";

		out << " against " << target_->print();

	if(damage_ <= 0)
	{
		out << " but misses."<<endl;
		return;
	}
	out << " for " <<damage_<< " damage";

	if(killed_ <= 0)
	{
		out << "."<<endl;
		return;
	}
	if(killedAll_)
		out << " slaying all."<<endl;
	else
		out << " slaying " <<killed_<<"."<<endl;
}

void CombatAttackMessage::clean()
{
}



//CombatAttackMessage  * CombatAttackMessage::headOfFreeList;

