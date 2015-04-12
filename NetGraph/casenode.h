#ifndef CASENODE_H
#define CASENODE_H

#include <string>
#include <vector>

using namespace std;

#include "node.h"

namespace NetGraph
{
class CaseNode : public Node
{
public:
    CaseNode(string *expression);
    CaseNode(string& expression);
    CaseNode(string&& expression);
    ~CaseNode();
private:
    string* _expression;
    vector<int> arrowConstants;
};
}

#endif // CASENODE_H
