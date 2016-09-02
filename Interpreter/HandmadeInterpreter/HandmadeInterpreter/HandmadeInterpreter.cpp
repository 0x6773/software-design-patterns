// HandmadeInterpreter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

struct Token
{
	enum class Type { integer, plus, minus, lparen, rparen } type;
	string text;

	Token(Type type, const string& text)
		: type(type),
		  text(text)
	{
	}

	friend std::ostream& operator<<(std::ostream& os, const Token& obj)
	{
		return os << "`" << obj.text << "`";
	}
};

struct Element
{
	virtual ~Element() = default;
	virtual int eval() const = 0;
};

struct Integer : Element
{
	int value;

	explicit Integer(int value)
		: value(value)
	{
	}

	int eval() const override { return value; }
};

struct BinaryOperation : Element
{
	enum class Type { addition, substraction } type;
	shared_ptr<Element> lhs, rhs;

	int eval() const override
	{
		if(type == Type::addition)
			return lhs->eval() + rhs->eval();
		return lhs->eval() - rhs->eval();
	}
};

vector<Token> lex(const string& input)
{
	vector<Token> result;
	for(auto i = 0; i < input.length(); ++i)
	{
		switch(input[i])
		{
		case '+':
			result.emplace_back(Token::Type::plus, "+");
			break;
		case '-':
			result.emplace_back(Token::Type::minus, "-");
			break;
		case '(':
			result.emplace_back(Token::Type::lparen, "(");
			break;
		case ')':
			result.emplace_back(Token::Type::rparen, ")");
			break;
		default:
			ostringstream buffer;
			buffer << input[i];
			for(auto j = i + 1; j < input.size(); ++j)
			{
				if(isdigit(input[j]))
				{
					buffer << input[j];
					i++;
				}
				else
				{
					result.emplace_back(Token::Type::integer, buffer.str());
					break;
				}
			}
		}
	}

	return result;
}

shared_ptr<Element> parse(const vector<Token>& tokens)
{
	auto result = make_shared<BinaryOperation>();
	auto have_lhs = false;

	for(auto i = 0; i < tokens.size(); ++i)
	{
		auto token = tokens[i];
		switch(token.type)
		{
		case Token::Type::integer: 
		{
			auto value = boost::lexical_cast<int>(token.text);
			auto integer = make_shared<Integer>(value);
			if(!have_lhs)
			{
				result->lhs = integer;
				have_lhs = true;
			}
			else result->rhs = integer;
		}
		break;
		case Token::Type::plus: 
			result->type = BinaryOperation::Type::addition;
			break;
		case Token::Type::minus: 
			result->type = BinaryOperation::Type::substraction;
			break;
		case Token::Type::lparen: 
		{
			auto j = i;
			for(; j < tokens.size(); ++j)
				if (tokens[j].type == Token::Type::rparen)
					break;
			vector<Token> subexpression(&tokens[i + 1], &tokens[j]);
			auto element = parse(subexpression);
			if (!have_lhs)
			{
				result->lhs = element;
				have_lhs = true;
			}
			else result->rhs = element;
			i = j;
		}
		break;
		case Token::Type::rparen: 
			
			break;
		default: break;
		}
	}
	return result;
}

int main()
{
	string input{ "(13-4)-(12-1)" };

	auto tokens = lex(input);
	for (auto& t : tokens)
		cout << t << "   ";

	auto parsed = parse(tokens);
	cout << input << " = " << parsed->eval() << endl;

	_getch();
    return 0;
}
