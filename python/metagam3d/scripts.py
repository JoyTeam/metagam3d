from _metagam3d import Variant, DynamicValue, ScriptExpressionProgram, ScriptGlobInstruction, getTime
from _metagam3d import ScriptConstInstruction, ScriptUnaryInstruction
from _metagam3d import ScriptBinaryInstruction, ScriptTernaryInstruction
from metagam3d.script_parser import ScriptParser, ScriptParserError, ScriptParserResult
from metagam3d import Parsing
import sys

class SyntaxTreeError(Exception):
    pass

_parser = {
    "unaryOps": {
        '-': ScriptUnaryInstruction.MINUS,
        'not': ScriptUnaryInstruction.NOT
    },
    "binaryOps": {
        '+': ScriptBinaryInstruction.PLUS,
        '-': ScriptBinaryInstruction.MINUS,
        '*': ScriptBinaryInstruction.MULT,
        '/': ScriptBinaryInstruction.DIV,
        '==': ScriptBinaryInstruction.EQUAL,
        '!=': ScriptBinaryInstruction.NOTEQUAL,
        '<': ScriptBinaryInstruction.LT,
        '<=': ScriptBinaryInstruction.LE,
        '>': ScriptBinaryInstruction.GT,
        '>=': ScriptBinaryInstruction.GE,
        'and': ScriptBinaryInstruction.AND,
        'or': ScriptBinaryInstruction.OR,
        '%': ScriptBinaryInstruction.MOD
    },
    "ternaryOps": {
        '?': ScriptTernaryInstruction.QUESTION
    }
}

def parser_spec():
    try:
        return _parser["spec"]
    except KeyError:
        spec = Parsing.Spec(sys.modules["metagam3d.script_parser"], skinny=False)
        _parser["spec"] = spec
        return spec

def mmoscript_expression_parse(text):
    parser = ScriptParser(parser_spec())
    try:
        parser.scan(text)
        # Tell the parser that the end of input has been reached.
        try:
            parser.eoi()
        except Parsing.SyntaxError as e:
            raise ScriptParserError("Expression unexpectedly ended", e)
    except ScriptParserResult as e:
        return e.val

def m3d_compile(val, till=None):
    if type(val) is not list:
        return DynamicValue(Variant(val))
    program = ScriptExpressionProgram()
    _m3d_compile(val, program)
    val = DynamicValue(program)
    if till is not None:
        val.setTill(till)
    return val

def _m3d_compile(val, program):
    if type(val) is not list:
        program.addInstruction(ScriptConstInstruction(Variant(val)))
    else:
        cmd = val[0]
        if cmd == "glob":
            program.addInstruction(ScriptGlobInstruction(val[1]))
        elif cmd in _parser["unaryOps"] and len(val) == 2:
            _m3d_compile(val[1], program)
            program.addInstruction(ScriptUnaryInstruction(_parser["unaryOps"][cmd]))
        elif cmd in _parser["binaryOps"] and len(val) == 3:
            _m3d_compile(val[1], program)
            _m3d_compile(val[2], program)
            program.addInstruction(ScriptBinaryInstruction(_parser["binaryOps"][cmd]))
        elif cmd in _parser["ternaryOps"] and len(val) == 4:
            _m3d_compile(val[1], program)
            _m3d_compile(val[2], program)
            _m3d_compile(val[3], program)
            program.addInstruction(ScriptTernaryInstruction(_parser["ternaryOps"][cmd]))
        else:
            raise SyntaxTreeError("Instruction '%s' is not supported" % cmd)

def m3d_expr(text, till=None):
    return m3d_compile(mmoscript_expression_parse(text), till)
