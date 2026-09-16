#pragma once
#include <QDockWidget>
#include <QMainWindow>
#include <QVBoxLayout>

#include "title_bar.h"

namespace hn::editor
{
class EditorMain : public QMainWindow
{
public:
	EditorMain();

	bool ShouldClose() const;

	QTabWidget* GetTabWidget() const;

private:

	void createScene1()
	{
		auto *scene = new QMainWindow();

		auto *content = new QWidget();
		auto *layout = new QVBoxLayout(content);

		auto *label = new QLabel("Bienvenue dans Horizon-Next !");
		auto *button = new QPushButton("Clique-moi");

		layout->addWidget(label);
		layout->addWidget(button);

		scene->setCentralWidget(content);

		// =========================
		// Inspector
		// =========================

		auto *inspector = new QDockWidget("Inspector", scene);

		inspector->setMinimumWidth(400);
		auto* inspectorWidget = new QWidget();

		auto* inspectorlayout = new QVBoxLayout(inspectorWidget);
		inspectorlayout->setAlignment(Qt::AlignTop);

		inspectorlayout->addWidget(new QPushButton("Bouton 1"));
		inspectorlayout->addWidget(new QPushButton("Bouton 2"));
		inspectorlayout->addWidget(new QLabel("Texte"));

		inspectorlayout->addStretch();

		inspector->setWidget(inspectorWidget);

		scene->addDockWidget(
			Qt::RightDockWidgetArea,
			inspector
		);

		QObject::connect(button, &QPushButton::clicked, [label]()
		{
			label->setText("Le bouton a été cliqué !");
		});

		tabs_->addTab(scene, "Scene 1");
	}

protected:
	void closeEvent(QCloseEvent *event) override;


private:
	bool should_close_ = false;

	QTabWidget *tabs_ = nullptr;



	//handle window resize
	enum class ResizeDirection
	{
		None,
		Left,
		Right,
		Top,
		Bottom,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};

	ResizeDirection resizeDirection_ = ResizeDirection::None;
	bool resizing_ = false;

	QPoint resizeStartPosition_;
	QRect resizeStartGeometry_;

	int resizeBorder_ = 6;
};
}
