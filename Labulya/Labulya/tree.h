#pragma once
#include "lec.h"
#include "heap.h"
#include "Types.h"
#include "Parser.h"

class TNode
{
	public:
		TNode()  {};
		virtual lexeme* exec() = 0;
		virtual void print() = 0;
};

class TConst : TNode
{
	public:
		TConst(lexeme* mconst)
		{
			m_const = mconst;
		}
		lexeme* exec()
		{
			return m_const;
		}
		void print()
		{
			printf("%s", m_const->Data());
		}
	protected:
		lexeme* m_const;
};   

class TVariable : TNode
{
	public:
		TVariable(lexeme* mvariable)
		{
			m_variable = mvariable;
		}			
		lexeme* exec()
		{
			return m_variable;
		}
		void print()
		{
			printf("%s", m_variable->Data());
		}
	protected:
		lexeme* m_variable;
};

class TUnaryOperation : TNode
{
	public:
		TUnaryOperation(TNode* operand, lexeme* moperation, bool left = true)
		{
			m_operand = operand;
			m_operation = moperation;
			m_left = left;
		}
		lexeme* exec()
		{ 
			lexeme* res1 = m_operand->exec();
			double o = Parser().ToDouble(res1->Data());
			lexeme* res = new lexeme("Number", NUMBER, "0", res1->Line(), res1->Start_Position(), 100);
			if (m_operation->Data() == "-" && m_left)
			{
				res1->DataChange(Parser().DoubleToString(-o));
				res->DataChange(res1->Data());
				return res;
			}
			if (m_operation->Data() == "++" && m_left)
			{
				res1->DataChange(Parser().DoubleToString(o + 1));
				res->DataChange(res1->Data());
				return res;
			}
			if (m_operation->Data() == "--" && m_left)
			{
				res1->DataChange(Parser().DoubleToString(o - 1));
				res->DataChange(res1->Data());
				return res;
			}
			if (m_operation->Data() == "++" && !m_left)
			{
				res->DataChange(res1->Data());
				res1->DataChange(Parser().DoubleToString(o + 1));
				return res;
			}
			if (m_operation->Data() == "--" && !m_left)
			{
				res->DataChange(res1->Data());
				res1->DataChange(Parser().DoubleToString(o - 1));
				return res;
			}
			if (m_operation->Data() == "+" && m_left)
			{
				return m_operand->exec();
			}
			if (m_operation->Data() == "!" && m_left)
			{
				bool b = Parser().ToBool(res1->Data());
				res1->DataChange(Parser().BoolToString(!b));
				res->DataChange(res1->Data());
				return m_operand->exec();
			}
			return NULL;
		}
		void print()
		{
			printf(m_operation->Data());
			m_operand->print();
		}
	protected:
		bool    m_left;
		lexeme* m_operation;
		TNode*  m_operand;
};

class TBinaryOperation : TNode
{
	//бинарный "+"
	public:
		TBinaryOperation(TNode*  operand1, TNode*  operand2, lexeme* operation)
		{
			m_operand1 = operand1;
			m_operand2 = operand2;
			m_operation = operation;
		}
		lexeme* exec()
		{
			lexeme* res1 = m_operand1->exec();
			lexeme* res2 = m_operand2->exec();
			double o1 = Parser().ToDouble(res1->Data());
			double o2 = Parser().ToDouble(res2->Data());
			lexeme* res = new lexeme("Number", NUMBER, "0", res1->Line(), res1->Start_Position(), 100);
			if (m_operation->Data() == "+")
			{
				res->DataChange(Parser().DoubleToString(o1 + o2));
			}
			if (m_operation->Data() == "-")
			{
				res->DataChange(Parser().DoubleToString(o1 - o2));
			}
			if (m_operation->Data() == "*")
			{
				res->DataChange(Parser().DoubleToString(o1 * o2));
			}
			if (m_operation->Data() == "/")
			{
				res->DataChange(Parser().DoubleToString(o1 / o2));
			}
			if (m_operation->Data() == "=")
			{
				if(IsCastable(res1->Type(), res2->Type()))
				{
					res1->DataChange(res2->Data());
					res = res1;
				}
			}
			if (m_operation->Data() == "==")
			{
				res->DataChange(Parser().BoolToString(o1 == o2));
			}
			if (m_operation->Data() == "!=")
			{
				res->DataChange(Parser().BoolToString(o1 != o2));
			}
			if (m_operation->Data() == "<=")
			{
				res->DataChange(Parser().BoolToString(o1 <= o2));
			}
			if (m_operation->Data() == ">=")
			{
				res->DataChange(Parser().BoolToString(o1 >= o2));
			}
			if (m_operation->Data() == "<")
			{
				res->DataChange(Parser().BoolToString(o1 < o2));
			}
			if (m_operation->Data() == ">")
			{
				res->DataChange(Parser().BoolToString(o1 > o2));
			}
			if (m_operation->Data() == "&&")
			{
				bool b1 = Parser().ToBool(res1->Data());
				bool b2 = Parser().ToBool(res2->Data());
				res->DataChange(Parser().BoolToString(b1 && b2));
			}
			if (m_operation->Data() == "||")
			{
				bool b1 = Parser().ToBool(res1->Data());
				bool b2 = Parser().ToBool(res2->Data());
				res->DataChange(Parser().BoolToString(b1 || b2));
			}
			return res;
		}
		void print()
		{
			m_operand1->print();
			printf(" ");
			printf(m_operation->Data());
			printf(" ");
            m_operand2->print();
			printf("\n");
		}
	protected:
		lexeme* m_operation;
		TNode*  m_operand1;
		TNode*  m_operand2;
};

