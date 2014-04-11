/**
 * @file Calculator.c
 * @author: Zhang Hai
 */

#include "zhclib/Common.h"
#include "zhclib/Console.h"

#include "Evaluator.h"


string EVALUATION_RESULTS[] = {
    "EVALUATION_SUCCESS",
    "EVALUATION_ERROR_MALFORMED_EXPRESSION",
    "EVALUATION_ERROR_COMMA_NOT_IN_FUNCTION",
    "EVALUATION_ERROR_UNPAIRED_PARENTHESIS",
    "EVALUATION_ERROR_PARSING_FAILED",
    "EVALUATION_ERROR_FINALIZATION_FAILED",
    "EVALUATION_ERROR_INVALID_OPERATION",
    "EVALUATION_ERROR_INTERNAL_FAILURE"
};


int main() {

    string line;
    double value;
    EvaluationResult result;

    while (!string_isEmpty(line = Console_readLine("> "))) {
        result = evaluateExpression(line, &value);
        Memory_free(line);
        if (result == EVALUATION_SUCCESS) {
            Console_printLine("%.10g", value);
        } else {
            Console_printErrorLine("Error %d: %s", result,
                    EVALUATION_RESULTS[result]);
        }
    }
    Memory_free(line);

    return 0;
}
