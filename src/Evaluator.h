/**
 * @file Evaluator.h
 * @author: Zhang Hai
 */

#ifndef _EVALUATOR_H_
#define _EVALUATOR_H_


#include "zhclib/Common.h"


typedef enum {
    EVALUATION_SUCCESS,
    EVALUATION_ERROR_MALFORMED_EXPRESSION,
    EVALUATION_ERROR_COMMA_NOT_IN_FUNCTION,
    EVALUATION_ERROR_UNPAIRED_PARENTHESIS,
    EVALUATION_ERROR_PARSING_FAILED,
    EVALUATION_ERROR_FINALIZATION_FAILED,
    EVALUATION_ERROR_INVALID_OPERATION,
    EVALUATION_ERROR_INTERNAL_FAILURE
} EvaluationResult;

typedef double Operand;


EvaluationResult evaluateExpression(string expression,
        Operand *value);


#endif /* _EVALUATOR_H_ */
