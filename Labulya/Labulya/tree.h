#pragma once
#include "lec.h"
#include "heap.h"
#include "list.h"
#include "Types.h"
#include "Parser.h"
#include "ErrorReporter.h"
#include "cmath"

class Lexes : lexeme
{
public:
	Lexes()
	{
		lex = new List(sizeof(lexeme));
	}
	~Lexes()
	{
		if(lex != nullptr)
			lex->~List();
	}
	void AddLexeme(lexeme* lexem)
	{
		lex->add(lexem);
	}
	List* RetLex()
	{
		return lex;
	}
private:
	List* lex;
};

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
			printf("%s", m_variable->Name());
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
			if (strcmp(m_operation->Data(), "-") == 0 && m_left)
			{
				res1->DataChange(Parser().DoubleToString(-o));
				res->DataChange(res1->Data());
				return res1;
			}
			if (strcmp(m_operation->Data(), "++") == 0 && m_left)
			{
				res1->DataChange(Parser().DoubleToString(o + 1));
				res->DataChange(res1->Data());
				return res1;
			}
			if (strcmp(m_operation->Data(), "--") == 0 && m_left)
			{
				res1->DataChange(Parser().DoubleToString(o - 1));
				res->DataChange(res1->Data());
				return res1;
			}
			if (strcmp(m_operation->Data(), "++") == 0 && !m_left)
			{
				res->DataChange(res1->Data());
				res1->DataChange(Parser().DoubleToString(o + 1));
				return res1;
			}
			if (strcmp(m_operation->Data(), "--") == 0 && !m_left)
			{
				res->DataChange(res1->Data());
				res1->DataChange(Parser().DoubleToString(o - 1));
				return res1;
			}
			if (strcmp(m_operation->Data(), "!") == 0 && m_left)
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
			if(m_left)
				printf(m_operation->Data());
			m_operand->print();
			if(!m_left)
				printf(m_operation->Data());
		}
	protected:
		bool    m_left;
		lexeme* m_operation;
		TNode*  m_operand;
};

class TBinaryOperation : TNode
{
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
			int type = res1->Type();
			if(res1->Type() > res2->Type())
				type = res2->Type();
			lexeme* res = new lexeme("Number", type, "0", res1->Line(), res1->Start_Position(), 100);
			if (strcmp(m_operation->Data(), "+") == 0)
			{
				res->DataChange(Parser().DoubleToString(o1 + o2));
			}
			if (strcmp(m_operation->Data(), "-") == 0)
			{
				res->DataChange(Parser().DoubleToString(o1 - o2));
			}
			if (strcmp(m_operation->Data(), "*") == 0)
			{
				res->DataChange(Parser().DoubleToString(o1 * o2));
			}
			if (strcmp(m_operation->Data(), "/") == 0)
			{
				res->DataChange(Parser().DoubleToString(o1 / o2));
			}
			if (strcmp(m_operation->Data(), "=") == 0)
			{
				if(IsCastable(res1->Type(), res2->Type()))
				{
					res1->DataChange(res2->Data());
					res = res1;
				}
				else
					ErrorReporter().FReport(logfile, "������ �������� ����!", m_operation->Line(), m_operation->Start_Position());
			}
			if (strcmp(m_operation->Data(), "==") == 0)
			{
				res->~lexeme();
				res = new lexeme("Number", BOOL, "0", res1->Line(), res1->Start_Position(), 100);
				res->DataChange(Parser().BoolToString(o1 == o2));
			}
			if (strcmp(m_operation->Data(), "!=") == 0)
			{
				res->~lexeme();
				res = new lexeme("Number", BOOL, "0", res1->Line(), res1->Start_Position(), 100);
				res->DataChange(Parser().BoolToString(o1 != o2));
			}
			if (strcmp(m_operation->Data(), "<=") == 0)
			{
				res->~lexeme();
				res = new lexeme("Number", BOOL, "0", res1->Line(), res1->Start_Position(), 100);
				res->DataChange(Parser().BoolToString(o1 <= o2));
			}
			if (strcmp(m_operation->Data(), ">=") == 0)
			{
				res->~lexeme();
				res = new lexeme("Number", BOOL, "0", res1->Line(), res1->Start_Position(), 100);
				res->DataChange(Parser().BoolToString(o1 >= o2));
			}
			if (strcmp(m_operation->Data(), "<") == 0)
			{
				res->~lexeme();
				res = new lexeme("Number", BOOL, "0", res1->Line(), res1->Start_Position(), 100);
				res->DataChange(Parser().BoolToString(o1 < o2));
			}
			if (strcmp(m_operation->Data(), ">") == 0)
			{
				res->~lexeme();
				res = new lexeme("Number", BOOL, "0", res1->Line(), res1->Start_Position(), 100);
				res->DataChange(Parser().BoolToString(o1 > o2));
			}
			if (strcmp(m_operation->Data(), "&&") == 0)
			{
				res->~lexeme();
				res = new lexeme("Number", BOOL, "0", res1->Line(), res1->Start_Position(), 100);
				bool b1 = Parser().ToBool(res1->Data());
				bool b2 = Parser().ToBool(res2->Data());
				res->DataChange(Parser().BoolToString(b1 && b2));
			}
			if (strcmp(m_operation->Data(), "||") == 0)
			{
				res->~lexeme();
				res = new lexeme("Number", BOOL, "0", res1->Line(), res1->Start_Position(), 100);
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
			nods = new List(sizeof(TNode*));
			lexems = new Lexes();
		}
		~TList()
		{
			nods->~List();
			lexems->~Lexes();
		}
		lexeme* exec()  
		{  
			lexems->~Lexes();
			lexems = new Lexes();
			for (int i = 0; i < nods->count(); ++i)
			{
				TNode* cur = *(TNode**)nods->get(i);
				lexeme* temp = cur->exec();
				if (temp != nullptr)
					lexems->AddLexeme(temp);
			}
            return (lexeme*)lexems;
		}
		void print() 
		{  
			
			for(int i = 0; i < nods->count(); i++)
			{
				TNode* cur = *(TNode**)nods->get(i);
				cur->print();
			}
		}
		void  addNode(TNode* node)
		{
			nods->add(&node);
		}
	protected:
		Lexes* lexems;
		List* nods;
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
				if (branch_else != nullptr)
					return branch_else->exec();
			return nullptr;
		}
		void print()
		{
			printf("if (");
			condition->print();
			printf(")\n{\n");
			branch_then->print();
			printf("\n}\n");
			if (branch_else != nullptr)
			{
				printf("else\n{\n");
				branch_else->print();
				printf("\n}\n");
			}
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
			printf("\n}\n");
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
		printf("\n}while(");
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
		printf("\n}\n");
	}
