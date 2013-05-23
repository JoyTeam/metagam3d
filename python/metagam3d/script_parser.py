from metagam3d import Parsing
from metagam3d.math import Vec3d
import re

re_newline = re.compile(r'\n')
re_numeric = re.compile(r'^#(.+)')

class ScriptParserError(Exception):
    def __init__(self, val, exc=None, **kwargs):
        self.val = val
        self.exc = exc
        self.kwargs = kwargs

    def __str__(self):
        return self.val.format(**self.kwargs)

    def __repr__(self):
        return self.val.format(**self.kwargs)

class ScriptParserResult(Exception):
    def __init__(self, val):
        self.val = val

#===============================================================================
# Tokens/precedences.  See Parsing documentation to learn about the
# significance of left-associative precedence.

class PQuestionOp(Parsing.Precedence):
    "%right pQuestionOp"
class TokenQuestion(Parsing.Token):
    "%token question [pQuestionOp]"
class TokenColon(Parsing.Token):
    "%token colon [pQuestionOp]"

class POrOp(Parsing.Precedence):
    "%left pOrOp >pQuestionOp"
class TokenOr(Parsing.Token):
    "%token or [pOrOp]"
class PAndOp(Parsing.Precedence):
    "%left pAndOp >pOrOp"
class TokenAnd(Parsing.Token):
    "%token and [pAndOp]"
class PNotOp(Parsing.Precedence):
    "%left pNotOp >pAndOp"
class TokenNot(Parsing.Token):
    "%token not [pNotOp]"

class PBitOrOp(Parsing.Precedence):
    "%left pBitOrOp >pNotOp"
class TokenBitOr(Parsing.Token):
    "%token bitor [pBitOrOp]"
class PBitAndOp(Parsing.Precedence):
    "%left pBitAndOp >pBitOrOp"
class TokenBitAnd(Parsing.Token):
    "%token bitand [pBitAndOp]"
class PBitNotOp(Parsing.Precedence):
    "%left pBitNotOp >pBitAndOp"
class TokenBitNot(Parsing.Token):
    "%token bitnot [pBitNotOp]"

class PCompareOp(Parsing.Precedence):
    "%left pCompareOp >pBitNotOp"
class TokenEquals(Parsing.Token):
    "%token equals [pCompareOp]"
class TokenNotEquals(Parsing.Token):
    "%token notequals [pCompareOp]"
class TokenLessThan(Parsing.Token):
    "%token lt [pCompareOp]"
class TokenGreaterThan(Parsing.Token):
    "%token gt [pCompareOp]"
class TokenLessEqual(Parsing.Token):
    "%token le [pCompareOp]"
class TokenGreaterEqual(Parsing.Token):
    "%token ge [pCompareOp]"
class TokenIn(Parsing.Token):
    "%token in [pCompareOp]"

class PAddOp(Parsing.Precedence):
    "%left pAddOp >pCompareOp"
class TokenPlus(Parsing.Token):
    "%token plus [pAddOp]"
class TokenMinus(Parsing.Token):
    "%token minus [pAddOp]"

class PMulOp(Parsing.Precedence):
    "%left pMulOp >pAddOp"
class TokenStar(Parsing.Token):
    "%token star [pMulOp]"
class TokenSlash(Parsing.Token):
    "%token slash [pMulOp]"
class TokenMod(Parsing.Token):
    "%token mod [pMulOp]"

class PDotOp(Parsing.Precedence):
    "%left pDotOp >pMulOp"
class TokenDot(Parsing.Token):
    "%token dot [pDotOp]"

class TokenNone(Parsing.Token):
    "%token nonetoken"

class TokenComma(Parsing.Token):
    "%token comma"

class TokenFunc(Parsing.Token):
    "%token func"

class TokenOnline(Parsing.Token):
    "%token online"

class TokenScalar(Parsing.Token):
    "%token scalar"
    def __init__(self, parser, val):
        Parsing.Token.__init__(self, parser)
        self.val = val

class TokenIdentifier(Parsing.Token):
    "%token identifier"
    def __init__(self, parser, val):
        Parsing.Token.__init__(self, parser)
        self.val = val

class TokenParLeft(Parsing.Token):
    "%token parleft"
class TokenParRight(Parsing.Token):
    "%token parright"

class TokenCurlyLeft(Parsing.Token):
    "%token curlyleft"
class TokenCurlyRight(Parsing.Token):
    "%token curlyright"

class TokenAssign(Parsing.Token):
    "%token assign"

class TokenRandom(Parsing.Token):
    "%token random"

class TokenEquip(Parsing.Token):
    "%token equip"

class TokenMember(Parsing.Token):
    "%token member"

#===============================================================================
# Nonterminals, with associated productions.  In traditional BNF, the following
# productions would look something like:

class AddOp(Parsing.Nonterm):
    "%nonterm"
    def reducePlus(self, plus):
	"%reduce plus"
	self.variant = "plus"

    def reduceMinus(self, minus):
	"%reduce minus"
	self.variant = "minus"

