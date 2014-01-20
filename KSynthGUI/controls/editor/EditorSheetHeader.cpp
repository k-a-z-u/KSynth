#include "EditorSheetHeader.h"
#include "EditorSheet.h"
#include <QImage>
#include <QPainter>

#include "../../misc/Skin.h"

EditorSheetHeader::EditorSheetHeader(EditorSheet& sheet, QWidget* parent) :
	QWidget(parent), sheet(sheet) {
	;
}

#include <iostream>
QSize EditorSheetHeader::sizeHint() const {
	QSize size(32, sheet.sizeHint().height());
	std::cout << "LOL: "<< size.width() << ":" << size.height() << std::endl;
	return size;
}

void EditorSheetHeader::paintEvent(QPaintEvent*) {
	// FIXME horrible! better solution???
//	blockSignals(true);
//	move(-parent.parentWidget()->x(), y());
//	blockSignals(false);
	static QImage imgBg = Skin::getImage("skin/editorClav.png", "PNG");
	QPainter p(this);
	p.drawImage(0,0, imgBg);
}
