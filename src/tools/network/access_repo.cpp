/*
 * This file was widely generated using AI
 */


#include "access_repo.h"

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out)
{
	out->append((char*)contents, size * nmemb);
	return size * nmemb;
}

namespace hn::tools
{
	std::vector<PluginEntry> FetchPluginManifest(const std::string& manifestUrl)
	{
		std::vector<PluginEntry> plugins;

		CURL* curl = curl_easy_init();
		if (!curl)
		{
			std::cerr << "Erreur: impossible d'initialiser curl" << std::endl;
			return plugins;
		}

		std::string response;
		curl_easy_setopt(curl, CURLOPT_URL, manifestUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "hn-engine-editor");

		CURLcode res = curl_easy_perform(curl);
		long httpCode = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
		curl_easy_cleanup(curl);

		if (res != CURLE_OK)
		{
			std::cerr << "Erreur curl: " << curl_easy_strerror(res) << std::endl;
			return plugins;
		}
		if (httpCode != 200)
		{
			std::cerr << "Erreur HTTP " << httpCode << " pour " << manifestUrl << std::endl;
			return plugins;
		}

		try
		{
			auto json = nlohmann::json::parse(response);
			for (auto& p : json["plugins"])
			{
				plugins.push_back({
					p["name"], p["category"], p["description"],
					p["download_url"], p["version"], p["sha256"]
				});
			}
		}
		catch (const nlohmann::json::exception& e)
		{
			std::cerr << "Erreur parsing JSON: " << e.what() << std::endl;
		}

		return plugins;
	}


	// Extrait le nom de fichier depuis une URL (tout ce qui suit le dernier '/')
	std::string ExtractFileNameFromUrl(const std::string& url)
	{
		size_t lastSlash = url.find_last_of('/');
		if (lastSlash == std::string::npos)
			return url; // pas de '/', l'URL entiere est consideree comme le nom

		std::string fileName = url.substr(lastSlash + 1);

		// Coupe les eventuels parametres de query string (?token=...)
		size_t queryPos = fileName.find('?');
		if (queryPos != std::string::npos)
			fileName = fileName.substr(0, queryPos);

		return fileName;
	}

	int DownloadPlugin(const char* downloadUrl, const char* destDir)
	{
		// Construit le chemin complet : dossier + nom de fichier extrait de l'URL
		std::string fileName = ExtractFileNameFromUrl(downloadUrl);
		if (fileName.empty())
		{
			std::cerr << "Erreur: impossible d'extraire le nom de fichier depuis l'URL" << std::endl;
			return -5;
		}

		std::filesystem::path destPath = std::filesystem::path(destDir) / fileName;

		// Cree le dossier de destination s'il n'existe pas
		std::filesystem::create_directories(destDir);

		CURL* curl = curl_easy_init();
		if (!curl)
		{
			std::cerr << "Erreur: impossible d'initialiser curl" << std::endl;
			return -1;
		}

		FILE* fp = fopen(destPath.string().c_str(), "wb");
		if (!fp)
		{
			std::cerr << "Erreur: impossible d'ouvrir " << destPath << " en ecriture" << std::endl;
			curl_easy_cleanup(curl);
			return -2;
		}

		curl_easy_setopt(curl, CURLOPT_URL, downloadUrl);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nullptr); // fwrite par defaut
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // indispensable pour /releases/download/
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "hn-engine-editor");

		CURLcode res = curl_easy_perform(curl);
		long httpCode = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

		fclose(fp);
		curl_easy_cleanup(curl);

		if (res != CURLE_OK)
		{
			std::cerr << "Erreur curl: " << curl_easy_strerror(res) << std::endl;
			std::filesystem::remove(destPath);
			return -3;
		}
		if (httpCode != 200)
		{
			std::cerr << "Erreur HTTP " << httpCode << " pour " << downloadUrl << std::endl;
			std::filesystem::remove(destPath);
			return -4;
		}

		return 0; // succes
	}

}