#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QBoxLayout>

#include "../NetGraph/netgraph.h"

class Editor : public QWidget
{
  Q_OBJECT

private:
    QOpenGLWidget* canvas;
public:
  explicit Editor(QWidget *parent = 0);
  ~Editor();

signals:

public slots:
};

#endif // EDITOR_H
