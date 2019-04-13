/**
 * Override the QLabel.
 * @author:		Sny
 * @version:	1.0
 * @since:		2019-01-15
 */


#pragma once


#include <QLabel>
#include <QPaintEvent>


class MSLabel : public QLabel
{
	Q_OBJECT

public:
	MSLabel(QWidget *parent = 0);
	~MSLabel();
	void showCross(bool);

protected:
	void paintEvent(QPaintEvent *pe);

private:
	bool showFlag;
};

