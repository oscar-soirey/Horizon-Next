#include "home_window.h"

#include <QMouseEvent>
#include <QFileDialog>
#include <QPixmap>
#include <QFrame>

#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <fstream>

#include "editor/window/main_window.h"
#include "../../../tools/utils.h"

namespace hn::editor
{
  namespace
  {
    QPixmap LoadThumbnail(const QString& path, int size)
    {
      const QString actualPath = path.isEmpty() ? QString(kPlaceholderImagePath) : path;
      QPixmap pixmap(actualPath);
      if (pixmap.isNull())
        pixmap = QPixmap(kPlaceholderImagePath);
      return pixmap.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    QDateTime FileTimeToQDateTime(const std::filesystem::file_time_type& ftime)
    {
      // std::filesystem::file_time_type n'est pas directement convertible en
      // system_clock avant C++20 (clock_cast). Ce hack marche en C++17 et +.
      const auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
      const std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);
      return QDateTime::fromSecsSinceEpoch(static_cast<qint64>(cftime));
    }

    // Lit le contenu entier d'un fichier. Retourne une chaîne vide (et logue
    // une erreur) si le fichier n'existe pas ou n'est pas lisible.
    std::string GetFileContent(const std::filesystem::path& path)
    {
      std::ifstream file(path, std::ios::in | std::ios::binary);
      if (!file)
      {
        std::cout << "Failed to open file: " << path.string() << std::endl;
        return {};
      }

      std::ostringstream buffer;
      buffer << file.rdbuf();
      return buffer.str();
    }
  }

  // -----------------------------------------------------------------------
  // RecentProjectItemWidget
  // -----------------------------------------------------------------------
  RecentProjectItemWidget::RecentProjectItemWidget(RecentProjectInfo info, QWidget* parent)
    : QWidget(parent)
    , m_info(std::move(info))
  {
    auto* rowLayout = new QHBoxLayout(this);

    m_thumbnailLabel = new QLabel(this);
    m_thumbnailLabel->setFixedSize(48, 48);
    m_thumbnailLabel->setPixmap(LoadThumbnail(m_info.thumbnailPath, 48));

    auto* textLayout = new QVBoxLayout();
    m_nameLabel = new QLabel(m_info.name, this);
    m_lastEditLabel = new QLabel(m_info.lastEdit.toString("yyyy-MM-dd HH:mm"), this);
    textLayout->addWidget(m_nameLabel);
    textLayout->addWidget(m_lastEditLabel);

    rowLayout->addWidget(m_thumbnailLabel);
    rowLayout->addLayout(textLayout);
    rowLayout->addStretch();

    setCursor(Qt::PointingHandCursor);
  }

  void RecentProjectItemWidget::mousePressEvent(QMouseEvent* event)
  {
    if (event->button() == Qt::LeftButton)
      emit Clicked(m_info.path);
    QWidget::mousePressEvent(event);
  }

  // -----------------------------------------------------------------------
  // TemplateItemWidget
  // -----------------------------------------------------------------------
  TemplateItemWidget::TemplateItemWidget(TemplateInfo info, QWidget* parent)
    : QWidget(parent)
    , m_info(std::move(info))
  {
    auto* colLayout = new QVBoxLayout(this);

    m_thumbnailLabel = new QLabel(this);
    m_thumbnailLabel->setFixedSize(96, 96);
    m_thumbnailLabel->setFrameShape(QFrame::Box);
    m_thumbnailLabel->setAlignment(Qt::AlignCenter);
    m_thumbnailLabel->setPixmap(LoadThumbnail(m_info.thumbnailPath, 96));

    m_nameLabel = new QLabel(m_info.displayName, this);
    m_nameLabel->setAlignment(Qt::AlignHCenter);

    colLayout->addWidget(m_thumbnailLabel);
    colLayout->addWidget(m_nameLabel);

    setCursor(Qt::PointingHandCursor);
    UpdateSelectionStyle();
  }

  void TemplateItemWidget::SetSelected(bool selected)
  {
    if (m_selected == selected)
      return;
    m_selected = selected;
    UpdateSelectionStyle();
  }

  void TemplateItemWidget::UpdateSelectionStyle()
  {
    m_thumbnailLabel->setStyleSheet(m_selected
      ? "border: 2px solid palette(highlight);"
      : "border: 1px solid palette(mid);");
  }

  void TemplateItemWidget::mousePressEvent(QMouseEvent* event)
  {
    if (event->button() == Qt::LeftButton)
      emit Clicked(m_info.id);
    QWidget::mousePressEvent(event);
  }

