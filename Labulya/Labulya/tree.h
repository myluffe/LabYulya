#pragma once

class TValue
{
public:
    int  type()         { return m_type; }
    void setType(int t) { m_type=t; }

    int  value()         { return m_value; }
    void setValue(int v) { m_value=v; }

    Tvalue operator + (Tvalue o1, Tvalue o2);
    bool   operator = (Tvalue o1, Tvalue o2);
    void   print();

private:
    int    m_type;
    double m_value;
};

class TNode
{
public:
   TNode()  {};
   ~TNode() {};

   virtual TValue exec();
   virtual void print();
};

class TConst : public TNode
{
public:
   TConst();
   ~TConst();

   TValue exec() { return m_const; };
   void print(); //  { printf("%g", m_const.value()); };

protected:
   TValue m_const;
};   

class TVariable : public TNode
{
};

class TUnaryOperation : public TNode
{
public:
    TUnaryOperation();
    ~TUnaryOperation();

   TValue exec(); // { Tvalue res= m_operand->exec();
                  //   res.setValue(-res.value());
                  //   return res; };
   void print();  // { printf("-"); m_operand->print(); }
protected:
   int     m_type;
   TNode   m_operand;
};

class TBinaryOperation : public TNode
{
public:
    TBinaryOperation();
    ~TBinaryOperation();

   TValue exec(); // { Tvalue res1= m_operand1->exec();
                  //   Tvalue res2= m_operand2->exec();
                  //   Tvalue res;
                  //   res.setValue(res1.value()+res2.value());
                  //   return res; };
                  // alt:
                  // { return m_operand1->exec() + m_operand2->exec(); }
   void print();  // { m_operand1->print(); 
                  //   printf(" + "); 
                  //   m_operand2->print(); }
protected:
   int     m_type;
   TNode   m_operand1;
   TNode   m_operand2;
};

class TList : public TNode
{
    struct list
    {
      TNode sentence;
      list* next;
    }
public:
    TList()
    ~TList();

    TValue exec()  {  
                       list cur = start;
                       while (cur)
                       {
                          cur->sentence->exec();
                          cur=cur->next;
                       };
                       TValue res;
                       return res;
                   };
    void   print() {  
                       list cur = start;
                       while (cur)
                       {
                          cur->sentence->print();
                          cur=cur->next;
                       };
                   };
    void  addNode(TNode node);

protected:
    list* start;
};

class TIf : public TNode
{
public:
      TIf();
      ~TIf();

      TValue exec() { if (condition.exec().value()) return branch_then.exec();
                      else                          return branch_else.exec();
                    };
      void print();

protected:
    TBinaryOperation condition;
    TList            branch_then;
    TList            branch_else;
};

class TWhile
{
public:
      TWhile();
      ~TWhile();

      TValue exec() { while (condition.exec().value()) 
                          body.exec();
                      return condition.exec();
                    };
      void print();
protected:
    TBinaryOperation condition;
    TList            body;
};