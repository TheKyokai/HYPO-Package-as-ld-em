// A Bison parser, made by GNU Bison 3.5.1.

// Skeleton interface for Bison LALR(1) parsers in C++

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


/**
 ** \file inc/assembler/parser.hpp
 ** Define the yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.

#ifndef YY_YY_INC_ASSEMBLER_PARSER_HPP_INCLUDED
# define YY_YY_INC_ASSEMBLER_PARSER_HPP_INCLUDED
// "%code requires" blocks.
#line 10 "misc/bison.ypp"


#include "assembler.hpp"


#line 54 "inc/assembler/parser.hpp"

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif
# include "loc.hpp"
#include <typeinfo>
#ifndef YY_ASSERT
# include <cassert>
# define YY_ASSERT assert
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

namespace yy {
#line 188 "inc/assembler/parser.hpp"




  /// A Bison parser.
  class parser
  {
  public:
#ifndef YYSTYPE
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class semantic_type
  {
  public:
    /// Type of *this.
    typedef semantic_type self_type;

    /// Empty construction.
    semantic_type () YY_NOEXCEPT
      : yybuffer_ ()
      , yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    semantic_type (YY_RVREF (T) t)
      : yytypeid_ (&typeid (T))
    {
      YY_ASSERT (sizeof (T) <= size);
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

    /// Destruction, allowed only if empty.
    ~semantic_type () YY_NOEXCEPT
    {
      YY_ASSERT (!yytypeid_);
    }

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == *that.yytypeid_);
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
      yytypeid_ = YY_NULLPTR;
    }

  private:
    /// Prohibit blind copies.
    self_type& operator= (const self_type&);
    semantic_type (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // GPR
      // CSR
      // NUM_LITERAL
      char dummy1[sizeof (int)];

      // SYMBOL
      // STR_LITERAL
      char dummy2[sizeof (string)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[size];
    } yybuffer_;

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };

#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

      location_type location;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        ASMD_GLOBAL = 3,
        ASMD_EXTERN = 4,
        ASMD_SECTION = 5,
        ASMD_WORD = 6,
        ASMD_SKIP = 7,
        ASMD_END = 8,
        ASMD_ASCII = 9,
        ASMD_EQU = 10,
        INST_HALT = 11,
        INST_INT = 12,
        INST_IRET = 13,
        INST_CALL = 14,
        INST_RET = 15,
        INST_JMP = 16,
        INST_BEQ = 17,
        INST_BNE = 18,
        INST_BGT = 19,
        INST_PUSH = 20,
        INST_POP = 21,
        INST_LD = 22,
        INST_ST = 23,
        INST_ADD = 24,
        INST_SUB = 25,
        INST_MUL = 26,
        INST_DIV = 27,
        INST_NOT = 28,
        INST_AND = 29,
        INST_OR = 30,
        INST_XOR = 31,
        INST_SHL = 32,
        INST_SHR = 33,
        INST_XCHG = 34,
        INST_CSRRD = 35,
        INST_CSRWR = 36,
        GPR = 37,
        CSR = 38,
        COLON = 39,
        COMMA = 40,
        IMM_PREFIX = 41,
        L_BRACKET = 42,
        R_BRACKET = 43,
        YYEOL = 44,
        PLUS = 45,
        MINUS = 46,
        MUL = 47,
        DIV = 48,
        NUM_LITERAL = 49,
        SYMBOL = 50,
        STR_LITERAL = 51
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef signed char token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get ().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ()
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that);
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, location_type&& l)
        : Base (t)
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const location_type& l)
        : Base (t)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, int&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const int& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, string&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const string& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }

      /// Destroy contents, and record that is empty.
      void clear ()
      {
        // User destructor.
        symbol_number_type yytype = this->type_get ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yytype)
        {
       default:
          break;
        }

        // Type destructor.
switch (yytype)
    {
      case 37: // GPR
      case 38: // CSR
      case 49: // NUM_LITERAL
        value.template destroy< int > ();
        break;

      case 50: // SYMBOL
      case 51: // STR_LITERAL
        value.template destroy< string > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_type (by_type&& that);
#endif

      /// Copy constructor.
      by_type (const by_type& that);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_number_type type_get () const YY_NOEXCEPT;

      /// The symbol type.
      /// \a empty_symbol when empty.
      /// An int, not token_number_type, to be able to store empty_symbol.
      int type;
    };

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_type>
    {
      /// Superclass.
      typedef basic_symbol<by_type> super_type;

      /// Empty symbol.
      symbol_type () {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, location_type l)
        : super_type(token_type (tok), std::move (l))
      {
        YY_ASSERT (tok == 0 || tok == token::ASMD_GLOBAL || tok == token::ASMD_EXTERN || tok == token::ASMD_SECTION || tok == token::ASMD_WORD || tok == token::ASMD_SKIP || tok == token::ASMD_END || tok == token::ASMD_ASCII || tok == token::ASMD_EQU || tok == token::INST_HALT || tok == token::INST_INT || tok == token::INST_IRET || tok == token::INST_CALL || tok == token::INST_RET || tok == token::INST_JMP || tok == token::INST_BEQ || tok == token::INST_BNE || tok == token::INST_BGT || tok == token::INST_PUSH || tok == token::INST_POP || tok == token::INST_LD || tok == token::INST_ST || tok == token::INST_ADD || tok == token::INST_SUB || tok == token::INST_MUL || tok == token::INST_DIV || tok == token::INST_NOT || tok == token::INST_AND || tok == token::INST_OR || tok == token::INST_XOR || tok == token::INST_SHL || tok == token::INST_SHR || tok == token::INST_XCHG || tok == token::INST_CSRRD || tok == token::INST_CSRWR || tok == token::COLON || tok == token::COMMA || tok == token::IMM_PREFIX || tok == token::L_BRACKET || tok == token::R_BRACKET || tok == token::YYEOL || tok == token::PLUS || tok == token::MINUS || tok == token::MUL || tok == token::DIV);
      }
#else
      symbol_type (int tok, const location_type& l)
        : super_type(token_type (tok), l)
      {
        YY_ASSERT (tok == 0 || tok == token::ASMD_GLOBAL || tok == token::ASMD_EXTERN || tok == token::ASMD_SECTION || tok == token::ASMD_WORD || tok == token::ASMD_SKIP || tok == token::ASMD_END || tok == token::ASMD_ASCII || tok == token::ASMD_EQU || tok == token::INST_HALT || tok == token::INST_INT || tok == token::INST_IRET || tok == token::INST_CALL || tok == token::INST_RET || tok == token::INST_JMP || tok == token::INST_BEQ || tok == token::INST_BNE || tok == token::INST_BGT || tok == token::INST_PUSH || tok == token::INST_POP || tok == token::INST_LD || tok == token::INST_ST || tok == token::INST_ADD || tok == token::INST_SUB || tok == token::INST_MUL || tok == token::INST_DIV || tok == token::INST_NOT || tok == token::INST_AND || tok == token::INST_OR || tok == token::INST_XOR || tok == token::INST_SHL || tok == token::INST_SHR || tok == token::INST_XCHG || tok == token::INST_CSRRD || tok == token::INST_CSRWR || tok == token::COLON || tok == token::COMMA || tok == token::IMM_PREFIX || tok == token::L_BRACKET || tok == token::R_BRACKET || tok == token::YYEOL || tok == token::PLUS || tok == token::MINUS || tok == token::MUL || tok == token::DIV);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, int v, location_type l)
        : super_type(token_type (tok), std::move (v), std::move (l))
      {
        YY_ASSERT (tok == token::GPR || tok == token::CSR || tok == token::NUM_LITERAL);
      }
#else
      symbol_type (int tok, const int& v, const location_type& l)
        : super_type(token_type (tok), v, l)
      {
        YY_ASSERT (tok == token::GPR || tok == token::CSR || tok == token::NUM_LITERAL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, string v, location_type l)
        : super_type(token_type (tok), std::move (v), std::move (l))
      {
        YY_ASSERT (tok == token::SYMBOL || tok == token::STR_LITERAL);
      }
#else
      symbol_type (int tok, const string& v, const location_type& l)
        : super_type(token_type (tok), v, l)
      {
        YY_ASSERT (tok == token::SYMBOL || tok == token::STR_LITERAL);
      }
#endif
    };

    /// Build a parser object.
    parser (Assembler& assembler_yyarg);
    virtual ~parser ();

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

    // Implementation of make_symbol for each symbol type.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASMD_GLOBAL (location_type l)
      {
        return symbol_type (token::ASMD_GLOBAL, std::move (l));
      }
#else
      static
      symbol_type
      make_ASMD_GLOBAL (const location_type& l)
      {
        return symbol_type (token::ASMD_GLOBAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASMD_EXTERN (location_type l)
      {
        return symbol_type (token::ASMD_EXTERN, std::move (l));
      }
#else
      static
      symbol_type
      make_ASMD_EXTERN (const location_type& l)
      {
        return symbol_type (token::ASMD_EXTERN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASMD_SECTION (location_type l)
      {
        return symbol_type (token::ASMD_SECTION, std::move (l));
      }
#else
      static
      symbol_type
      make_ASMD_SECTION (const location_type& l)
      {
        return symbol_type (token::ASMD_SECTION, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASMD_WORD (location_type l)
      {
        return symbol_type (token::ASMD_WORD, std::move (l));
      }
#else
      static
      symbol_type
      make_ASMD_WORD (const location_type& l)
      {
        return symbol_type (token::ASMD_WORD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASMD_SKIP (location_type l)
      {
        return symbol_type (token::ASMD_SKIP, std::move (l));
      }
#else
      static
      symbol_type
      make_ASMD_SKIP (const location_type& l)
      {
        return symbol_type (token::ASMD_SKIP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASMD_END (location_type l)
      {
        return symbol_type (token::ASMD_END, std::move (l));
      }
#else
      static
      symbol_type
      make_ASMD_END (const location_type& l)
      {
        return symbol_type (token::ASMD_END, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASMD_ASCII (location_type l)
      {
        return symbol_type (token::ASMD_ASCII, std::move (l));
      }
#else
      static
      symbol_type
      make_ASMD_ASCII (const location_type& l)
      {
        return symbol_type (token::ASMD_ASCII, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASMD_EQU (location_type l)
      {
        return symbol_type (token::ASMD_EQU, std::move (l));
      }
#else
      static
      symbol_type
      make_ASMD_EQU (const location_type& l)
      {
        return symbol_type (token::ASMD_EQU, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_HALT (location_type l)
      {
        return symbol_type (token::INST_HALT, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_HALT (const location_type& l)
      {
        return symbol_type (token::INST_HALT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_INT (location_type l)
      {
        return symbol_type (token::INST_INT, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_INT (const location_type& l)
      {
        return symbol_type (token::INST_INT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_IRET (location_type l)
      {
        return symbol_type (token::INST_IRET, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_IRET (const location_type& l)
      {
        return symbol_type (token::INST_IRET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_CALL (location_type l)
      {
        return symbol_type (token::INST_CALL, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_CALL (const location_type& l)
      {
        return symbol_type (token::INST_CALL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_RET (location_type l)
      {
        return symbol_type (token::INST_RET, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_RET (const location_type& l)
      {
        return symbol_type (token::INST_RET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_JMP (location_type l)
      {
        return symbol_type (token::INST_JMP, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_JMP (const location_type& l)
      {
        return symbol_type (token::INST_JMP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_BEQ (location_type l)
      {
        return symbol_type (token::INST_BEQ, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_BEQ (const location_type& l)
      {
        return symbol_type (token::INST_BEQ, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_BNE (location_type l)
      {
        return symbol_type (token::INST_BNE, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_BNE (const location_type& l)
      {
        return symbol_type (token::INST_BNE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_BGT (location_type l)
      {
        return symbol_type (token::INST_BGT, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_BGT (const location_type& l)
      {
        return symbol_type (token::INST_BGT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_PUSH (location_type l)
      {
        return symbol_type (token::INST_PUSH, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_PUSH (const location_type& l)
      {
        return symbol_type (token::INST_PUSH, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_POP (location_type l)
      {
        return symbol_type (token::INST_POP, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_POP (const location_type& l)
      {
        return symbol_type (token::INST_POP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_LD (location_type l)
      {
        return symbol_type (token::INST_LD, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_LD (const location_type& l)
      {
        return symbol_type (token::INST_LD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_ST (location_type l)
      {
        return symbol_type (token::INST_ST, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_ST (const location_type& l)
      {
        return symbol_type (token::INST_ST, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_ADD (location_type l)
      {
        return symbol_type (token::INST_ADD, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_ADD (const location_type& l)
      {
        return symbol_type (token::INST_ADD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_SUB (location_type l)
      {
        return symbol_type (token::INST_SUB, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_SUB (const location_type& l)
      {
        return symbol_type (token::INST_SUB, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_MUL (location_type l)
      {
        return symbol_type (token::INST_MUL, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_MUL (const location_type& l)
      {
        return symbol_type (token::INST_MUL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_DIV (location_type l)
      {
        return symbol_type (token::INST_DIV, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_DIV (const location_type& l)
      {
        return symbol_type (token::INST_DIV, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_NOT (location_type l)
      {
        return symbol_type (token::INST_NOT, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_NOT (const location_type& l)
      {
        return symbol_type (token::INST_NOT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_AND (location_type l)
      {
        return symbol_type (token::INST_AND, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_AND (const location_type& l)
      {
        return symbol_type (token::INST_AND, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_OR (location_type l)
      {
        return symbol_type (token::INST_OR, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_OR (const location_type& l)
      {
        return symbol_type (token::INST_OR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_XOR (location_type l)
      {
        return symbol_type (token::INST_XOR, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_XOR (const location_type& l)
      {
        return symbol_type (token::INST_XOR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_SHL (location_type l)
      {
        return symbol_type (token::INST_SHL, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_SHL (const location_type& l)
      {
        return symbol_type (token::INST_SHL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_SHR (location_type l)
      {
        return symbol_type (token::INST_SHR, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_SHR (const location_type& l)
      {
        return symbol_type (token::INST_SHR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_XCHG (location_type l)
      {
        return symbol_type (token::INST_XCHG, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_XCHG (const location_type& l)
      {
        return symbol_type (token::INST_XCHG, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_CSRRD (location_type l)
      {
        return symbol_type (token::INST_CSRRD, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_CSRRD (const location_type& l)
      {
        return symbol_type (token::INST_CSRRD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INST_CSRWR (location_type l)
      {
        return symbol_type (token::INST_CSRWR, std::move (l));
      }
#else
      static
      symbol_type
      make_INST_CSRWR (const location_type& l)
      {
        return symbol_type (token::INST_CSRWR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GPR (int v, location_type l)
      {
        return symbol_type (token::GPR, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_GPR (const int& v, const location_type& l)
      {
        return symbol_type (token::GPR, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CSR (int v, location_type l)
      {
        return symbol_type (token::CSR, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CSR (const int& v, const location_type& l)
      {
        return symbol_type (token::CSR, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COLON (location_type l)
      {
        return symbol_type (token::COLON, std::move (l));
      }
#else
      static
      symbol_type
      make_COLON (const location_type& l)
      {
        return symbol_type (token::COLON, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COMMA (location_type l)
      {
        return symbol_type (token::COMMA, std::move (l));
      }
#else
      static
      symbol_type
      make_COMMA (const location_type& l)
      {
        return symbol_type (token::COMMA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IMM_PREFIX (location_type l)
      {
        return symbol_type (token::IMM_PREFIX, std::move (l));
      }
#else
      static
      symbol_type
      make_IMM_PREFIX (const location_type& l)
      {
        return symbol_type (token::IMM_PREFIX, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_L_BRACKET (location_type l)
      {
        return symbol_type (token::L_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_L_BRACKET (const location_type& l)
      {
        return symbol_type (token::L_BRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_R_BRACKET (location_type l)
      {
        return symbol_type (token::R_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_R_BRACKET (const location_type& l)
      {
        return symbol_type (token::R_BRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYEOL (location_type l)
      {
        return symbol_type (token::YYEOL, std::move (l));
      }
#else
      static
      symbol_type
      make_YYEOL (const location_type& l)
      {
        return symbol_type (token::YYEOL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PLUS (location_type l)
      {
        return symbol_type (token::PLUS, std::move (l));
      }
#else
      static
      symbol_type
      make_PLUS (const location_type& l)
      {
        return symbol_type (token::PLUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MINUS (location_type l)
      {
        return symbol_type (token::MINUS, std::move (l));
      }
#else
      static
      symbol_type
      make_MINUS (const location_type& l)
      {
        return symbol_type (token::MINUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MUL (location_type l)
      {
        return symbol_type (token::MUL, std::move (l));
      }
#else
      static
      symbol_type
      make_MUL (const location_type& l)
      {
        return symbol_type (token::MUL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIV (location_type l)
      {
        return symbol_type (token::DIV, std::move (l));
      }
#else
      static
      symbol_type
      make_DIV (const location_type& l)
      {
        return symbol_type (token::DIV, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NUM_LITERAL (int v, location_type l)
      {
        return symbol_type (token::NUM_LITERAL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_NUM_LITERAL (const int& v, const location_type& l)
      {
        return symbol_type (token::NUM_LITERAL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SYMBOL (string v, location_type l)
      {
        return symbol_type (token::SYMBOL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_SYMBOL (const string& v, const location_type& l)
      {
        return symbol_type (token::SYMBOL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STR_LITERAL (string v, location_type l)
      {
        return symbol_type (token::STR_LITERAL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_STR_LITERAL (const string& v, const location_type& l)
      {
        return symbol_type (token::STR_LITERAL, v, l);
      }
#endif


  private:
    /// This class is not copyable.
    parser (const parser&);
    parser& operator= (const parser&);

    /// Check the lookahead yytoken.
    /// \returns  true iff the token will be eventually shifted.
    bool yy_lac_check_ (int yytoken) const;
    /// Establish the initial context if no initial context currently exists.
    /// \returns  true iff the token will be eventually shifted.
    bool yy_lac_establish_ (int yytoken);
    /// Discard any previous initial lookahead context because of event.
    /// \param event  the event which caused the lookahead to be discarded.
    ///               Only used for debbuging output.
    void yy_lac_discard_ (const char* event);

    /// Stored state numbers (used for stacks).
    typedef unsigned char state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yyla      the lookahead token.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         const symbol_type& yyla) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    /// In theory \a t should be a token_type, but character literals
    /// are valid, yet not members of the token_type enum.
    static token_number_type yytranslate_ (int t);

    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const signed char yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const signed char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const signed char yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const short yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const unsigned char yytable_[];

    static const short yycheck_[];

    // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
    // symbol of state STATE-NUM.
    static const signed char yystos_[];

    // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
    static const signed char yyr1_[];

    // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
    static const signed char yyr2_[];


    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);


    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const unsigned char yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// \a empty_symbol when empty.
      symbol_number_type type_get () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::reverse_iterator iterator;
      typedef typename S::const_reverse_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200)
        : seq_ (n)
      {}

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      std::ptrdiff_t
      ssize () const YY_NOEXCEPT
      {
        return std::ptrdiff_t (size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.rbegin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.rend ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range)
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
      stack (const stack&);
      stack& operator= (const stack&);
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;
    /// The stack for LAC.
    /// Logically, the yy_lac_stack's lifetime is confined to the function
    /// yy_lac_check_. We just store it as a member of this class to hold
    /// on to the memory and to avoid frequent reallocations.
    /// Since yy_lac_check_ is const, this member must be mutable.
    mutable std::vector<state_type> yylac_stack_;
    /// Whether an initial LAC context was established.
    bool yy_lac_established_;


    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1);

    /// Some specific tokens.
    static const token_number_type yy_error_token_ = 1;
    static const token_number_type yy_undef_token_ = 2;

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 167,     ///< Last index in yytable_.
      yynnts_ = 22,  ///< Number of nonterminal symbols.
      yyfinal_ = 3, ///< Termination state number.
      yyntokens_ = 52  ///< Number of tokens.
    };


    // User arguments.
    Assembler& assembler;
  };

  inline
  parser::token_number_type
  parser::yytranslate_ (int t)
  {
    return static_cast<token_number_type> (t);
  }

  // basic_symbol.
#if 201103L <= YY_CPLUSPLUS
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (basic_symbol&& that)
    : Base (std::move (that))
    , value ()
    , location (std::move (that.location))
  {
    switch (this->type_get ())
    {
      case 37: // GPR
      case 38: // CSR
      case 49: // NUM_LITERAL
        value.move< int > (std::move (that.value));
        break;

      case 50: // SYMBOL
      case 51: // STR_LITERAL
        value.move< string > (std::move (that.value));
        break;

      default:
        break;
    }

  }
#endif

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->type_get ())
    {
      case 37: // GPR
      case 38: // CSR
      case 49: // NUM_LITERAL
        value.copy< int > (YY_MOVE (that.value));
        break;

      case 50: // SYMBOL
      case 51: // STR_LITERAL
        value.copy< string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->type_get ())
    {
      case 37: // GPR
      case 38: // CSR
      case 49: // NUM_LITERAL
        value.move< int > (YY_MOVE (s.value));
        break;

      case 50: // SYMBOL
      case 51: // STR_LITERAL
        value.move< string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_type.
  inline
  parser::by_type::by_type ()
    : type (empty_symbol)
  {}

#if 201103L <= YY_CPLUSPLUS
  inline
  parser::by_type::by_type (by_type&& that)
    : type (that.type)
  {
    that.clear ();
  }
#endif

  inline
  parser::by_type::by_type (const by_type& that)
    : type (that.type)
  {}

  inline
  parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  parser::by_type::type_get () const YY_NOEXCEPT
  {
    return type;
  }

} // yy
#line 1971 "inc/assembler/parser.hpp"



// "%code provides" blocks.
#line 17 "misc/bison.ypp"


yy::parser::symbol_type yylex();


#line 1982 "inc/assembler/parser.hpp"


#endif // !YY_YY_INC_ASSEMBLER_PARSER_HPP_INCLUDED
