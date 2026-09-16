#pragma once

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QFileDialog>
#include <QMouseEvent>

#include <iostream>

namespace hn::editor
{
class TitleBar : public QWidget
{
public:
  explicit TitleBar(QWidget *parent = nullptr)
    : QWidget(parent)
  {
    setFixedHeight(35);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 0, 0, 0);
    layout->setSpacing(2);

  	auto *image = new QLabel();

  	QPixmap pixmap("Editor/logo.png");
  	image->setPixmap(pixmap);
  	image->setFixedSize(35,35);
  	image->setScaledContents(true);

  	layout->addWidget(image);

    auto *title = new QLabel("Horizon Editor");

    // =========================
    // File
    // =========================

    auto *fileButton = new QPushButton("File");
    fileButton->setFlat(true);

    auto *fileMenu = new QMenu(this);

    fileMenu->addAction("New");

    auto *openAction = fileMenu->addAction("Open");

    QObject::connect(openAction, &QAction::triggered, [this]()
    {
      QString file = QFileDialog::getOpenFileName(
        window(),
        "Open File",
        "",
        "All Files (*.*)"
      );

      if (!file.isEmpty())
      {
        std::cout << file.toStdString() << std::endl;
      }
    });

    fileMenu->addAction("Save");

    fileMenu->addSeparator();

    auto *exitAction = fileMenu->addAction("Exit");

    QObject::connect(exitAction, &QAction::triggered, [this]()
    {
      window()->close();
    });

    fileButton->setMenu(fileMenu);

    // =========================
    // Edit
    // =========================

    auto *editButton = new QPushButton("Edit");
    editButton->setFlat(true);

    auto *editMenu = new QMenu(this);

    editMenu->addAction("Undo");
    editMenu->addAction("Redo");

    editMenu->addSeparator();

    editMenu->addAction("Copy");
    editMenu->addAction("Paste");

    editButton->setMenu(editMenu);

    // =========================
    // Window buttons
    // =========================

    auto *minimize = new QPushButton();
    auto *maximize = new QPushButton();
    auto *close = new QPushButton();

  	minimize->setIcon(QIcon("Editor/Icons/minimize.png"));
  	maximize->setIcon(QIcon("Editor/Icons/maximize.png"));
  	close->setIcon(QIcon("Editor/Icons/close.png"));

  	minimize->setIconSize(QSize(14, 14));
  	maximize->setIconSize(QSize(14, 14));
  	close->setIconSize(QSize(14, 14));

  	minimize->setFixedSize(35, 30);
  	maximize->setFixedSize(35, 30);
  	close->setFixedSize(35, 30);


    layout->addWidget(title);

    layout->addWidget(fileButton);
    layout->addWidget(editButton);

    layout->addStretch();

    layout->addWidget(minimize);
    layout->addWidget(maximize);
    layout->addWidget(close);

    // =========================
    // Buttons
    // =========================

    QObject::connect(close, &QPushButton::clicked, [this]()
    {
      window()->close();
    });

    QObject::connect(minimize, &QPushButton::clicked, [this]()
    {
      window()->showMinimized();
    });

    QObject::connect(maximize, &QPushButton::clicked, [this]()
    {
      toggleMaximized();
    });
  }

protected:

  void mouseDoubleClickEvent(QMouseEvent *event) override
  {
    if (event->button() == Qt::LeftButton)
    {
      toggleMaximized();
    }

    QWidget::mouseDoubleClickEvent(event);
  }

	void mousePressEvent(QMouseEvent *event) override
  {
  	if (event->button() == Qt::LeftButton)
  	{
  		dragging_ = true;

  		dragStartPosition_ =
				event->globalPosition().toPoint();

  		dragStartOffsetY_ =
				event->position().toPoint().y();

  		dragPosition_ =
				event->globalPosition().toPoint() -
				window()->frameGeometry().topLeft();
  	}

  	QWidget::mousePressEvent(event);
  }

	void mouseMoveEvent(QMouseEvent *event) override
  {
  	if (!dragging_)
  	{
  		QWidget::mouseMoveEvent(event);
  		return;
  	}

  	QPoint currentPosition = event->globalPosition().toPoint();

  	// Si la fenêtre est maximisée et qu'on commence à la déplacer
  	if (window()->isMaximized())
  	{
  		// Pourcentage horizontal de la souris dans la fenêtre
  		double xRatio =
				static_cast<double>(dragStartPosition_.x()) /
				window()->width();

  		// On démaximise
  		window()->showNormal();

  		// Nouvelle position pour garder la souris
  		// approximativement au même endroit dans la title bar
  		int x =
				currentPosition.x() -
				static_cast<int>(window()->width() * xRatio);

  		int y =
				currentPosition.y() -
				dragStartOffsetY_;

  		window()->move(x, y);

  		// On ne veut plus utiliser l'ancien dragPosition_
  		dragPosition_ =
				currentPosition -
				window()->frameGeometry().topLeft();

  		dragStartPosition_ = currentPosition;
  	}
  	else
  	{
  		window()->move(
				currentPosition - dragPosition_
			);
  	}

  	QWidget::mouseMoveEvent(event);
  }

	void mouseReleaseEvent(QMouseEvent *event) override
  {
  	if (event->button() == Qt::LeftButton)
  		dragging_ = false;

  	QWidget::mouseReleaseEvent(event);
  }

private:

  void toggleMaximized()
  {
    if (window()->isMaximized())
      window()->showNormal();
    else
      window()->showMaximized();
  }

private:
  bool dragging_ = false;
  QPoint dragPosition_;
	QPoint dragStartPosition_;
	int dragStartOffsetY_ = 0;
};
}