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


	int DownloadPlugin(const char* downloadUrl, const char* destPath)
	{
		CURL* curl = curl_easy_init();
		if (!curl)
		{
			std::cerr << "Erreur: impossible d'initialiser curl" << std::endl;
			return -1;
		}

		FILE* fp = fopen(destPath, "wb");
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
			std::remove(destPath); // nettoie le fichier partiel/corrompu
			return -3;
		}
		if (httpCode != 200)
		{
			std::cerr << "Erreur HTTP " << httpCode << " pour " << downloadUrl << std::endl;
			std::remove(destPath);
			return -4;
		}

		return 0; // succes
	}

}