#pragma once

#include "../../ui/editor_window.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>

#include <vector>

namespace hn::editor
{
  // Default placeholder used whenever a thumbnail path isn't provided.
  inline constexpr const char* kPlaceholderImagePath = "Engine/logo.png";

  struct RecentProjectInfo
  {
    QString name;
    QString path;
    QDateTime lastEdit;
    QString thumbnailPath; // optional, falls back to kPlaceholderImagePath
  };

  struct TemplateInfo
  {
    QString id;           // unique identifier, e.g. "blank", "third_person"
    QString displayName;  // e.g. "Blank", "Third-Person"
    QString thumbnailPath; // optional, falls back to kPlaceholderImagePath
  };

  // -----------------------------------------------------------------------
  // Single row inside the "Recent projects" list.
  // -----------------------------------------------------------------------
  class RecentProjectItemWidget : public QWidget
  {
    Q_OBJECT
  public:
    explicit RecentProjectItemWidget(RecentProjectInfo info, QWidget* parent = nullptr);

    const RecentProjectInfo& GetInfo() const { return m_info; }

    signals:
    void Clicked(const QString& path);
    void DoubleClicked(const QString& path);

  protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

  private:
    RecentProjectInfo m_info;
    QLabel* m_thumbnailLabel = nullptr;
    QLabel* m_nameLabel = nullptr;
    QLabel* m_lastEditLabel = nullptr;
  };

  // -----------------------------------------------------------------------
  // Single tile inside the "Select Template" area.
  // -----------------------------------------------------------------------
  class TemplateItemWidget : public QWidget
  {
    Q_OBJECT
  public:
    explicit TemplateItemWidget(TemplateInfo info, QWidget* parent = nullptr);

    const TemplateInfo& GetInfo() const { return m_info; }

    void SetSelected(bool selected);
    bool IsSelected() const { return m_selected; }

  signals:
    void Clicked(const QString& templateId);

  protected:
    void mousePressEvent(QMouseEvent* event) override;

  private:
    void UpdateSelectionStyle();

    TemplateInfo m_info;
    bool m_selected = false;
    QLabel* m_thumbnailLabel = nullptr;
    QLabel* m_nameLabel = nullptr;
  };

  // -----------------------------------------------------------------------
  // Main "Home" window.
  // -----------------------------------------------------------------------
  class HomeWindow : public EditorWindow
  {
    Q_OBJECT
  public:
    HomeWindow();

    // Clears and repopulates the "Recent projects" list.
    void PopulateRecentProjects(const std::vector<RecentProjectInfo>& projects);

    // Clears and repopulates the "Select Template" area.
    void PopulateTemplates(const std::vector<TemplateInfo>& templates);

  protected:
    // Called once the user clicks "Create new project", with everything
    // gathered from the form. Empty for now — implement project creation here.
    virtual void CreateNewProject(const QString& name,
                                   const QString& path,
                                   const QString& templateId,
                                   const QString& cppStandard);

    virtual void OpenProject(const QString& path);

  signals:
    void Clicked(const QString& path);
    void DoubleClicked(const QString& path);

  private slots:
    void OnBrowsePathClicked();
    void OnCreateProjectClicked();
    void OnDownloadMoreTemplatesClicked();
    void OnRecentProjectClicked(const QString& path);
    void OnRecentProjectDoubleClicked(const QString& path);
    void OnTemplateSelected(const QString& templateId);

  private:
    void BuildUi();
    QWidget* BuildRecentProjectsPanel();
    QWidget* BuildNewProjectPanel();

    // Recent projects
    QWidget* m_recentProjectsContainer = nullptr;
    QVBoxLayout* m_recentProjectsLayout = nullptr;
    std::vector<RecentProjectItemWidget*> m_recentProjectItems;

    // New project form
    QLineEdit* m_nameEdit = nullptr;
    QLineEdit* m_pathEdit = nullptr;
    QPushButton* m_browsePathButton = nullptr;
    QComboBox* m_cppStandardCombo = nullptr;
    QPushButton* m_createProjectButton = nullptr;

    // Template selection
    QScrollArea* m_templatesScrollArea = nullptr;
    QWidget* m_templatesContainer = nullptr;
    QHBoxLayout* m_templatesLayout = nullptr;
    QPushButton* m_downloadMoreTemplatesButton = nullptr;
    std::vector<TemplateItemWidget*> m_templateItems;
    QString m_selectedTemplateId;
  };
}