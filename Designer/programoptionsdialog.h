#ifndef PROGRAMOPTIONSDIALOG_H
#define PROGRAMOPTIONSDIALOG_H

#include <QDialog>

class QAbstractButton;
class QString;

namespace Ui {
class ProgramOptionsDialog;
}

class ProgramOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgramOptionsDialog(QWidget *parent = 0);
    ~ProgramOptionsDialog();
    ProgramOptionsDialog(int solutions, int loops, unsigned recursion,
                         bool strategyBF, bool backtracking, unsigned maxPathLength, bool executeBack,
                         bool oneArrow, QString* mainNetName, QStringList* nets, QWidget *parent = 0);
    void getResults(int& solutions, int& loops, unsigned& recursion,
                    bool& strategyBF, bool& backtracking, unsigned& maxPathLength, bool& executeBack,
                    bool& oneArrow, QString*& mainNetName);

private:
    Ui::ProgramOptionsDialog *ui;

    int mSolutions; // -1 ask after all 0 all
    int mLoops; // -1 No 0 Any
    unsigned mRrecursion; // 0 Any
    bool mStrategyBF; //true - bf false - df
    bool mBacktracking;
    unsigned mMaxPathLength; // 0 - Any
    bool mExecuteBack;
    bool mOneArrow;
    QString* mMainNetName;

private slots:
    void handleExit(QAbstractButton * button);
};

#endif // PROGRAMOPTIONSDIALOG_H
