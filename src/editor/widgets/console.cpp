#include "console.h"

#include <QPlainTextEdit>

#include "editor/common.h"
#include "editor/ui/application.h"

namespace hn::editor
{
	ConsoleWidget::ConsoleWidget(QWidget *parent)
				: QDockWidget("Console", parent), console_widget_(new QPlainTextEdit())
	{
		setMinimumWidth(400);
		setMinimumHeight(400);

		console_widget_->setPlaceholderText("Enter console commands...");

		console_widget_->setReadOnly(true);

		console_widget_->setStyleSheet(
				"QPlainTextEdit {"
				"    background-color: #1e1e1e;"
				"    color: #d4d4d4;"
				"    border: none;"
				"    font-family: Consolas;"
				"    font-size: 16px;"
				"}"
		);

		console_widget_->setPlainText(GetApplication()->GetFullMessageConsole().c_str());

		// Le texte prend toute la place du dock
		setWidget(console_widget_);

		GetApplication()->ED_MessageReceived.Subscribe([this]()
		{
			console_widget_->setPlainText(GetApplication()->GetFullMessageConsole().c_str());
		});
	}

	void ConsoleWidget::closeEvent(QCloseEvent *event)
	{
	}
}
