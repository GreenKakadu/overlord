/***************************************************************************
                          OrderParameters.h 
         This is container for storing geterogeneous parameters.
                             -------------------
    begin                : Wen Aug  7 16:08:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef ORDER_PARAMETERS_H
#define ORDER_PARAMETERS_H
class OrderParameters
{
    public:
        OrderParameters(int parameterNum);
        ~OrderParameters(int parameterNum);
         ParameterHandler* operator []       (int index);
    protected:
	int parameterNum_;
	vector <ParameterHandler *> parameterHandlers_;
    private:

};
#endif
