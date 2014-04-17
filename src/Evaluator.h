/**
 * @file Evaluator.h
 * @author: Zhang Hai
 */

/*
 * Copyright (C) 2014 Zhang Hai
 *
 * This file is part of calc.
 *
 * calc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * calc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with calc.  If not, see <http://www.gnu.org/licenses/>.
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
