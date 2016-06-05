#pragma once
#include "lec.h"
#include "heap.h"

class TNode
{
	public:
		TNode()  {};
		~TNode() {};

		virtual lexeme* exec();
		virtual void print();
};

class TConst : TNode
{
	public:
		TConst(lexeme* mconst)
		{
			m_const = mconst;
		}
		~TConst() {}

		lexeme* exec()
		{
			return m_const;
		}
		void print()
		{
			printf("%s", m_const->Data);
		}
	protected:
		lexeme* m_const;
};   

class TVariable : TNode
{
	public:
		TVariable(lexeme* mvariable)
		{
			m_variable = m_variable;
		}
		~TVariable() {}
			
		lexeme* exec()
		{
			return m_variable;
		}
		void print()
		{
			printf("%s", m_variable->Name);
		}
	protected:
		lexeme* m_variable;
};

class TUnaryOperation : TNode
{
	public:
		TUnaryOperation(TNode* operand)
		{
			m_operand = operand;
		}
		~TUnaryOperation();
			
		lexeme exec()
		{ 
			lexeme res= m_operand->exec();
            res.DataChange("-" + res.Data);
            return res;
		}
		void print()
		{
			printf("-");
			m_operand->print();
		}
	protected:
		int    m_type;
		TNode* m_operand;
};

class TBinaryOperation : TNode
{
	//бинарный "+"
	public:
		TBinaryOperation(TNode*  operand1, TNode*  operand2)
		{
			m_operand1 = operand1;
			m_operand2 = operand2;
		}
		~TBinaryOperation();

		lexeme exec()
		{
			lexeme res1= m_operand1->exec();
            lexeme res2= m_operand2->exec();
            lexeme res;
			//перевод res1.Data() и res2.Data() в числа value1 и value2!
            res.DataChange(value1 + value2);
            return res; 
		}
                  // alt:
                  // { return m_operand1->exec() + m_operand2->exec(); }
		void print()
		{
			m_operand1->print(); 
            printf(" + ");
            m_operand2->print();
			printf("\n");
		}
	protected:
		int     m_type;
		TNode*  m_operand1;
		TNode*  m_operand2;
};

class TList : TNode
{
	public:
		TList()
		{

		}
		~TList();
		lexeme exec()  
		{  
			list* cur = start;
            while(cur)
            {
				cur->sentence->exec();
                cur=cur->next;
            }
            lexeme res;
            return res;
		}
		void print() 
		{  
			list* cur = start;
			while (cur)
			{
				cur->sentence->print();
				cur=cur->next;
			}
		}
		void  addNode(TNode* node)
		{
			
		}
	protected:
		struct list
		{
			TNode* sentence;
			list* next;
		};
		list* start;
};

class TIf : TNode
{
	public:
		TIf();
		~TIf();

		lexeme exec()
		{ 
			if(condition.exec().Data)
				return branch_then.exec();
			else
				return branch_else.exec();
		}
		void print()
		{
			printf("if(");
			condition.print();
			printf(")\n{");
			branch_then.print();
			printf("\n}\nelse\n{");
			branch_else.print();
			printf("}\n");
		}
	protected:
		TBinaryOperation condition;
		TList branch_then;
		TList branch_else;
};

class TWhile
{
	public:
		TWhile();
		~TWhile();

		lexeme exec() 
		{
			while(condition.exec().value()) 
				body.exec();
			return condition.exec();
		}
		void print();
	protected:
		TBinaryOperation condition;
		TList            body;
};