class MulOp(Parsing.Nonterm):
    "%nonterm"
    def reduceStar(self, star):
	"%reduce star"
	self.variant = "star"

    def reduceSlash(self, slash):
	"%reduce slash"
	self.variant = "slash"

    def reduceMod(self, slash):
	"%reduce mod"
	self.variant = "mod"

class List(Parsing.Nonterm):
    "%nonterm"
    def reduceFirst(self, expr):
        "%reduce Expr"
        self.val = [expr.val]

    def reduceNext(self, lst, Comma, expr):
        "%reduce List comma Expr"
        self.val = lst.val + [expr.val]

class PAttrKey(Parsing.Precedence):
    "%left pAttrKey"

class AttrKey(Parsing.Nonterm):
    "%nonterm [pAttrKey]"
    def reduceIdentifier(self, identifier):
        "%reduce identifier"
        self.val = identifier.val

    def reduceOnline(self, online):
        "%reduce online"
        self.val = "online"

    def reduceEquip(self, equip):
        "%reduce equip"
        self.val = "equip"

class Expr(Parsing.Nonterm):
    "%nonterm"
    def reduceScalar(self, s):
	"%reduce scalar"
	self.val = s.val

    def reduceRandom(self, r):
        "%reduce random"
        self.val = ["random"]

    def reduceNone(self, n):
        "%reduce nonetoken"
        self.val = None

    def reduceIdentifier(self, i):
	"%reduce identifier"
	self.val = ["glob", i.val]

    def recuceDot(self, exprA, d, ident):
        "%reduce Expr dot AttrKey"
        self.val = [".", exprA.val, ident.val]

    def reduceVec3d(self, ParLeft, x, c1, y, c2, z, ParRight):
        "%reduce parleft scalar comma scalar comma scalar parright"
        self.val = Vec3d(x.val, y.val, z.val)

    def reducePar(self, ParLeft, ex, ParRight):
        "%reduce parleft Expr parright"
        self.val = ex.val

    def reduceQuestion(self, exprA, q, exprB, c, exprC):
        "%reduce Expr question Expr colon Expr [pQuestionOp]"
        self.val = ["?", exprA.val, exprB.val, exprC.val]

    def reduceEquals(self, exprA, EqualsOp, exprB):
        "%reduce Expr equals Expr [pCompareOp]"
        self.val = ["==", exprA.val, exprB.val]

    def reduceNotEquals(self, exprA, NotEqualsOp, exprB):
        "%reduce Expr notequals Expr [pCompareOp]"
        self.val = ["!=", exprA.val, exprB.val]

    def reduceLessThan(self, exprA, LessThanOp, exprB):
        "%reduce Expr lt Expr [pCompareOp]"
        self.val = ["<", exprA.val, exprB.val]

    def reduceGreaterThan(self, exprA, GreaterThanOp, exprB):
        "%reduce Expr gt Expr [pCompareOp]"
        self.val = [">", exprA.val, exprB.val]

    def reduceLessEqThan(self, exprA, LessThanOp, exprB):
        "%reduce Expr le Expr [pCompareOp]"
        self.val = ["<=", exprA.val, exprB.val]

    def reduceGreaterEqThan(self, exprA, GreaterThanOp, exprB):
        "%reduce Expr ge Expr [pCompareOp]"
        self.val = [">=", exprA.val, exprB.val]

    def reduceBitNot(self, NotOp, exprA):
        "%reduce bitnot Expr [pBitNotOp]"
        self.val = ["~", exprA.val]

    def reduceBitAnd(self, exprA, AndOp, exprB):
        "%reduce Expr bitand Expr [pBitAndOp]"
        self.val = ["&", exprA.val, exprB.val]

    def reduceBitOr(self, exprA, OrOp, exprB):
        "%reduce Expr bitor Expr [pBitOrOp]"
        self.val = ["|", exprA.val, exprB.val]

    def reduceNot(self, NotOp, exprA):
        "%reduce not Expr [pNotOp]"
        self.val = ["not", exprA.val]

    def reduceAnd(self, exprA, AndOp, exprB):
        "%reduce Expr and Expr [pAndOp]"
        self.val = ["and", exprA.val, exprB.val]

    def reduceOr(self, exprA, OrOp, exprB):
        "%reduce Expr or Expr [pOrOp]"
        self.val = ["or", exprA.val, exprB.val]

    def reduceAdd(self, exprA, AddOp, exprB):
	"%reduce Expr AddOp Expr [pAddOp]"
	if AddOp.variant == "plus":
    	    self.val = ["+", exprA.val, exprB.val]
	elif AddOp.variant == "minus":
            self.val = ["-", exprA.val, exprB.val]

    def reduceMul(self, exprA, MulOp, exprB):
	"%reduce Expr MulOp Expr [pMulOp]"
	if MulOp.variant == "star":
            self.val = ["*", exprA.val, exprB.val]
	elif MulOp.variant == "slash":
            self.val = ["/", exprA.val, exprB.val]
	elif MulOp.variant == "mod":
            self.val = ["%", exprA.val, exprB.val]

    exprFuncs = set(["min", "max", "uc", "lc", "selrand", "floor",
        "round", "ceil", "abs", "sqrt", "sqr", "pow", "log", "exp", "sin", "cos", "tan", "asin", "acos", "atan"])

    def reduceFunc(self, func, ParLeft, lst, ParRight):
        "%reduce func parleft List parright"
        if func.fname not in self.exprFuncs:
            raise Parsing.SyntaxError(func.script_parser._("Function %s is not supported in expression context") % func.fname)
        self.val = ["call", func.fname] + lst.val

    def reduceIn(self, exprA, op, exprB):
        "%reduce Expr in Expr [pCompareOp]"
        self.val = ["in", exprA.val, exprB.val]

    def reduceMember(self, m):
        "%reduce member"
        self.val = ["glob", "member"]

