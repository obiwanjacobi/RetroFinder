#include "FloatingPanel.h"

FloatingPanel::FloatingPanel(Clay_ElementId id) : Panel(id) {}

void FloatingPanel::Prepare(Theme* theme, BoxStyle& style)
{
	Panel::Prepare(theme, style);
	style.floating = _floating;
}
