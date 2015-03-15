#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>

class RenderArea
{
public:
  enum Element {ArrowRegular, ArrowDotted, NodeBegin, NodeEnd, NodeRegular};
  RenderArea(QWidget* parent = 0);
  ~RenderArea();
};

#endif // RENDERAREA_H
