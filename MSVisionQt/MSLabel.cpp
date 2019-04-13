#include "stdafx.h"
#include "MSLabel.h"


MSLabel::MSLabel(QWidget *parent)
	: QLabel(parent),
	showFlag(false)
{

}

MSLabel::~MSLabel()
{
}

void MSLabel::showCross(bool flag)
{
	showFlag = flag;
}

void MSLabel::paintEvent(QPaintEvent *pe)
{
	Q_UNUSED(pe);
	QLabel::paintEvent(pe);
	QPainter painter(this);
	painter.setPen(QColor(0, 192, 0));
	if (showFlag)
	{
		int w = this->width();
		int h = this->height();
		painter.drawLine(QPointF(0, h / 2), QPointF(w - 1, h / 2));
		painter.drawLine(QPointF(w / 2, 0), QPointF(w / 2, h - 1));
	}
}
