/***************************************************************************
                          Element.h
Template for implementation of small objects demanding efficient memory use.
                             -------------------
    begin                : Wed Jan 1 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef ELEMENT_H
#define ELEMENT_H
#include <stdlib.h>
#include "AbstractData.h"
class Entity;
extern const int BLOCK_SIZE;
extern void longtostr(unsigned long u, char *out);
template <class R, class P> class Element2 : public AbstractData {
public: 
	  Element2(R * rule, P parameter) {	rule_ = rule;	parameter1_ = parameter;}
	  Element2(const Element2 <R,P> &element): AbstractData(){	rule_ = element.getRule();	parameter1_ = element.getParameter1();}
	 virtual ~Element2(){};
   static void * operator new(size_t size);
   static void * operator new(size_t, void * element);  // Placement form of new
   static void   operator delete(void * deadObject, size_t size);
   inline R *   getRule()      const  {return rule_;}
   inline P     getParameter1() const  {return parameter1_;}
   inline void  setParameter1(P value) { parameter1_ = value;}
   inline void  setRule(R * rule)     { rule_ = rule;}
   inline bool operator ==  (Element2 <R,P>  rule2)  {return this->rule_ == rule2.getRule();}
   inline bool operator !=  (Element2 <R,P>  rule2)  {return this->rule_ != rule2.getRule();}
   inline bool operator >  (Element2 <R,P>  rule2)  {return this->rule_->getTag() > rule2.getRule()->getTag();}
   inline bool operator <  (Element2 <R,P>  rule2)  {return this->rule_->getTag() < rule2.getRule()->getTag();}
   virtual inline bool isValidElement() const {return (rule_ != 0);}
	static Element2<R,P>  * headOfFreeList;
  /**  */
protected:
//	  Element2(){};
	union
	{
			Element2<R,P>  * next; //for use in operator new
  		R  * rule_;
	};
   P parameter1_;
};

template <class R, class P> void * Element2<R,P>::operator new (size_t size)
{
	if(size != sizeof(Element2<R,P> ) )
		return ::operator new(size);

	Element2<R,P>  * p =  Element2<R,P> ::headOfFreeList;
	if(p)
			Element2<R,P> ::headOfFreeList = p->next;
	else
		{
			Element2<R,P>  * newBlock = static_cast<Element2<R,P>  *>
							(:: operator new ( BLOCK_SIZE * sizeof (Element2<R,P> ) ) );
			for (int i = 1; i < BLOCK_SIZE - 1 ; ++i)
				{
					newBlock[i].next = &newBlock[i+1];
				}
			newBlock[BLOCK_SIZE - 1 ].next = 0;
			p = newBlock;
			Element2<R,P> ::headOfFreeList = &newBlock[1];
		}	
	return p;
}
template <class R, class P> void * Element2<R,P>::operator new(size_t, void * element)
{
 return  element;
}
template <class R, class P> void  Element2<R,P>::operator delete (void * deadObject, size_t size)
{
 if(deadObject == 0) return;
 if(size != sizeof(Element2<R,P> ) )
		{
			::operator delete(deadObject);
			return;
		}
	Element2<R,P>  * p = static_cast<Element2<R,P>  *> (deadObject);
	p->next = Element2<R,P> ::headOfFreeList;
	Element2<R,P> ::headOfFreeList = p;
}
//   inline ostream& operator << (ostream& out, Element2 <R,P> & data)
//                                { out<<data.printName(); return out;}


 
template <class R, class P1, class P2> class Element3  : public AbstractData{
public:
	  Element3(R * rule, P1 par1, P2 par2){rule_ = rule;	parameter1_ = par1;	parameter2_ = par2;}
	  Element3(const Element3 <R,P1,P2> &element): AbstractData() {	rule_ = element.getRule();	parameter1_ = element.getParameter1(); parameter2_ = element.getParameter2();}
	 virtual ~Element3(){};
   static void * operator new(size_t size);
   static void * operator new(size_t, void * element);  // Placement form of new
   static void   operator delete(void * deadObject, size_t size);
   inline R *   getRule()      const  {return rule_;}
   inline P1     getParameter1() const  {return parameter1_;}
   inline P2     getParameter2() const  {return parameter2_;}
   inline void  setParameter1(P1 value) { parameter1_ = value;}
   inline void  setParameter2(P2 value) { parameter2_ = value;}
   inline void  setRule(R * rule)     { rule_ = rule;}
   inline bool operator ==  (Element3 <R,P1,P2>  rule2)  {return this->rule_ == rule2.getRule();}
   inline bool operator !=  (Element3 <R,P1,P2>  rule2)  {return this->rule_ != rule2.getRule();}
   inline bool operator <   (Element3 <R,P1,P2>  rule2)  {return this->rule_->getTag() < rule2.getRule()->getTag();}
   inline bool operator >   (Element3 <R,P1,P2>  rule2)  {return this->rule_->getTag() > rule2.getRule()->getTag();}
   virtual inline bool isValidElement() const {return (rule_ != 0);}
	static Element3<R,P1,P2>  * headOfFreeList;
  /**  */
protected:
//	  Element3(){};
	union
	{
			Element3<R,P1,P2>  * next; //for use in operator new
  		R  * rule_;
	};
   P1 parameter1_;
   P2 parameter2_;
};

template <class R, class P1, class P2> void * Element3<R,P1,P2>::operator new (size_t size)
{
	if(size != sizeof(Element3<R,P1,P2> ) )
		return ::operator new(size);

	Element3<R,P1,P2>  * p =  Element3<R,P1,P2> ::headOfFreeList;
	if(p)
			Element3<R,P1,P2> ::headOfFreeList = p->next;
	else
		{
			Element3<R,P1,P2>  * newBlock = static_cast<Element3<R,P1,P2>  *>
							(:: operator new ( BLOCK_SIZE * sizeof (Element3<R,P1,P2> ) ) );
			for (int i = 1; i < BLOCK_SIZE - 1 ; ++i)
				{
					newBlock[i].next = &newBlock[i+1];
				}
			newBlock[BLOCK_SIZE - 1 ].next = 0;
			p = newBlock;
			Element3<R,P1,P2> ::headOfFreeList = &newBlock[1];
		}	
	return p;
}
template <class R, class P1, class P2> void * Element3<R,P1,P2>::operator new(size_t, void * element)
{
 return  element;
}
template <class R, class P1, class P2> void  Element3<R,P1,P2>::operator delete (void * deadObject, size_t size)
{
 if(deadObject == 0) return;
 if(size != sizeof(Element3<R,P1,P2> ) )
		{
			::operator delete(deadObject);
			return;
		}
	Element3<R,P1,P2>  * p = static_cast<Element3<R,P1,P2>  *> (deadObject);
	p->next = Element3<R,P1,P2> ::headOfFreeList;
	Element3<R,P1,P2> ::headOfFreeList = p;
}

#endif
