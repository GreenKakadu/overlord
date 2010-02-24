/* 
 * File:   CreatePortalAction.cpp
 * Author: alex
 * 
 * Created on February 15, 2010, 9:15 PM
 */

#include "CreatePortalAction.h"
#include "Entity.h"
#include "LocationEntity.h"
#include "DirectionVariety.h"
#include "MovementVariety.h"
#include "ExplicitExit.h"
#include "SimpleMessage.h"
#include "BinaryMessage.h"
#include "UnaryMessage.h"

extern int roll_1Dx(int);
extern ReportPattern * portalFailureReport;
extern ReportPattern * portalCreatedReport;
extern ReportPattern * portalAlreadyExistReport;

CreatePortalAction sampleCreatePortalAction = CreatePortalAction("CREATE_PORTAL_ACTION", &sampleAction);

CreatePortalAction::CreatePortalAction(const CreatePortalAction * prototype)
: ActionRule(prototype)
{
    name_ = "Create Portal Action";
}

GameData * CreatePortalAction::createInstanceOfSelf()
{
    return CREATE_INSTANCE<CreatePortalAction > (this);
}

STATUS
CreatePortalAction::initialize(Parser *parser)
{


    if (parser->matchKeyword("DESTINATION"))
    {
        LocationEntity * loc = locations[parser->getWord()];
        if (loc)
        {
            destinations_.push_back(loc);
        }
        return OK;
    }
    ActionRule::initialize(parser);
    return OK;
}

ACTION_RESULT CreatePortalAction::carryOut(Entity * entity, AbstractData * parameter, int value)
{
    DirectionVariety * portal = directions["PORTAL"];
    TokenEntity * mage = dynamic_cast<TokenEntity *> (entity);
    assert(mage);
    LocationEntity * currentLocation = mage->getLocation();
    if (currentLocation->findExit(portal) != 0)
    {
        SimpleMessage * msg = new SimpleMessage(portalAlreadyExistReport);
        currentLocation->addReport(msg);
        mage->addReport(msg);
        return ACTION_FAILURE;

    }

    LocationEntity * target = 0;

    int size = destinations_.size();
    if (size)
    {
        for (int i = 0; i < 10; ++i)
        {
            // create portal to random location from destinations list
            int n = roll_1Dx(size);
            target = destinations_[n];
            // No more than one portal per location;
            // check that no portals exist
            if (target->findExit(portal) != 0)// Portal already exist, let's try once more
            {
                target = 0;
                continue;
            }
            break;

        }
    } else
    {
        cout << "No portal destination  for CreatePortalAction " << print() << endl;
    }
    if (target == 0)
    {
        mage->addReport(new SimpleMessage(portalFailureReport));
        return ACTION_FAILURE;
    }


    MovementMode <int> travelTimes;
    travelTimes[walkingMode] = 1;
    travelTimes[flyingMode] = 1;


    currentLocation->addExit(new ExplicitExit(currentLocation, portal, target,
            travelTimes));

  currentLocation->addReport(new BinaryMessage(portalCreatedReport,mage, target));
  mage->addReport(new UnaryMessage(portalCreatedReport,target));
    return ACTION_SUCCESS;
}