  // -----------------------------------------------------------------------
  // HomeWindow
  // -----------------------------------------------------------------------
  HomeWindow::HomeWindow(): EditorWindow("Home")
  {
    resize(800, 700);

    //setContentsMargins(8,8,8,8);

    std::vector<TemplateInfo> templates;
    templates.push_back({"blank", "Blank", "Editor/Icons/project.png"});
    templates.push_back({"third-person", "Third-Person", "Editor/Icons/third-person.png"});
    templates.push_back({"first-person", "First-Person", "Editor/Icons/first-person.png"});


    std::vector<RecentProjectInfo> recent_projects;
    std::string r_p_content = GetFileContent("cache-editor/recent-projects.json");

    //Parser les projets recents
    try
    {
      const nlohmann::json j = nlohmann::json::parse(r_p_content);

      for (const auto& pathValue : j.at("recent-projects"))
      {
        const std::filesystem::path projectPath = pathValue.get<std::string>();

        RecentProjectInfo info;
        info.path = QString::fromStdString(projectPath.string());
        info.name = QString::fromStdString(projectPath.filename().string());

        std::error_code ec;
        const auto ftime = std::filesystem::last_write_time(projectPath, ec);
        info.lastEdit = ec ? QDateTime() : FileTimeToQDateTime(ftime);

        info.thumbnailPath = "Editor/Icons/project.png";

        recent_projects.push_back(std::move(info));
      }
    }
    catch (const nlohmann::json::exception& e)
    {
      std::cout << "Failed to parse recent-projects.json: " << e.what() << std::endl;
    }


    BuildUi();
    PopulateTemplates(templates);

    PopulateRecentProjects(recent_projects);

  }

  void HomeWindow::BuildUi()
  {
    auto* topRow = new QHBoxLayout();
    topRow->addWidget(BuildRecentProjectsPanel(), 1);
    topRow->addWidget(BuildNewProjectPanel(), 2);

    layout->addLayout(topRow);
  }

  QWidget* HomeWindow::BuildRecentProjectsPanel()
  {
    auto* group = new QGroupBox(tr("Recent projects"), central);

    m_recentProjectsContainer = new QWidget(group);
    m_recentProjectsLayout = new QVBoxLayout(m_recentProjectsContainer);
    m_recentProjectsLayout->addStretch();

    auto* groupLayout = new QVBoxLayout(group);
    groupLayout->addWidget(m_recentProjectsContainer);

    return group;
  }

  QWidget* HomeWindow::BuildNewProjectPanel()
  {
    auto* group = new QGroupBox(tr("New Project"), central);
    auto* groupLayout = new QVBoxLayout(group);

    // Name + C++ standard row
    auto* nameRow = new QHBoxLayout();
    m_nameEdit = new QLineEdit(group);
    m_nameEdit->setPlaceholderText(tr("Name"));
    m_cppStandardCombo = new QComboBox(group);
    m_cppStandardCombo->addItems({ "C++17", "C++20", "C++23" });
    nameRow->addWidget(m_nameEdit, 1);
    nameRow->addWidget(m_cppStandardCombo);
    groupLayout->addLayout(nameRow);

    // Path row
    auto* pathRow = new QHBoxLayout();
    m_pathEdit = new QLineEdit(group);
    m_pathEdit->setPlaceholderText(tr("Path"));
    m_browsePathButton = new QPushButton("...", group);
    pathRow->addWidget(m_pathEdit, 1);
    pathRow->addWidget(m_browsePathButton);
    groupLayout->addLayout(pathRow);

    // Select Template
    auto* templateGroup = new QGroupBox(tr("Select Template"), group);
    auto* templateGroupLayout = new QVBoxLayout(templateGroup);

    m_templatesContainer = new QWidget(templateGroup);
    m_templatesLayout = new QHBoxLayout(m_templatesContainer);
    m_templatesLayout->addStretch();

    m_templatesScrollArea = new QScrollArea(templateGroup);
    m_templatesScrollArea->setWidget(m_templatesContainer);
    m_templatesScrollArea->setWidgetResizable(true);
    m_templatesScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_templatesScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_downloadMoreTemplatesButton = new QPushButton(tr("Download more templates"), templateGroup);

    templateGroupLayout->addWidget(m_templatesScrollArea);
    templateGroupLayout->addWidget(m_downloadMoreTemplatesButton, 0, Qt::AlignRight);

    groupLayout->addWidget(templateGroup, 1);

    // Create button
    m_createProjectButton = new QPushButton(tr("Create new project"), group);
    groupLayout->addWidget(m_createProjectButton, 0, Qt::AlignRight);

    connect(m_browsePathButton, &QPushButton::clicked, this, &HomeWindow::OnBrowsePathClicked);
    connect(m_createProjectButton, &QPushButton::clicked, this, &HomeWindow::OnCreateProjectClicked);
    connect(m_downloadMoreTemplatesButton, &QPushButton::clicked, this, &HomeWindow::OnDownloadMoreTemplatesClicked);

    return group;
  }

  void RecentProjectItemWidget::mouseDoubleClickEvent(QMouseEvent* event)
  {
    if (event->button() == Qt::LeftButton)
      emit DoubleClicked(m_info.path);
    QWidget::mouseDoubleClickEvent(event);
  }

