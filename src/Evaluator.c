/**
 * @file Evaluator.c
 * @author: Zhang Hai
 */

#include "Evaluator.h"

#include <math.h>

#include "zhclib/Stack.h"


double factorial(unsigned int operand) {
    double result = 1;
    if (operand == 0 || operand == 1) {
        return 1;
    }
    do {
        result *= operand--;
    } while (operand > 1);
    return result;
}

/**
 * Evaluate the stack reversely, so the operator stack should be kept
 * in precedence ascendant order.
 */

typedef enum {
    OPERATOR_ADDITION,
    OPERATOR_SUBTRACTION,
    OPERATOR_MULPLICATION,
    OPERATOR_DIVISION,
    OPERATOR_POWER,
    OPERATOR_FACTORIAL,
    OPERATOR_PARENTHESIS_LEFT,
    OPERATOR_SIN,
    OPERATOR_COS,
    OPERATOR_TAN,
    OPERATOR_POW,
    OPERATOR_LOG,
    OPERATOR_COMMA,
    OPERATOR_PARENTHESIS_RIGHT
} Operator;

static int OPERATOR_PRECEDENCE[] = {
    1,
    1,
    2,
    2,
    3,
    4,
    5,
    5,
    5,
    5,
    5,
    5,
    0,
    0
};

static string OPERATOR_STRINGS[] = {
    "+",
    "-",
    "*",
    "/",
    "^",
    "!",
    "(",
    "sin(",
    "cos(",
    "tan(",
    "pow(",
    "log(",
    ",",
    ")"
};

int Operator_comparePrecedence(Operator operator1,
        Operator operator2) {
    if (OPERATOR_PRECEDENCE[operator1]
                    == OPERATOR_PRECEDENCE[OPERATOR_PARENTHESIS_LEFT]
            || operator1 == OPERATOR_COMMA) {
        /* Magic left parenthesis & comma! */
        return 0;
    } else if (operator1 == OPERATOR_PARENTHESIS_RIGHT) {
        /* Magic right parenthesis */
        return 1;
    } else {
        return OPERATOR_PRECEDENCE[operator1]
                - OPERATOR_PRECEDENCE[operator2];
    }
}

bool replaceExpressionStart(string *expression, string old,
        string new) {
    if (string_startsWith(*expression, old)) {
        string_subsititute(expression,
                string_replaceFirst(*expression, old, new));
        return true;
    } else {
        return false;
    }
}

bool replaceExpressionRecursive(string *expression, string old,
        string new) {
    string theExpression = string_replaceRecursiveReturnsNull(
            *expression, old, new);
    if (theExpression == null) {
        return false;
    } else {
        Memory_free(*expression);
        *expression = theExpression;
        return true;
    }
}

void normalizeExpression(string *expression) {

    while (replaceExpressionRecursive(expression, "++", "+")
            || replaceExpressionRecursive(expression, "+-", "-")
            || replaceExpressionRecursive(expression, "-+", "-")
            || replaceExpressionRecursive(expression, "--", "+"));

    replaceExpressionStart(expression, "+", "");

    replaceExpressionStart(expression, "-", "0-");

    replaceExpressionRecursive(expression, "(+", "(");

    replaceExpressionRecursive(expression, "(-", "(0-");
}

bool readOperator(string start, Operator *operator,
        string *operatorStart, string *operatorEnd) {
    size_t i, position, oldPosition = string_length(start);
    bool found = false;
    for (i = 0; i < ARRAY_SIZE(OPERATOR_STRINGS); ++i) {
        if ((position = string_indexOfIgnoreCase(start,
                    OPERATOR_STRINGS[i])) != -1
                && position < oldPosition) {
            found = true;
            oldPosition = position;
            *operator = i;
            *operatorStart = start + position;
            *operatorEnd = *operatorStart
                    + string_length(OPERATOR_STRINGS[i]);
        }
    }
    if (found) {
        return true;
    } else {
        return false;
    }
}

bool readOperand(string start, string end, Operand *operand) {
    size_t operandLength = end - start;
    /*
     * @note Use a new string so that string_parseDouble will nor read
     *       more than it should.
     */
    string operandString = string_subString(start, 0, operandLength);
    Operand theOperand;
    if (string_parseDouble(operandString, &theOperand)
            == operandLength) {
        Memory_free(operandString);
        *operand = theOperand;
        return true;
    } else {
        Memory_free(operandString);
        return false;
    }
}

