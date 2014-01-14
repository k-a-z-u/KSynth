#include "EditorSlider.h"

#include "../../model/Context.h"
#include <KSynth/Sequencer.h>
#include "Editor.h"

EditorSlider::EditorSlider(Editor& editor, Context& ctx, QWidget *parent) :
	QWidget(parent), editor(editor), ctx(ctx) {

	setMinimumWidth(7);
	setMaximumWidth(7);
	setMinimumHeight(100);

	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
	setVisible(true);

}

EditorSlider::~EditorSlider() {
	;
}



#include <QPainter>
void EditorSlider::paintEvent(QPaintEvent* e) {

	Q_UNUSED(e);
	QPainter p(this);
	p.setPen(QColor(0,0,0));
	p.setBrush(QColor(0,0,0));

	p.drawRect(0, 0, width(), width());
	p.drawLine(3, 0, 3, height());

}
