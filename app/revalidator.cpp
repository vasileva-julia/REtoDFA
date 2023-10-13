#include "revalidator.h"

QValidator::State ReValidator::validate(QString& input, int&) const {
    int balance = 0;
    for (char symbol : input.toStdString()) {
        switch (symbol) {
        case '(':
            ++balance;
            break;
        case ')':
            --balance;
            if (balance < 0)
                return QValidator::Intermediate;
            break;
        case '|':
        case '.':
        case '*':
        case '+':
            break;
        default:
            if (!std::isalnum(symbol) && !std::isblank(symbol))
                return QValidator::Intermediate;
            break;
        }
    }
    if (balance == 0)
        return QValidator::Acceptable;
    else
        return QValidator::Intermediate;
}
