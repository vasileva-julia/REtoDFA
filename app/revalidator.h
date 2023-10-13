#ifndef REVALIDATOR_H
#define REVALIDATOR_H

#include <QValidator>

class ReValidator : public QValidator { // Regular expression validator.
public:
    ReValidator(QObject* parent) : QValidator(parent) {}
    QValidator::State validate(QString& input, int& pos) const override;
};

#endif // REVALIDATOR_H