class TTernaryOperator : TNode
{
public: 
	TTernaryOperator(TNode* operand1, TNode* operand2, TNode* operand3)
	{
		m_operand1 = operand1;
		m_operand2 = operand2;
		m_operand3 = operand3;
	}
	virtual lexeme* exec()
	{
		if (Parser().ToBool(m_operand1->exec()->Data()))
			return m_operand2->exec();
		else
			return m_operand3->exec();
		
	}
	virtual void print()
	{
		m_operand1->print();
		printf("?");
		m_operand2->print();
		printf(":");
		m_operand3->print();
	}
protected:
	TNode*  m_operand1;
	TNode*  m_operand2;
	TNode*  m_operand3;
};

class TList : TNode
{
	public:
		TList()
		{
			start = NULL;
		}
		~TList()
		{
			list* cur = start;
			while (cur)
			{
				list* next = cur->next;
				heap.free_mem(&cur);
				cur = next;
			}
		}
		lexeme* exec()  
		{  
			list* cur = start;
            while(cur)
            {
				cur->sentence->exec();
                cur=cur->next;
            }
            lexeme* res = nullptr;
            return res;
		}
		void print() 
		{  
			list* cur = start;
			while (cur != nullptr)
			{
				cur->sentence->print();
				cur=cur->next;
			}
		}
		void  addNode(TNode* node)
		{
			list *a = (list*)heap.get_mem(sizeof(list));
			a->sentence = node;
			a->next = NULL;
			if (start != NULL)
			{
				start->next = a;
			}
			start = a;
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
		TIf(TNode* mcondition, TList* mbranch_then, TList* mbranch_else)
		{
			condition = mcondition;
			branch_then = mbranch_then;
			branch_else = mbranch_else;
		}
		lexeme* exec()
		{ 
			if(Parser().ToBool(condition->exec()->Data()))
				return branch_then->exec();
			else
				return branch_else->exec();
		}
		void print()
		{
			printf("if(");
			condition->print();
			printf(")\n{");
			branch_then->print();
			printf("\n}\nelse\n{");
			branch_else->print();
			printf("}\n");
		}
	protected:
		TNode* condition;
		TList* branch_then;
		TList* branch_else;
};

class TWhile : TNode
{
	public:
		TWhile(TNode* mcondition, TList* mbody)
		{
			condition = mcondition;
			body = mbody;
		}
		lexeme* exec() 
		{
			while(Parser().ToBool(condition->exec()->Data())) 
				body->exec();
			return condition->exec();
		}
		void print()
		{
			printf("while(");
			condition->print();
			printf(")\n{");
			body->print();
			printf("}\n");
		}
	protected:
		TNode* condition;
		TList* body;
};

class TDoWhile : TNode
{
public:
	TDoWhile(TList* mbody, TNode* mcondition)
	{
		condition = mcondition;
		body = mbody;
	}
	lexeme* exec()
	{
		do
			body->exec();
		while (Parser().ToBool(condition->exec()->Data()));
			
		return condition->exec();
	}
	void print()
	{
		printf("do\n{");
		body->print();
		printf("}while(");
		condition->print();
		printf(");\n");
	}
protected:
	TNode* condition;
	TList* body;
};

class TFor : TNode
{
public:
	TFor(TVariable* minitialization, TNode* mcondition, TNode* mloop, TList* mbody)
	{
		initialization = minitialization;
		condition = mcondition;
		loop = mloop;
		body = mbody;
	}
	lexeme* exec()
	{
		for (initialization->exec(); Parser().ToBool(condition->exec()->Data()); loop->exec())
			body->exec();
		return condition->exec();
	}
	void print()
	{
		printf("for(");
		initialization->print();
		printf("; ");
		condition->print();
		printf("; ");
		loop->print();
		printf(")\n{");
		body->print();
		printf("}\n");
	}
protected:
	TVariable* initialization;
	TNode* condition;
	TNode* loop;
	TList* body;
};