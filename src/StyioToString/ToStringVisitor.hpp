#pragma once
#ifndef STYIO_TO_STRING_VISITOR_H_
#define STYIO_TO_STRING_VISITOR_H_

// [STL]
#include <iostream>
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

// [Styio]
#include "../StyioAST/ASTDecl.hpp"
#include "../StyioIR/IRDecl.hpp"

// Generic Visitor
template <typename... Types>
class ToStringVisitor;

template <typename T>
class ToStringVisitor<T>
{
public:
  virtual std::string toString(T* t, int indent = 0) = 0;
};

template <typename T, typename... Types>
class ToStringVisitor<T, Types...> : public ToStringVisitor<Types...>
{
public:
  using ToStringVisitor<Types...>::toString;
  virtual std::string toString(T* t, int indent = 0) = 0;
};

using StyioToStringVisitor = ToStringVisitor<
  class CommentAST,

  class NoneAST,
  class EmptyAST,

  class BoolAST,
  class IntAST,
  class FloatAST,
  class CharAST,

  class StringAST,
  class SetAST,
  class ListAST,

  class StructAST,
  class TupleAST,

  class NameAST,
  class DTypeAST,

  class VarAST,
  class ArgAST,
  class OptArgAST,
  class OptKwArgAST,

  class FlexBindAST,
  class FinalBindAST,

  class BinCompAST,
  class CondAST,
  class BinOpAST,

  class AnonyFuncAST,
  class FuncAST,

  class CallAST,

  class SizeOfAST,
  class TypeConvertAST,
  class ListOpAST,
  class RangeAST,

  class IterAST,
  class LoopAST,

  class CondFlowAST,

  class EOFAST,
  class PassAST,
  class BreakAST,
  class ReturnAST,

  class CasesAST,
  class MatchCasesAST,

  class BlockAST,
  class MainBlockAST,

  class ExtPackAST,

  class InfiniteAST,

  class VarTupleAST,

  class ForwardAST,
  class CheckEqAST,
  class CheckIsinAST,
  class FromToAST,

  class FmtStrAST,

  class ResourceAST,

  class LocalPathAST,
  class RemotePathAST,
  class WebUrlAST,
  class DBUrlAST,

  class PrintAST,
  class ReadFileAST>;

class StyioRepr : public StyioToStringVisitor
{
public:
  StyioRepr() {}

  ~StyioRepr() {}

  /* styio.ast.toString() */

  std::string toString(BoolAST* ast, int indent = 0);

  std::string toString(NoneAST* ast, int indent = 0);

  std::string toString(EOFAST* ast, int indent = 0);

  std::string toString(EmptyAST* ast, int indent = 0);

  std::string toString(PassAST* ast, int indent = 0);

  std::string toString(BreakAST* ast, int indent = 0);

  std::string toString(ReturnAST* ast, int indent = 0);

  std::string toString(CommentAST* ast, int indent = 0);

  std::string toString(NameAST* ast, int indent = 0);

  std::string toString(VarAST* ast, int indent = 0);

  std::string toString(ArgAST* ast, int indent = 0);

  std::string toString(OptArgAST* ast, int indent = 0);

  std::string toString(OptKwArgAST* ast, int indent = 0);

  std::string toString(VarTupleAST* ast, int indent = 0);

  std::string toString(DTypeAST* ast, int indent = 0);

  std::string toString(IntAST* ast, int indent = 0);

  std::string toString(FloatAST* ast, int indent = 0);

  std::string toString(CharAST* ast, int indent = 0);

  std::string toString(StringAST* ast, int indent = 0);

  std::string toString(TypeConvertAST* ast, int indent = 0);

  std::string toString(FmtStrAST* ast, int indent = 0);

  std::string toString(LocalPathAST* ast, int indent = 0);

  std::string toString(RemotePathAST* ast, int indent = 0);

  std::string toString(WebUrlAST* ast, int indent = 0);

  std::string toString(DBUrlAST* ast, int indent = 0);

  std::string toString(ListAST* ast, int indent = 0);

  std::string toString(TupleAST* ast, int indent = 0);

  std::string toString(SetAST* ast, int indent = 0);

  std::string toString(RangeAST* ast, int indent = 0);

  std::string toString(SizeOfAST* ast, int indent = 0);

  std::string toString(BinOpAST* ast, int indent = 0);

  std::string toString(BinCompAST* ast, int indent = 0);

  std::string toString(CondAST* ast, int indent = 0);

  std::string toString(CallAST* ast, int indent = 0);

  std::string toString(ListOpAST* ast, int indent = 0);

  std::string toString(ResourceAST* ast, int indent = 0);

  std::string toString(FlexBindAST* ast, int indent = 0);

  std::string toString(FinalBindAST* ast, int indent = 0);

  std::string toString(StructAST* ast, int indent = 0);

  std::string toString(ReadFileAST* ast, int indent = 0);

  std::string toString(PrintAST* ast, int indent = 0);

  std::string toString(ExtPackAST* ast, int indent = 0);

  std::string toString(BlockAST* ast, int indent = 0);

  std::string toString(CasesAST* ast, int indent = 0);

  std::string toString(CondFlowAST* ast, int indent = 0);

  std::string toString(CheckEqAST* ast, int indent = 0);

  std::string toString(CheckIsinAST* ast, int indent = 0);

  std::string toString(FromToAST* ast, int indent = 0);

  std::string toString(ForwardAST* ast, int indent = 0);

  std::string toString(InfiniteAST* ast, int indent = 0);

  std::string toString(AnonyFuncAST* ast, int indent = 0);

  std::string toString(FuncAST* ast, int indent = 0);

  std::string toString(LoopAST* ast, int indent = 0);

  std::string toString(IterAST* ast, int indent = 0);

  std::string toString(MatchCasesAST* ast, int indent = 0);

  std::string toString(MainBlockAST* ast, int indent = 0);

  /* styio.ir.toString() */
};

#endif  // STYIO_TO_STRING_VISITOR_H_