protected:
	TVariable* initialization;
	TNode* condition;
	TNode* loop;
	TList* body;
};

class TOutPut : TNode
{
public:
	TOutPut(TNode* mstringexpression)
	{
		stringexpression = mstringexpression;
	}
	lexeme* exec()
	{
		stringexpression->print();
		return nullptr;
	}
	void print()
	{
		printf("output(");
		stringexpression->print();
		printf(");\n");
	}
protected:
	TNode* stringexpression;
};

class TInPut : TNode
{
public:
	TInPut(lexeme* Input)
	{
		input = Input;
	}
	lexeme* exec()
	{
		char* str = nullptr;
		scanf_s("%s", str, 100);
		return new lexeme("String", STRING, str, input->Line(), input->Start_Position(), input->Priority());
	}
	void print()
	{
		printf("input();\n");
	}
protected:
	lexeme* input;
};

class TMinMax : TNode
{
public:
	TMinMax(lexeme* Spec, TNode* mparam1, TNode* mparam2, bool MAX)
	{
		param1 = mparam1;
		param2 = mparam2;
		max = MAX;
		spec = Spec;
	}
	lexeme* exec()
	{
		bool f = false;
		if (parser.ToDouble(param1->exec()->Data()) > parser.ToDouble(param2->exec()->Data()))
			f = true;
		if (!max)
			f = !f;
		return new lexeme("Bool", BOOL, parser.BoolToString(f), spec->Line(), spec->Start_Position(), spec->Priority());
	}
	void print()
	{
		if (max) 
			printf("max(");
		else
			printf("min(");
		param1->print();
		printf(", ");
		param2->print();
		printf(");\n");
	}
protected:
	TNode* param1;
	TNode* param2;
	bool max;
	lexeme* spec;
};

class TSinCos : TNode
{
public:
	TSinCos(lexeme* Spec, TNode* mparam, bool SIN)
	{
		param = mparam;
		msin = SIN;
		spec = Spec;
	}
	lexeme* exec()
	{
		double res;
		if (msin)
			res = sin(parser.ToDouble(param->exec()->Data()));
		else res = cos(parser.ToDouble(param->exec()->Data()));
		return new lexeme("Number", DOUBLE, parser.DoubleToString(res), spec->Line(), spec->Start_Position(), spec->Priority());
	}
	void print()
	{
		if (msin)
			printf("sin(");
		else
			printf("cos(");
		param->print();
		printf(");\n");
	}
protected:
	TNode* param;
	bool msin;
	lexeme* spec;
};

class TElementArray : TNode
{
public:
	TElementArray(lexeme* array, TList* pointer)
	{
		_sarray = array;
		_spointer = pointer;
	}
	lexeme* exec()
	{
		Lexes* a = (Lexes*)_spointer->exec();
		lexeme* j = GetMassElem(_sarray, a->RetLex());
		return j;
		//return nullptr;
	}
	lexeme* GetMassElem(lexeme* mass, List* indexes)
	{
		if (mass->Rank() > 0)
		{
			//��� ��������!
			if (indexes->count() != mass->Sizes->count())
			{
				errorReporter.FReport(logfile, "���������� �������� �� ��������� � ������������ �������!", mass->Line(), mass->Start_Position());
				return nullptr;
			}
			int realindex = 1;
			for (int p = 0; p < indexes->count(); p++)
			{
				int s = *(int*)mass->Sizes->get(p);
				lexeme* sindex = (lexeme*)indexes->get(p);
				int index = parser.ToInt(sindex->Data());
				realindex = realindex * index;
				if (index >= s)
				{
					char str[100];
					sprintf_s(str, "����� �� ������� ���������! ������ \"%d\" >= \"%d\"", index, s);
					errorReporter.FReport(logfile, str, mass->Line(), mass->Start_Position());
					return nullptr;
				}
				for (int g = p + 1; g < indexes->count(); g++)
					realindex *= *(int*)indexes->get(g);
			}
			return *(lexeme**)(mass->Values->get(realindex));
		}
		return nullptr;
	}
	void print()
	{
		printf("%s[", _sarray->Name());
		_spointer->print();
		printf("]");
	}
protected:
	lexeme* _sarray;
	TList* _spointer;
};