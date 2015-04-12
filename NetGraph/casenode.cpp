#include "casenode.h"

namespace NetGraph
{
CaseNode::CaseNode(string *expression)
{
    _expression = expression;
}

CaseNode::CaseNode(string &expression) : CaseNode(new string(expression)) {}

CaseNode::CaseNode(string &&expression) : CaseNode(new string(expression)) {}

CaseNode::~CaseNode()
{

}


}
