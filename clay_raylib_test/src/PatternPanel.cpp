#include "PatternPanel.h"

PatternPanel::PatternPanel(Clay_ElementId id) :
	ContainerControl(id),
	_pattern(CLAY_ID("patternpanel_pattern"))
{}

void PatternPanel::Prepare(Theme* theme, BoxStyle& style)
{
	ContainerControl::Prepare(theme, style);

	Clay_ElementData panelData = Clay_GetElementData(Id());
	float width = panelData.found ? panelData.boundingBox.width : 0.0f;
	float height = panelData.found ? panelData.boundingBox.height : 0.0f;

	_pattern.SetRenderSize(width, height);
	_pattern.ApplyToStyle(theme, style);
}
