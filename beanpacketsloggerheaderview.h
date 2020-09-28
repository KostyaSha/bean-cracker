#ifndef BEANPACKETSLOGGERHEADERVIEW_H
#define BEANPACKETSLOGGERHEADERVIEW_H

#include <QHeaderView>
#include <QPainter>

class BeanPacketsLoggerHeaderView : public QHeaderView
{
    Q_OBJECT

public:
    explicit BeanPacketsLoggerHeaderView(Qt::Orientation orientation, QWidget * parent = nullptr);
    ~BeanPacketsLoggerHeaderView() override;

signals:
    void checkBoxClicked(bool state);

protected:
  void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
  void mousePressEvent(QMouseEvent *event) override;

private:
  bool isChecked{};
  void redrawCheckBox();
};

#endif // BEANPACKETSLOGGERHEADERVIEW_H
