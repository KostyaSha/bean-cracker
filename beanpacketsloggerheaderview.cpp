#include "beanpacketsloggerheaderview.h"

#include <QHeaderView>

bool isChecked = false;

BeanPacketsLoggerHeaderView::BeanPacketsLoggerHeaderView(Qt::Orientation orientation, QWidget * parent)
    : QHeaderView(orientation, parent)
{
}

BeanPacketsLoggerHeaderView::~BeanPacketsLoggerHeaderView() = default;

void BeanPacketsLoggerHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
  painter->save();
  QHeaderView::paintSection(painter, rect, logicalIndex);
  painter->restore();
  if (logicalIndex == 0) {
    QStyleOptionButton option;
    option.rect = QRect(1,3,20,20);
    if (isChecked) {
      option.state = QStyle::State_On;
    }  else {
      option.state = QStyle::State_Off;
    }
    this->style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &option, painter);
  }

}

void BeanPacketsLoggerHeaderView::mousePressEvent(QMouseEvent *event)
{
    isChecked = !isChecked;
    this->update();
    QHeaderView::mousePressEvent(event);
    redrawCheckBox();
}

void BeanPacketsLoggerHeaderView::redrawCheckBox()
{
    viewport()->update();
}