# This is the start symbol; there can be only one such class in the grammar.
class Result(Parsing.Nonterm):
    "%start"
    def reduce(self, e):
	"%reduce Expr"
        raise ScriptParserResult(e.val)

class ScriptParser(Parsing.Glr):
    re_token = re.compile(r'(\s*)((-?\d+\.\d+)|(-?\d+)|(==|!=|>=|<=|=|>|<|\+|-|\*|\&|\||~|/|%|\.|,|\(|\)|\?|:|{|})|"((?:\\.|[^"])*)"|\'((?:\\.|[^\'])*)\'|([a-z_][a-z_0-9]*))', re.IGNORECASE)
    syms = {
        "+": TokenPlus,
        "-": TokenMinus,
        "*": TokenStar,
        "/": TokenSlash,
        "%": TokenMod,
        "(": TokenParLeft,
        ")": TokenParRight,
        "{": TokenCurlyLeft,
        "}": TokenCurlyRight,
        "?": TokenQuestion,
        ":": TokenColon,
        ".": TokenDot,
        ",": TokenComma,
        "==": TokenEquals,
        "!=": TokenNotEquals,
        "=": TokenAssign,
        ">": TokenGreaterThan,
        "<": TokenLessThan,
        ">=": TokenGreaterEqual,
        "<=": TokenLessEqual,
        "none": TokenNone,
        "and": TokenAnd,
        "or": TokenOr,
        "not": TokenNot,
        "random": TokenRandom,
        "online": TokenOnline,
        "equip": TokenEquip,
        "in": TokenIn,
        "member": TokenMember,
        "~": TokenBitNot,
        "&": TokenBitAnd,
        "|": TokenBitOr,
    }
    funcs = set(["min", "max", "uc", "lc", "selrand", "floor",
        "round", "ceil", "abs", "sqrt", "sqr", "pow", "log", "exp", "sin", "cos", "tan", "asin", "acos", "atan"])

    def _(self, text):
        return text

    def scan(self, input):
        input = input.strip()
        pos = 0
        tokens = []
        last_dot = False
        while True:
            token_match = type(self).re_token.match(input, pos)
            if not token_match:
                if pos < len(input):
                    raise ScriptParserError(self._("Error parsing '{expression}': {error}"), expression=u"".join(tokens).strip(), error=self._("unexpected end"))
                else:
                    break
            res = token_match.groups()
            if res[0]:
                tokens.append(" ")
            tokens.append(res[1])
            token = None
            dot = False
            if res[2] is not None:
                token = TokenScalar(self, float(res[2]))
            elif res[3] is not None:
                token = TokenScalar(self,int(res[3]))
            elif res[4] is not None:
                if res[4] == ".":
                    dot = True
                cls = type(self).syms.get(res[4])
                if cls:
                    token = cls(self)
            elif res[5] is not None:
                token = TokenScalar(self, unquotestr(res[5]))
            elif res[6] is not None:
                token = TokenScalar(self, unquotestr(res[6]))
            elif res[7] is not None:
                if last_dot:
                    token = TokenIdentifier(self, res[7])
                else:
                    cls = type(self).syms.get(res[7])
                    if cls:
                        token = cls(self)
                    else:
                        if res[7] in type(self).funcs:
                            token = TokenFunc(self)
                            token.fname = res[7]
                        else:
                            token = TokenIdentifier(self, res[7])
            if token is None:
                data = input[pos:pos+10]
                raise ScriptParserError(self._("Error parsing '{expression}': {error}"), expression=u"".join(tokens).strip(), error=self._("unexpected symbols: %s") % data)
            token.script_parser = self
            try:
                self.token(token)
            except Parsing.SyntaxError as exc:
                raise ScriptParserError(self._("Error parsing '{expression}': {error}"), expression=u"".join(tokens).strip(), error=exc)
            pos = token_match.end()
            if re_newline.search(res[0]):
                tokens = tokens[len(tokens)-1:]
            last_dot = dot