  void HomeWindow::PopulateRecentProjects(const std::vector<RecentProjectInfo>& projects)
  {
    for (auto* item : m_recentProjectItems)
      item->deleteLater();
    m_recentProjectItems.clear();

    // Remove the trailing stretch so new items are inserted before it.
    QLayoutItem* stretchItem = m_recentProjectsLayout->takeAt(m_recentProjectsLayout->count() - 1);

    for (const auto& info : projects)
    {
      auto* item = new RecentProjectItemWidget(info, m_recentProjectsContainer);
      connect(item, &RecentProjectItemWidget::Clicked, this, &HomeWindow::OnRecentProjectClicked);
      connect(item, &RecentProjectItemWidget::DoubleClicked, this, &HomeWindow::OnRecentProjectDoubleClicked);
      m_recentProjectsLayout->addWidget(item);
      m_recentProjectItems.push_back(item);
    }

    m_recentProjectsLayout->addItem(stretchItem);
  }

  void HomeWindow::PopulateTemplates(const std::vector<TemplateInfo>& templates)
  {
    for (auto* item : m_templateItems)
      item->deleteLater();
    m_templateItems.clear();
    m_selectedTemplateId.clear();

    QLayoutItem* stretchItem = m_templatesLayout->takeAt(m_templatesLayout->count() - 1);

    for (const auto& info : templates)
    {
      auto* item = new TemplateItemWidget(info, m_templatesContainer);
      connect(item, &TemplateItemWidget::Clicked, this, &HomeWindow::OnTemplateSelected);
      m_templatesLayout->addWidget(item);
      m_templateItems.push_back(item);
    }

    m_templatesLayout->addItem(stretchItem);
  }

  void HomeWindow::OnBrowsePathClicked()
  {
    const QString dir = QFileDialog::getExistingDirectory(this, tr("Select project path"));
    if (!dir.isEmpty())
      m_pathEdit->setText(dir);
  }

  void AddToJsonList(const std::string& file, const std::string& value)
  {
    nlohmann::json data;

    std::ifstream input(file);

    if (input.is_open())
    {
      input >> data;
      input.close();
    }

    if (!data.is_array())
      data = nlohmann::json::array();

    if (std::find(data.begin(), data.end(), value) == data.end())
      data.push_back(value);

    std::ofstream output(file);
    output << data.dump(4);
  }

  void AddRecentProject(const std::string& path)
  {
    nlohmann::json data;

    std::ifstream input("cache-editor/recent-projects.json");

    if (input)
      input >> data;

    auto& projects = data["recent-projects"];

    if (std::find(projects.begin(), projects.end(), path) == projects.end())
      projects.push_back(path);

    std::ofstream output("cache-editor/recent-projects.json");
    output << data.dump(4);
  }

  void HomeWindow::OnCreateProjectClicked()
  {
    CreateNewProject(
      m_nameEdit->text(),
      m_pathEdit->text(),
      m_selectedTemplateId,
      m_cppStandardCombo->currentText());

    fs::path p(m_pathEdit->text().toStdString());
    const fs::path projectPath = p / m_nameEdit->text().toStdString();
    AddRecentProject(projectPath.string());

    OpenProject(QString::fromStdString(projectPath.string()));
  }

  void HomeWindow::OnRecentProjectDoubleClicked(const QString& path)
  {
    OpenProject(path);
  }

  void HomeWindow::OpenProject(const QString& path)
  {
    // Empty for now — implement actual project loading here (fermer le Home,
    // lancer l'Engine sur `path`, etc.).
    hide();
    OpenProjectRessource(path.toStdString().c_str());
    GetMainWindow()->show();
  }

  void HomeWindow::OnDownloadMoreTemplatesClicked()
  {
    // TODO: fetch additional templates (e.g. from a manifest, similar to the
    // plugin registry) and call PopulateTemplates() with the merged list.
  }

  void HomeWindow::OnRecentProjectClicked(const QString& path)
  {

  }

  void HomeWindow::OnTemplateSelected(const QString& templateId)
  {
    m_selectedTemplateId = templateId;
    for (auto* item : m_templateItems)
      item->SetSelected(item->GetInfo().id == templateId);
  }

  void HomeWindow::CreateNewProject(const QString& name,
                                     const QString& path,
                                     const QString& templateId,
                                     const QString& cppStandard)
  {
    namespace fs = std::filesystem;

    const fs::path parentDir = path.toStdString();
    if (!fs::is_directory(parentDir))
    {
      std::cout << "Not a valid directory" << std::endl;
      return;
    }

    const fs::path projectPath = parentDir / name.toStdString();
    if (fs::exists(projectPath) && !fs::is_empty(projectPath))
    {
      std::cout << "Project directory already exists and is not empty" << std::endl;
      return;
    }

    const fs::path templateSrc = fs::path("Editor/Templates") / templateId.toStdString();
    if (!fs::is_directory(templateSrc))
    {
      std::cout << "Unknown template: " << templateId.toStdString() << std::endl;
      return;
    }

    fs::create_directories(projectPath);
    tools::CopyDirectoryContents(templateSrc, projectPath);
  }
}
