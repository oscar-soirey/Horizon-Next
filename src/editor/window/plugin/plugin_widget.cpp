#include "plugin_widget.h"

#include <filesystem>
#include <QHBoxLayout>
#include <QLabel>

#include "tools/network/access_repo.h"

namespace hn::editor
{

  PluginWidget::PluginWidget(const tools::PluginEntry& entry, bool can_download)
  {
    //Placeholder
    constexpr int kIconSize = 64;
    constexpr const char* kLogoPath = "Editor/Icons/plugin.png";


    //Prend toute la largeur disponible du parent
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    auto* rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(8, 8, 8, 8);
    rootLayout->setSpacing(12);

    // --- Image a gauche ---
    m_iconLabel = new QLabel(this);
    QPixmap pixmap(kLogoPath);
    if (!pixmap.isNull())
    {
      m_iconLabel->setPixmap(
        pixmap.scaled(kIconSize, kIconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    m_iconLabel->setFixedSize(kIconSize, kIconSize);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    rootLayout->addWidget(m_iconLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);

    // --- Colonne d'infos a droite (prend le reste de la largeur) ---
    auto* infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(2);

    m_nameLabel = new QLabel(QString::fromStdString(entry.name), this);
    QFont nameFont = m_nameLabel->font();
    nameFont.setBold(true);
    nameFont.setPointSize(nameFont.pointSize() + 1);
    m_nameLabel->setFont(nameFont);

    m_categoryLabel = new QLabel(QString::fromStdString(entry.category), this);
    m_categoryLabel->setStyleSheet("color: gray;");

    m_versionLabel = new QLabel(
      QString("v%1").arg(QString::fromStdString(entry.version)), this);

    m_descriptionLabel = new QLabel(QString::fromStdString(entry.description), this);
    m_descriptionLabel->setWordWrap(true);

    infoLayout->addWidget(m_nameLabel);
    infoLayout->addWidget(m_categoryLabel);
    infoLayout->addWidget(m_versionLabel);
    infoLayout->addWidget(m_descriptionLabel);
    infoLayout->addStretch();

    // stretch=1 sur infoLayout -> occupe tout l'espace horizontal restant
    rootLayout->addLayout(infoLayout, 1);

    // --- Boutons de download/repair a droite (optionnel) ---
    if (can_download)
    {
      const std::string pluginName = entry.name;
      const std::string downloadLink = entry.downloadUrl;

      const std::filesystem::path destPath =
        std::filesystem::path("Plugins") / entry.category / entry.name;
      const std::string destPathStr = destPath.string();
      const bool alreadyInstalled = std::filesystem::exists(destPath);

      m_downloadButton = new QPushButton(alreadyInstalled ? "Installed" : "Download", this);
      m_downloadButton->setEnabled(!alreadyInstalled);
      rootLayout->addWidget(m_downloadButton, 0, Qt::AlignRight | Qt::AlignVCenter);

      m_repairButton = new QPushButton("Repair", this);
      m_repairButton->setVisible(alreadyInstalled);
      rootLayout->addWidget(m_repairButton, 0, Qt::AlignRight | Qt::AlignVCenter);

      // Logique de telechargement commune, reutilisee par Download et Repair
      auto runDownload = [this, pluginName, downloadLink, destPathStr]()
      {
        m_downloadButton->setEnabled(false);
        m_downloadButton->setText("Downloading...");
        m_repairButton->setEnabled(false);

        qDebug() << "Download clicked for plugin:" << QString::fromStdString(pluginName);

        // Placeholder synchrone — a remplacer par une version asynchrone
        // (signal/slot ou future) si le telechargement est long, pour eviter
        // de geler l'UI pendant l'appel.
        const int success = tools::DownloadPlugin(downloadLink.c_str(), destPathStr.c_str());

        if (success == 0)
        {
          m_downloadButton->setText("Installed");
          m_repairButton->setVisible(true);
          m_repairButton->setEnabled(true);
        }
        else
        {
          m_downloadButton->setEnabled(true);
          m_downloadButton->setText("Download");
          m_repairButton->setEnabled(true);
        }
      };

      connect(m_downloadButton, &QPushButton::clicked, this, runDownload);

      connect(m_repairButton, &QPushButton::clicked, this, [this, destPathStr, runDownload]()
      {
        std::error_code ec;
        std::filesystem::remove_all(destPathStr, ec);
        if (ec)
        {
          qDebug() << "Repair: failed to remove" << QString::fromStdString(destPathStr)
                    << "-" << QString::fromStdString(ec.message());
          return;
        }

        runDownload();
      });
    }
  }
}