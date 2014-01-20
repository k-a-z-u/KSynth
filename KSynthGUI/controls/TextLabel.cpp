#include "TextLabel.h"

TextLabel::TextLabel(QWidget *parent) :
	QWidget(parent) {
	setText("Rofl Kartoffel miau");
	setFocusPolicy(Qt::ClickFocus);
}

TextLabel::~TextLabel() {
	;
}


void TextLabel::focusInEvent(QFocusEvent *e) {
	Q_UNUSED(e);
	state.focused = true;
	emit repaint();
}

void TextLabel::focusOutEvent(QFocusEvent *e) {
	Q_UNUSED(e);
	state.focused = false;
	emit repaint();
}


void TextLabel::setText(const std::string& text) {
	bool changed = (this->text != text);
	this->text = text;
	if (changed) {emit repaint();}
}

const std::string& TextLabel::getText() const {
	return this->text;
}

#include <QKeyEvent>
void TextLabel::keyPressEvent(QKeyEvent *e) {

	int c = (char) e->key();
	//std::string x = e->text().toStdString();

	if (e->key() == Qt::Key_Backspace) {
		if (text.empty()) {return;}
		text = text.substr(0, text.length() - 1);
	} else if ( c >= (unsigned char) 32 && c <= (unsigned char) 127 ) {
		text += e->text().toUtf8().constData();
	} else {
		return;
	}

	emit repaint();
	emit valueChanged();

}

#include <QPainter>
void TextLabel::paintEvent (QPaintEvent* event) {

	Q_UNUSED(event);
	QPainter p(this);

	QFont font;	font.setPixelSize(11); font.setBold(true);
	QFontMetrics fm(font);
	p.setFont(font);

	p.setBrush(QColor(255,255,255));
	//p.drawRect(0,0,width(),height());

	p.setPen(QColor(0,0,0));

	// get text width
	int w = fm.width(text.c_str());
	int h = fm.height();

	int x = width() / 2 + h / 3;
	int y = height() / 2 + w / 2;

	p.translate(x,y);
	p.rotate(270);
	p.drawText(0,0, text.c_str());

	if (state.focused) {
		p.drawLine(w+1,-12, w+1,2);
	}

}

void TextLabel::mouseReleaseEvent (QMouseEvent* e) {
	Q_UNUSED(e);
}
