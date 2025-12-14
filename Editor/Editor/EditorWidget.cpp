#include "stdafx.h"
#include "EditorWIdget.h"


EditorWidget::EditorWidget()
	:refCount_(1)
{
}

EditorWidget::~EditorWidget()
{
	CleanUp();
}


void EditorWidget::CleanUp()
{

}