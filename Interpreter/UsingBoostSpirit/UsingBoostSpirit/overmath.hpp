#pragma once

#include "stdafx.h"

#define BOOST_SPIRIT_UNICODE
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/optional.hpp>

namespace overmath
{
	using namespace std;
	namespace qi = boost::spirit::qi;
	using namespace boost::spirit::unicode;

	struct parameter
	{
		wstring name;
		wstring type;
	};

	struct assignment_statement
	{
		wstring variable_begin_assigned;
		wstring value;

		wstring infer_type() const
		{
			return wstring(L"float");
		}
	};

	struct function
	{
		wstring name;
		vector<parameter> params;
		vector<assignment_statement> assignments;

		boost::optional<const parameter&> find_parameter(const wstring& name) const
		{
			auto found = find_if(begin(params), end(params), 
				[&](const parameter& p)
			{
				return p.name == name;
			});
			if (found == end(params))
				return boost::none;
			return *found;
		}
	};

	struct numeric_types_ : qi::symbols<wchar_t, wstring>
	{
		numeric_types_()
		{
			add(L"f32", L"float");
			add(L"f64", L"double");
		}
	} numeric_types;
}

BOOST_FUSION_ADAPT_STRUCT(
	overmath::function,
	(std::wstring, name),
	(std::vector<overmath::parameter>, params),
	(std::vector<overmath::assignment_statement>, assignments)
)

BOOST_FUSION_ADAPT_STRUCT(
	overmath::parameter,
	(std::wstring, name),
	(std::wstring, type)
)

BOOST_FUSION_ADAPT_STRUCT(
	overmath::assignment_statement,
	(std::wstring, variable_begin_assigned),
	(std::wstring, value)
)

namespace overmath
{
	template<typename Iterator>
	struct function_parser : qi::grammar<Iterator, function, space_type>
	{
		function_parser() : function_parser::base_type(start)
		{
			using qi::lit;

			param %=
				+alnum
				>> ':'
				>> numeric_types
				>> -char_(',');

			assignment %=
				+alnum
				>> '='
				>> +alnum
				>> ';';

			start %= lit("void")
				>> +(char_ - '(')
				>> '('
				>> *param
				>> ')'
				>> '{'
				>> *assignment
				>> '}';
		}

		qi::rule<Iterator, parameter, space_type> param;
		qi::rule<Iterator, assignment_statement, space_type> assignment;
		qi::rule<Iterator, function, space_type> start;
	};

	inline wstring render(const function& f)
	{
		wostringstream s;

		s << "void " << f.name << "(";

		// parameters
		const auto param_count = f.params.size();
		for(auto i = 0u; i < param_count; ++i)
		{
			auto& p = f.params[i];
			s << p.type << " " << p.name;
			if (i + 1 < param_count)
				s << ", ";
		}
		s << ")\n{\n";

		// assignments (later)
		const auto assign_count = f.assignments.size();
		for(auto i = 0u; i < assign_count; ++i)
		{
			s << " ";

			auto& a = f.assignments[i];
			auto type = a.infer_type();

			auto is_param = f.find_parameter(a.variable_begin_assigned) != boost::none;
			if (!is_param)
				s << type << " ";
			s << a.variable_begin_assigned << " = " << a.value << ";\n";
		}

		s << "}\n";

		return s.str();
	}

	template<typename Iterator>
	wstring parse(Iterator first, Iterator last)
	{
		using boost::spirit::qi::phrase_parse;

		function f;
		function_parser<wstring::const_iterator> fp{};
		auto b = phrase_parse(first, last, fp, space, f);

		if(b)
		{
			//return wstring(L"SUCCESS");
			return render(f);
		}
		return wstring(L"FAIL");
	}
}