void pushOperator(Operator operator, Stack *operatorStack) {
    Operator *theOperator = Memory_allocateType(Operator);
    *theOperator = operator;
    $(operatorStack, push, theOperator);
}

void pushOperand(Operand operand, Stack *operandStack) {
    Operand *theOperand = Memory_allocateType(Operand);
    *theOperand = operand;
    $(operandStack, push, theOperand);
}

EvaluationResult evaluteOperator(Operator operator,
        Stack *operatorStack, Stack *operandStack) {

    Operator *operator1;
    Operand *operand1, *operand2;
    EvaluationResult result;

    switch (operator) {
    case OPERATOR_ADDITION:
        operand2 = $(operandStack, pop);
        operand1 = $(operandStack, pop);
        if (operand1 == null || operand2 == null) {
            return EVALUATION_ERROR_MALFORMED_EXPRESSION;
        }
        pushOperand(*operand1 + *operand2, operandStack);
        Memory_free(operand1);
        Memory_free(operand2);
        break;
    case OPERATOR_SUBTRACTION:
        operand2 = $(operandStack, pop);
        operand1 = $(operandStack, pop);
        if (operand1 == null || operand2 == null) {
            return EVALUATION_ERROR_MALFORMED_EXPRESSION;
        }
        pushOperand(*operand1 - *operand2, operandStack);
        Memory_free(operand1);
        Memory_free(operand2);
        break;
    case OPERATOR_MULPLICATION:
        operand2 = $(operandStack, pop);
        operand1 = $(operandStack, pop);
        if (operand1 == null || operand2 == null) {
            return EVALUATION_ERROR_MALFORMED_EXPRESSION;
        }
        pushOperand(*operand1 * *operand2, operandStack);
        Memory_free(operand1);
        Memory_free(operand2);
        break;
    case OPERATOR_DIVISION:
        operand2 = $(operandStack, pop);
        operand1 = $(operandStack, pop);
        if (operand1 == null || operand2 == null) {
            return EVALUATION_ERROR_MALFORMED_EXPRESSION;
        }
        pushOperand(*operand1 / *operand2, operandStack);
        Memory_free(operand1);
        Memory_free(operand2);
        break;
    case OPERATOR_POWER:
    case OPERATOR_POW:
        operand2 = $(operandStack, pop);
        operand1 = $(operandStack, pop);
        if (operand1 == null || operand2 == null) {
            return EVALUATION_ERROR_MALFORMED_EXPRESSION;
        }
        pushOperand(pow(*operand1, *operand2), operandStack);
        Memory_free(operand1);
        Memory_free(operand2);
        break;
    case OPERATOR_PARENTHESIS_LEFT:
        break;
    case OPERATOR_FACTORIAL:
        operand1 = $(operandStack, pop);
        if (operand1 == null) {
            return EVALUATION_ERROR_MALFORMED_EXPRESSION;
        }
        if (*operand1 < 0 || *operand1 != (unsigned int)*operand1) {
            Memory_free(operand1);
            return EVALUATION_ERROR_INVALID_OPERATION;
        }
        pushOperand(factorial((unsigned int)*operand1), operandStack);
        Memory_free(operand1);
        break;
    case OPERATOR_SIN:
        operand1 = $(operandStack, pop);
        if (operand1 == null) {
            return EVALUATION_ERROR_MALFORMED_EXPRESSION;
        }
        pushOperand(sin(*operand1), operandStack);
        Memory_free(operand1);
        break;
    case OPERATOR_COS:
        operand1 = $(operandStack, pop);
        if (operand1 == null) {
            return EVALUATION_ERROR_MALFORMED_EXPRESSION;
        }
        pushOperand(cos(*operand1), operandStack);
        Memory_free(operand1);
        break;
    case OPERATOR_TAN:
        operand1 = $(operandStack, pop);
        if (operand1 == null) {
            return EVALUATION_ERROR_MALFORMED_EXPRESSION;
        }
        pushOperand(tan(*operand1), operandStack);
        Memory_free(operand1);
        break;
    case OPERATOR_LOG:
        operand2 = $(operandStack, pop);
        operand1 = $(operandStack, pop);
        if (operand1 == null || operand2 == null) {
            return EVALUATION_ERROR_MALFORMED_EXPRESSION;
        }
        if (*operand1 <= 0 || *operand1 == 1 || *operand2 <= 0) {
            Memory_free(operand1);
            Memory_free(operand2);
            return EVALUATION_ERROR_INVALID_OPERATION;
        }
        pushOperand(log(*operand2) / log(*operand1), operandStack);
        Memory_free(operand1);
        Memory_free(operand2);
        break;
    case OPERATOR_COMMA:
        /*
         * Should have been handle by the evaluation of
         * OPERATOR_PARENTHESIS_RIGHT.
         */
        return EVALUATION_ERROR_COMMA_NOT_IN_FUNCTION;
        break;
    case OPERATOR_PARENTHESIS_RIGHT:
        while (*(operator1 = $(operatorStack, pop))
                == OPERATOR_COMMA) {
            Memory_free(operator1);
        }
        if (OPERATOR_PRECEDENCE[*operator1]
                == OPERATOR_PRECEDENCE[OPERATOR_PARENTHESIS_LEFT]) {
            result = evaluteOperator(*operator1, operatorStack,
                    operandStack);
            Memory_free(operator1);
            if (result != EVALUATION_SUCCESS) {
                return result;
            }
        } else {
            Memory_free(operator1);
            return EVALUATION_ERROR_UNPAIRED_PARENTHESIS;
        }
        break;
    default:
        return EVALUATION_ERROR_INTERNAL_FAILURE;
    }

    return EVALUATION_SUCCESS;
}

