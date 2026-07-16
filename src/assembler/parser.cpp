// A Bison parser, made by GNU Bison 3.5.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.



// First part of user prologue.
#line 1 "misc/bison.ypp"


#include <iostream>
using namespace std;


#line 47 "src/assembler/parser.cpp"


#include "parser.hpp"




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 143 "src/assembler/parser.cpp"


  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  parser::parser (Assembler& assembler_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      yy_lac_established_ (false),
      assembler (assembler_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_number_type
  parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[+state];
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 37: // GPR
      case 38: // CSR
      case 49: // NUM_LITERAL
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 50: // SYMBOL
      case 51: // STR_LITERAL
        value.YY_MOVE_OR_COPY< string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 37: // GPR
      case 38: // CSR
      case 49: // NUM_LITERAL
        value.move< int > (YY_MOVE (that.value));
        break;

      case 50: // SYMBOL
      case 51: // STR_LITERAL
        value.move< string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 37: // GPR
      case 38: // CSR
      case 49: // NUM_LITERAL
        value.copy< int > (that.value);
        break;

      case 50: // SYMBOL
      case 51: // STR_LITERAL
        value.copy< string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 37: // GPR
      case 38: // CSR
      case 49: // NUM_LITERAL
        value.move< int > (that.value);
        break;

      case 50: // SYMBOL
      case 51: // STR_LITERAL
        value.move< string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    /// Discard the LAC context in case there still is one left from a
    /// previous invocation.
    yy_lac_discard_ ("init");

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex ());
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      {
        if (!yy_lac_establish_ (yyla.type_get ()))
           goto yyerrlab;
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        if (!yy_lac_establish_ (yyla.type_get ()))
           goto yyerrlab;

        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    yy_lac_discard_ ("shift");
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case 37: // GPR
      case 38: // CSR
      case 49: // NUM_LITERAL
        yylhs.value.emplace< int > ();
        break;

      case 50: // SYMBOL
      case 51: // STR_LITERAL
        yylhs.value.emplace< string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2:
#line 68 "misc/bison.ypp"
                               { cout << ".end\n\n"; YYACCEPT; }
#line 636 "src/assembler/parser.cpp"
    break;

  case 5:
#line 75 "misc/bison.ypp"
                                { cout << endl; }
#line 642 "src/assembler/parser.cpp"
    break;

  case 6:
#line 77 "misc/bison.ypp"
                                { cout << endl; }
#line 648 "src/assembler/parser.cpp"
    break;

  case 7:
#line 79 "misc/bison.ypp"
                            { cout << endl; }
#line 654 "src/assembler/parser.cpp"
    break;

  case 8:
#line 81 "misc/bison.ypp"
                      { cout << endl; }
#line 660 "src/assembler/parser.cpp"
    break;

  case 9:
#line 84 "misc/bison.ypp"
                            { cout << ".global "; }
#line 666 "src/assembler/parser.cpp"
    break;

  case 11:
#line 86 "misc/bison.ypp"
                            { cout << ".extern "; }
#line 672 "src/assembler/parser.cpp"
    break;

  case 13:
#line 88 "misc/bison.ypp"
                                                  { cout << ".section " << yystack_[0].value.as < string > (); assembler.start_new_section(yystack_[0].value.as < string > ()); }
#line 678 "src/assembler/parser.cpp"
    break;

  case 14:
#line 90 "misc/bison.ypp"
                          { assembler.process_instruction(".word "); }
#line 684 "src/assembler/parser.cpp"
    break;

  case 16:
#line 92 "misc/bison.ypp"
                                               { cout << ".skip " << yystack_[0].value.as < int > (); }
#line 690 "src/assembler/parser.cpp"
    break;

  case 17:
#line 94 "misc/bison.ypp"
                                                { cout << ".ascii " << yystack_[0].value.as < string > (); }
#line 696 "src/assembler/parser.cpp"
    break;

  case 18:
#line 96 "misc/bison.ypp"
                                                                   { cout << ".equ " << yystack_[2].value.as < string > () << ", " << yystack_[0].value.as < int > (); }
#line 702 "src/assembler/parser.cpp"
    break;

  case 19:
#line 100 "misc/bison.ypp"
                                    { cout << yystack_[0].value.as < string > (); }
#line 708 "src/assembler/parser.cpp"
    break;

  case 20:
#line 102 "misc/bison.ypp"
                                                      { cout << ", " << yystack_[0].value.as < string > (); }
#line 714 "src/assembler/parser.cpp"
    break;

  case 21:
#line 105 "misc/bison.ypp"
                                    { cout << yystack_[0].value.as < string > (); }
#line 720 "src/assembler/parser.cpp"
    break;

  case 22:
#line 107 "misc/bison.ypp"
                                     { cout << yystack_[0].value.as < int > (); }
#line 726 "src/assembler/parser.cpp"
    break;

  case 23:
#line 109 "misc/bison.ypp"
                                                       { cout << ", " << yystack_[0].value.as < string > (); }
#line 732 "src/assembler/parser.cpp"
    break;

  case 24:
#line 111 "misc/bison.ypp"
                                                        { cout << ", " << yystack_[0].value.as < int > (); }
#line 738 "src/assembler/parser.cpp"
    break;

  case 25:
#line 115 "misc/bison.ypp"
                             { cout << yystack_[1].value.as < string > () << ": "; }
#line 744 "src/assembler/parser.cpp"
    break;

  case 28:
#line 124 "misc/bison.ypp"
                          { cout << "halt"; }
#line 750 "src/assembler/parser.cpp"
    break;

  case 29:
#line 126 "misc/bison.ypp"
                         { cout << "int"; }
#line 756 "src/assembler/parser.cpp"
    break;

  case 30:
#line 128 "misc/bison.ypp"
                          { cout << "iret"; }
#line 762 "src/assembler/parser.cpp"
    break;

  case 31:
#line 130 "misc/bison.ypp"
                          { cout << "call"; }
#line 768 "src/assembler/parser.cpp"
    break;

  case 33:
#line 132 "misc/bison.ypp"
                         { cout << "ret"; }
#line 774 "src/assembler/parser.cpp"
    break;

  case 34:
#line 134 "misc/bison.ypp"
                         { cout << "jmp"; }
#line 780 "src/assembler/parser.cpp"
    break;

  case 36:
#line 136 "misc/bison.ypp"
                                             { cout << "beq %r" << yystack_[3].value.as < int > () << ", %r" << yystack_[1].value.as < int > () << " "; }
#line 786 "src/assembler/parser.cpp"
    break;

  case 38:
#line 138 "misc/bison.ypp"
                                             { cout << "beq %r" << yystack_[3].value.as < int > () << ", %r" << yystack_[1].value.as < int > () << " "; }
#line 792 "src/assembler/parser.cpp"
    break;

  case 40:
#line 140 "misc/bison.ypp"
                                             { cout << "beq %r" << yystack_[3].value.as < int > () << ", %r" << yystack_[1].value.as < int > () << " "; }
#line 798 "src/assembler/parser.cpp"
    break;

  case 42:
#line 142 "misc/bison.ypp"
                              { cout << "push %r" << yystack_[0].value.as < int > (); }
#line 804 "src/assembler/parser.cpp"
    break;

  case 43:
#line 144 "misc/bison.ypp"
                             { cout << "pop %r" << yystack_[0].value.as < int > (); }
#line 810 "src/assembler/parser.cpp"
    break;

  case 44:
#line 146 "misc/bison.ypp"
                                        { cout << "xchg %r" << yystack_[2].value.as < int > () << ", %r" << yystack_[0].value.as < int > (); }
#line 816 "src/assembler/parser.cpp"
    break;

  case 45:
#line 148 "misc/bison.ypp"
                                       { cout << "add %r" << yystack_[2].value.as < int > () << ", %r" << yystack_[0].value.as < int > (); }
#line 822 "src/assembler/parser.cpp"
    break;

  case 46:
#line 150 "misc/bison.ypp"
                                       { cout << "sub %r" << yystack_[2].value.as < int > () << ", %r" << yystack_[0].value.as < int > (); }
#line 828 "src/assembler/parser.cpp"
    break;

  case 47:
#line 152 "misc/bison.ypp"
                                       { cout << "mul %r" << yystack_[2].value.as < int > () << ", %r" << yystack_[0].value.as < int > (); }
#line 834 "src/assembler/parser.cpp"
    break;

  case 48:
#line 154 "misc/bison.ypp"
                                       { cout << "div %r" << yystack_[2].value.as < int > () << ", %r" << yystack_[0].value.as < int > (); }
#line 840 "src/assembler/parser.cpp"
    break;

  case 49:
#line 156 "misc/bison.ypp"
                             { cout << "not %r" << yystack_[0].value.as < int > (); }
#line 846 "src/assembler/parser.cpp"
    break;

  case 50:
#line 158 "misc/bison.ypp"
                                       { cout << "and %r" << yystack_[2].value.as < int > () << ", %r" << yystack_[0].value.as < int > (); }
#line 852 "src/assembler/parser.cpp"
    break;

  case 51:
#line 160 "misc/bison.ypp"
                                      { cout << "or %r" << yystack_[2].value.as < int > () << ", %r" << yystack_[0].value.as < int > (); }
#line 858 "src/assembler/parser.cpp"
    break;

  case 52:
#line 162 "misc/bison.ypp"
                                       { cout << "xor %r" << yystack_[2].value.as < int > () << ", %r" << yystack_[0].value.as < int > (); }
#line 864 "src/assembler/parser.cpp"
    break;

  case 53:
#line 164 "misc/bison.ypp"
                                       { cout << "shl %r" << yystack_[2].value.as < int > () << ", %r" << yystack_[0].value.as < int > (); }
#line 870 "src/assembler/parser.cpp"
    break;

  case 54:
#line 166 "misc/bison.ypp"
                                       { cout << "shr %r" << yystack_[2].value.as < int > () << ", %r" << yystack_[0].value.as < int > (); }
#line 876 "src/assembler/parser.cpp"
    break;

  case 55:
#line 168 "misc/bison.ypp"
                        { cout << "ld "; }
#line 882 "src/assembler/parser.cpp"
    break;

  case 56:
#line 168 "misc/bison.ypp"
                                                             { cout << ", %r" << yystack_[0].value.as < int > (); }
#line 888 "src/assembler/parser.cpp"
    break;

  case 57:
#line 170 "misc/bison.ypp"
                                  { cout << "st %r" << yystack_[1].value.as < int > () << ", "; }
#line 894 "src/assembler/parser.cpp"
    break;

  case 59:
#line 172 "misc/bison.ypp"
                                         { cout << "csrrd %csr" << yystack_[2].value.as < int > () << ", %r" << yystack_[0].value.as < int > (); }
#line 900 "src/assembler/parser.cpp"
    break;

  case 60:
#line 174 "misc/bison.ypp"
                                         { cout << "csrwr %r" << yystack_[2].value.as < int > () << ", %csr" << yystack_[0].value.as < int > (); }
#line 906 "src/assembler/parser.cpp"
    break;

  case 61:
#line 178 "misc/bison.ypp"
                                     { cout << yystack_[0].value.as < int > (); }
#line 912 "src/assembler/parser.cpp"
    break;

  case 62:
#line 180 "misc/bison.ypp"
                                    { cout << yystack_[0].value.as < string > (); }
#line 918 "src/assembler/parser.cpp"
    break;

  case 63:
#line 183 "misc/bison.ypp"
                                                { cout << "$" << yystack_[0].value.as < int > (); }
#line 924 "src/assembler/parser.cpp"
    break;

  case 64:
#line 185 "misc/bison.ypp"
                                               { cout << "$" << yystack_[0].value.as < string > (); }
#line 930 "src/assembler/parser.cpp"
    break;

  case 65:
#line 187 "misc/bison.ypp"
                                     { cout << yystack_[0].value.as < int > (); }
#line 936 "src/assembler/parser.cpp"
    break;

  case 66:
#line 189 "misc/bison.ypp"
                                    { cout << yystack_[0].value.as < string > (); }
#line 942 "src/assembler/parser.cpp"
    break;

  case 67:
#line 191 "misc/bison.ypp"
                    { cout << "%r" << yystack_[0].value.as < int > (); }
#line 948 "src/assembler/parser.cpp"
    break;

  case 68:
#line 193 "misc/bison.ypp"
                                        { cout << "[ %r" << yystack_[1].value.as < int > () << " ]"; }
#line 954 "src/assembler/parser.cpp"
    break;

  case 69:
#line 195 "misc/bison.ypp"
                                                         { cout << "[ %r" << yystack_[3].value.as < int > () << " + " << yystack_[1].value.as < int > () << " ]"; }
#line 960 "src/assembler/parser.cpp"
    break;

  case 70:
#line 197 "misc/bison.ypp"
                                                    { cout << "[ %r" << yystack_[3].value.as < int > () << " + " << yystack_[1].value.as < string > () << " ]"; }
#line 966 "src/assembler/parser.cpp"
    break;


#line 970 "src/assembler/parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[+yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yy_error_token_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      yy_lac_discard_ ("error recovery");
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  bool
  parser::yy_lac_check_ (int yytoken) const
  {
    // Logically, the yylac_stack's lifetime is confined to this function.
    // Clear it, to get rid of potential left-overs from previous call.
    yylac_stack_.clear ();
    // Reduce until we encounter a shift and thereby accept the token.
#if YYDEBUG
    YYCDEBUG << "LAC: checking lookahead " << yytname_[yytoken] << ':';
#endif
    std::ptrdiff_t lac_top = 0;
    while (true)
      {
        state_type top_state = (yylac_stack_.empty ()
                                ? yystack_[lac_top].state
                                : yylac_stack_.back ());
        int yyrule = yypact_[+top_state];
        if (yy_pact_value_is_default_ (yyrule)
            || (yyrule += yytoken) < 0 || yylast_ < yyrule
            || yycheck_[yyrule] != yytoken)
          {
            // Use the default action.
            yyrule = yydefact_[+top_state];
            if (yyrule == 0)
              {
                YYCDEBUG << " Err\n";
                return false;
              }
          }
        else
          {
            // Use the action from yytable.
            yyrule = yytable_[yyrule];
            if (yy_table_value_is_error_ (yyrule))
              {
                YYCDEBUG << " Err\n";
                return false;
              }
            if (0 < yyrule)
              {
                YYCDEBUG << " S" << yyrule << '\n';
                return true;
              }
            yyrule = -yyrule;
          }
        // By now we know we have to simulate a reduce.
        YYCDEBUG << " R" << yyrule - 1;
        // Pop the corresponding number of values from the stack.
        {
          std::ptrdiff_t yylen = yyr2_[yyrule];
          // First pop from the LAC stack as many tokens as possible.
          std::ptrdiff_t lac_size = std::ptrdiff_t (yylac_stack_.size ());
          if (yylen < lac_size)
            {
              yylac_stack_.resize (std::size_t (lac_size - yylen));
              yylen = 0;
            }
          else if (lac_size)
            {
              yylac_stack_.clear ();
              yylen -= lac_size;
            }
          // Only afterwards look at the main stack.
          // We simulate popping elements by incrementing lac_top.
          lac_top += yylen;
        }
        // Keep top_state in sync with the updated stack.
        top_state = (yylac_stack_.empty ()
                     ? yystack_[lac_top].state
                     : yylac_stack_.back ());
        // Push the resulting state of the reduction.
        state_type state = yy_lr_goto_state_ (top_state, yyr1_[yyrule]);
        YYCDEBUG << " G" << state;
        yylac_stack_.push_back (state);
      }
  }

  // Establish the initial context if no initial context currently exists.
  bool
  parser::yy_lac_establish_ (int yytoken)
  {
    /* Establish the initial context for the current lookahead if no initial
       context is currently established.

       We define a context as a snapshot of the parser stacks.  We define
       the initial context for a lookahead as the context in which the
       parser initially examines that lookahead in order to select a
       syntactic action.  Thus, if the lookahead eventually proves
       syntactically unacceptable (possibly in a later context reached via a
       series of reductions), the initial context can be used to determine
       the exact set of tokens that would be syntactically acceptable in the
       lookahead's place.  Moreover, it is the context after which any
       further semantic actions would be erroneous because they would be
       determined by a syntactically unacceptable token.

       yy_lac_establish_ should be invoked when a reduction is about to be
       performed in an inconsistent state (which, for the purposes of LAC,
       includes consistent states that don't know they're consistent because
       their default reductions have been disabled).

       For parse.lac=full, the implementation of yy_lac_establish_ is as
       follows.  If no initial context is currently established for the
       current lookahead, then check if that lookahead can eventually be
       shifted if syntactic actions continue from the current context.  */
    if (!yy_lac_established_)
      {
#if YYDEBUG
        YYCDEBUG << "LAC: initial context established for "
                 << yytname_[yytoken] << '\n';
#endif
        yy_lac_established_ = true;
        return yy_lac_check_ (yytoken);
      }
    return true;
  }

  // Discard any previous initial lookahead context.
  void
  parser::yy_lac_discard_ (const char* evt)
  {
   /* Discard any previous initial lookahead context because of Event,
      which may be a lookahead change or an invalidation of the currently
      established initial context for the current lookahead.

      The most common example of a lookahead change is a shift.  An example
      of both cases is syntax error recovery.  That is, a syntax error
      occurs when the lookahead is syntactically erroneous for the
      currently established initial context, so error recovery manipulates
      the parser stacks to try to find a new initial context in which the
      current lookahead is syntactically acceptable.  If it fails to find
      such a context, it discards the lookahead.  */
    if (yy_lac_established_)
      {
        YYCDEBUG << "LAC: initial context discarded due to "
                 << evt << '\n';
        yy_lac_established_ = false;
      }
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    std::ptrdiff_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
         In the first two cases, it might appear that the current syntax
         error should have been detected in the previous state when
         yy_lac_check was invoked.  However, at that time, there might
         have been a different syntax error that discarded a different
         initial context during error recovery, leaving behind the
         current lookahead.
    */
    if (!yyla.empty ())
      {
        symbol_number_type yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];

#if YYDEBUG
        // Execute LAC once. We don't care if it is succesful, we
        // only do it for the sake of debugging output.
        if (!yy_lac_established_)
          yy_lac_check_ (yytoken);
#endif

        int yyn = yypact_[+yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            for (int yyx = 0; yyx < yyntokens_; ++yyx)
              if (yyx != yy_error_token_ && yyx != yy_undef_token_
                  && yy_lac_check_ (yyx))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -54;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
     -54,    36,    -2,   -54,   -54,   -54,    -3,   -54,    26,   -54,
      32,    29,   -54,   -54,   -54,   -54,   -54,   -54,    47,    48,
      49,    50,    53,   -54,    54,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    71,   -54,    70,
     -54,    72,    38,    73,   -54,    56,    56,   -54,   -11,   -54,
     -54,    74,    -9,    -9,    75,    78,    79,   -54,   -54,    39,
      80,    81,    82,    83,    84,   -54,    85,    86,    87,    88,
      89,    90,    91,    92,   -54,   -54,   -54,   -54,   -54,   -54,
      93,    93,   -54,   -54,    94,    95,   -54,   -54,   -54,   -54,
      76,    98,    99,   -54,    -6,   100,   -54,   -54,   101,   -54,
     102,   103,   105,   106,   108,   109,   110,   111,   112,   113,
     114,   115,   104,    -4,   -54,   116,   117,   118,   -54,   -54,
      -8,   122,    39,   -54,   -54,   -54,   -54,   -54,   -54,   -54,
     -54,   -54,   -54,   -54,   -54,   -54,   -54,   -54,   -54,   -54,
     -54,   -54,    28,   -54,   -54,    -9,    -9,    -9,   119,   120,
     -54,   -54,   -54,   -54,   -54
  };

  const signed char
  parser::yydefact_[] =
  {
       3,     0,     0,     1,     9,    11,     0,    14,     0,     2,
       0,     0,    28,    29,    30,    31,    33,    34,     0,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     8,     0,
       4,     0,     0,     0,    27,     0,     0,    13,     0,    16,
      17,     0,     0,     0,     0,     0,     0,    42,    43,     0,
       0,     0,     0,     0,     0,    49,     0,     0,     0,     0,
       0,     0,     0,     0,    25,     5,     7,    26,     6,    19,
      10,    12,    22,    21,    15,     0,    61,    62,    32,    35,
       0,     0,     0,    67,     0,     0,    65,    66,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    18,     0,     0,     0,    63,    64,
       0,     0,     0,    45,    46,    47,    48,    50,    51,    52,
      53,    54,    44,    59,    60,    20,    24,    23,    36,    38,
      40,    68,     0,    56,    58,     0,     0,     0,     0,     0,
      37,    39,    41,    69,    70
  };

  const signed char
  parser::yypgoto_[] =
  {
     -54,   -54,   -54,   -54,   -54,   -54,   -54,   -54,   121,   -54,
     -54,   -54,    96,   -54,   -54,   -54,   -54,   -54,   -54,   -54,
     -53,   -12
  };

  const short
  parser::yydefgoto_[] =
  {
      -1,     1,     2,    40,    41,    45,    46,    48,    80,    84,
      42,    43,    44,    52,    53,   145,   146,   147,    59,   122,
      88,    98
  };

  const unsigned char
  parser::yytable_[] =
  {
      89,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,   141,     3,   142,    82,    83,
      86,    87,    38,   118,   119,   136,   137,    47,    39,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    49,    93,   148,   149,    51,
      94,    95,    76,    50,    54,    55,    56,    57,    96,    97,
      58,    60,   150,   151,   152,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    79,    72,    73,    74,
     144,     0,     0,   115,    85,    90,    75,    78,    91,    92,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   116,   117,   120,    77,   123,
     124,   121,   125,   126,   114,   127,   128,   129,   130,   131,
     132,   133,     0,   134,   135,     0,   138,   139,   140,   143,
       0,     0,   153,   154,     0,     0,     0,    81
  };

  const short
  parser::yycheck_[] =
  {
      53,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    43,     0,    45,    49,    50,
      49,    50,    44,    49,    50,    49,    50,    50,    50,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    49,    37,    49,    50,    50,
      41,    42,    44,    51,    37,    37,    37,    37,    49,    50,
      37,    37,   145,   146,   147,    37,    37,    37,    37,    37,
      37,    37,    37,    37,    37,    37,    50,    38,    37,    39,
     122,    -1,    -1,    37,    40,    40,    44,    44,    40,    40,
      40,    40,    40,    40,    40,    40,    40,    40,    40,    40,
      40,    40,    40,    40,    40,    37,    37,    37,    42,    37,
      37,    40,    37,    37,    49,    37,    37,    37,    37,    37,
      37,    37,    -1,    38,    50,    -1,    40,    40,    40,    37,
      -1,    -1,    43,    43,    -1,    -1,    -1,    46
  };

  const signed char
  parser::yystos_[] =
  {
       0,    53,    54,     0,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    44,    50,
      55,    56,    62,    63,    64,    57,    58,    50,    59,    49,
      51,    50,    65,    66,    37,    37,    37,    37,    37,    70,
      37,    37,    37,    37,    37,    37,    37,    37,    37,    37,
      37,    37,    38,    37,    39,    44,    44,    64,    44,    50,
      60,    60,    49,    50,    61,    40,    49,    50,    72,    72,
      40,    40,    40,    37,    41,    42,    49,    50,    73,    40,
      40,    40,    40,    40,    40,    40,    40,    40,    40,    40,
      40,    40,    40,    40,    49,    37,    37,    37,    49,    50,
      37,    40,    71,    37,    37,    37,    37,    37,    37,    37,
      37,    37,    37,    37,    38,    50,    49,    50,    40,    40,
      40,    43,    45,    37,    73,    67,    68,    69,    49,    50,
      72,    72,    72,    43,    43
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    52,    53,    54,    54,    55,    55,    55,    55,    57,
      56,    58,    56,    56,    59,    56,    56,    56,    56,    60,
      60,    61,    61,    61,    61,    62,    63,    63,    64,    64,
      64,    65,    64,    64,    66,    64,    67,    64,    68,    64,
      69,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    70,    64,    71,    64,    64,
      64,    72,    72,    73,    73,    73,    73,    73,    73,    73,
      73
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     0,     2,     2,     2,     2,     1,     0,
       3,     0,     3,     2,     0,     3,     2,     2,     4,     1,
       3,     1,     1,     3,     3,     2,     2,     1,     1,     1,
       1,     0,     3,     1,     0,     3,     0,     7,     0,     7,
       0,     7,     2,     2,     4,     4,     4,     4,     4,     2,
       4,     4,     4,     4,     4,     0,     5,     0,     5,     4,
       4,     1,     1,     2,     2,     1,     1,     1,     3,     5,
       5
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "$end", "error", "$undefined", "ASMD_GLOBAL", "ASMD_EXTERN",
  "ASMD_SECTION", "ASMD_WORD", "ASMD_SKIP", "ASMD_END", "ASMD_ASCII",
  "ASMD_EQU", "INST_HALT", "INST_INT", "INST_IRET", "INST_CALL",
  "INST_RET", "INST_JMP", "INST_BEQ", "INST_BNE", "INST_BGT", "INST_PUSH",
  "INST_POP", "INST_LD", "INST_ST", "INST_ADD", "INST_SUB", "INST_MUL",
  "INST_DIV", "INST_NOT", "INST_AND", "INST_OR", "INST_XOR", "INST_SHL",
  "INST_SHR", "INST_XCHG", "INST_CSRRD", "INST_CSRWR", "GPR", "CSR",
  "COLON", "COMMA", "IMM_PREFIX", "L_BRACKET", "R_BRACKET", "YYEOL",
  "PLUS", "MINUS", "MUL", "DIV", "NUM_LITERAL", "SYMBOL", "STR_LITERAL",
  "$accept", "program", "lines", "line", "directive", "$@1", "$@2", "$@3",
  "symbol_list", "sym_lit_list", "label", "inst_line", "instruction",
  "$@4", "$@5", "$@6", "$@7", "$@8", "$@9", "$@10", "dest_operand",
  "operand", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  parser::yyrline_[] =
  {
       0,    68,    68,    70,    72,    75,    77,    79,    81,    84,
      84,    86,    86,    88,    90,    90,    92,    94,    96,   100,
     102,   105,   107,   109,   111,   115,   118,   120,   124,   126,
     128,   130,   130,   132,   134,   134,   136,   136,   138,   138,
     140,   140,   142,   144,   146,   148,   150,   152,   154,   156,
     158,   160,   162,   164,   166,   168,   168,   170,   170,   172,
     174,   178,   180,   183,   185,   187,   189,   191,   193,   195,
     197
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 1599 "src/assembler/parser.cpp"

#line 200 "misc/bison.ypp"



void yy::parser::error(const yy::location& l, const std::string& m)
{
    std::cerr << l << ": " << m << std::endl;
}