EvaluationResult processOperator(Operator operator,
        Stack *operatorStack, Stack *operandStack) {

    Operator *topOperator;
    EvaluationResult result;

    while (_(operatorStack, size) != 0
            && Operator_comparePrecedence(
                    *(Operator *)$(operatorStack, peek),
                    operator) > 0) {
        topOperator = $(operatorStack, pop);
        result = evaluteOperator(*topOperator, operatorStack,
                operandStack);
        Memory_free(topOperator);
        if (result != EVALUATION_SUCCESS) {
            return result;
        }
    }

    pushOperator(operator, operatorStack);

    return EVALUATION_SUCCESS;
}

EvaluationResult doFinal(Stack *operatorStack, Stack *operandStack) {

    Operator *operator;
    EvaluationResult result;

    while (_(operatorStack, size) != 0) {
        operator = $(operatorStack, pop);
        result = evaluteOperator(*operator, operatorStack,
                operandStack);
        Memory_free(operator);
        if (result != EVALUATION_SUCCESS) {
            return result;
        }
    }

    if (_(operandStack, size) == 1) {
        return EVALUATION_SUCCESS;
    } else {
        return EVALUATION_ERROR_FINALIZATION_FAILED;
    }
}

void cleanUp(string expression, Stack *operatorStack,
        Stack *operandStack) {
    Memory_free(expression);
    $(operatorStack, delete);
    $(operandStack, delete);
}

EvaluationResult evaluateExpression(string expression,
        Operand *value) {

    string start, end, operatorStart, operatorEnd;
    Stack *operatorStack = Stack_new(),
            *operandStack = Stack_new();
    Operator operator;
    Operand operand;
    EvaluationResult result;

    expression = string_replaceRecursive(expression, " ", "");
    normalizeExpression(&expression);

    start = expression;
    end = expression + string_length(expression);

    while (true) {
        if (readOperator(start, &operator, &operatorStart,
                &operatorEnd)) {
            if (operatorStart != start) {
                readOperand(start, operatorStart, &operand);
                pushOperand(operand, operandStack);
            }
            start = operatorEnd;
            result = processOperator(operator, operatorStack,
                    operandStack);
            if (result != EVALUATION_SUCCESS) {
                cleanUp(expression, operatorStack, operandStack);
                return result;
            }
        } else {
            /* No more operator now. */
            if (readOperand(start, end, &operand)) {
                pushOperand(operand, operandStack);
            } else if (start != end) {
                cleanUp(expression, operatorStack, operandStack);
                return EVALUATION_ERROR_PARSING_FAILED;
            }
            break;
        }
    }

    result = doFinal(operatorStack, operandStack);
    if (result == EVALUATION_SUCCESS) {
        *value = *(Operand *)$(operandStack, peek);
    }

    cleanUp(expression, operatorStack, operandStack);
    return result;
}
